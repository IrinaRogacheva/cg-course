#include "FilesystemUtils.h"
#include <codecvt>
#include <fstream>
#ifdef _WIN32
#include <Windows.h>
#include <sdkddkver.h>
#endif

namespace
{
	const size_t FILE_RESERVE_SIZE = 4096;
	const size_t MAX_PATH_SIZE = 4096;

	// ���������� ���� � �������� ������������ �����,
	// .exe �� Windows, ELF �� Linux, MachO �� MacOSX
	std::string GetExecutablePath()
	{
		char buffer[MAX_PATH_SIZE];
		size_t size = sizeof(buffer);
#if defined(_WIN32)
		size = size_t(::GetModuleFileNameA(nullptr, buffer, DWORD(size)));
#elif defined(__linux__)
		ssize_t result = readlink("/proc/self/exe", buffer, size);
		if (result <= 0)
		{
			throw std::runtime_error("Unexpected error on readlink()");
		}
		size = size_t(result);
#elif defined(__apple__)
		uint32_t sizeU32 = uint32_t(size);
		if (_NSGetExecutablePath(buffer, &sizeU32) != 0)
		{
			throw std::runtime_error("Unexpected error on _NSGetExecutablePath()");
		}
		size = size_t(sizeU32);
#endif
		return std::string(buffer, size_t(size));
	}

	// ������� ����������� ���� � ������ ��� ������� ������� � UTF-8.
	//  ���������� SDL2 ��������� ���� � UTF-8 �� ���� ����������.
	std::string ConvertPathToUtf8(const fs::path& path)
	{
#ifdef _WIN32
		const std::wstring& source = path.native();
		std::string destination;
		if (source.length() != 0)
		{
			const int bufLen = WideCharToMultiByte(CP_UTF8, 0, source.c_str(), static_cast<int>(source.length()), NULL, 0, NULL, NULL);
			if (bufLen != 0)
			{
				destination.resize(bufLen + 1);
				WideCharToMultiByte(CP_UTF8, 0, source.c_str(), static_cast<int>(source.length()),
					&destination[0], static_cast<int>(destination.length()), NULL, NULL);
			}
		}
		return destination;
#else // unix-���������.
		return path.native();
#endif
	}
} // namespace

fs::path CFilesystemUtils::GetResourceAbspath(const fs::path& currentPath)
{
	if (currentPath.is_absolute())
	{
		return currentPath;
	}

	const fs::path executableDir = fs::path(GetExecutablePath()).parent_path();
	const fs::path currentDir = fs::current_path();
	for (const auto& dir : { executableDir, currentDir })
	{
		fs::path abspath = fs::absolute(currentPath, dir);
		if (fs::exists(abspath))
		{
			return abspath;
		}
	}
	throw std::runtime_error("Resource not found: " + currentPath.generic_string());
}

std::string CFilesystemUtils::LoadFileAsString(const fs::path& path)
{
	const fs::path abspath = GetResourceAbspath(path);

	std::ifstream input;
	input.open(path.native());
	if (!input.is_open())
	{
		throw std::runtime_error("Cannot open for reading: " + path.generic_string());
	}

	std::string text;
	text.reserve(FILE_RESERVE_SIZE);
	input.exceptions(std::ios::badbit);

	std::string line;
	while (std::getline(input, line))
	{
		text.append(line);
		text.append("\n");
	}

	return text;
}

SDLSurfacePtr CFilesystemUtils::LoadImageByPath(const fs::path& path)
{
	const std::string pathUtf8 = ConvertPathToUtf8(GetResourceAbspath(path));
	SDLSurfacePtr pSurface(IMG_Load(pathUtf8.c_str()));
	if (!pSurface)
	{
		throw std::runtime_error("Cannot find texture at " + path.generic_string());
	}

	return pSurface;
}

#pragma once
#include "Utils.h"
#include <string>
#include "filesystem_alias.h"

class CFilesystemUtils
{
public:
	CFilesystemUtils() = delete;

	static fs::path GetResourceAbspath(const fs::path& path);
	static std::string LoadFileAsString(const fs::path& path);
	static SDLSurfacePtr LoadImageByPath(const fs::path& path);
};

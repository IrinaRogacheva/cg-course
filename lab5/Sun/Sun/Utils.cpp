#include "Utils.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <thread>
#include <cstdlib> // ��� std::memcpy

using namespace std::chrono;

// �������� ������������� SDL2 ������ ���� ��� �� ����� ������ ����������.
// (� ������������� ����� ��� ����������� std::call_once ������ static bool)
void CUtils::InitOnceSDL2()
{
	static bool didInit = false;
	if (!didInit)
	{
		// � ������ ������ ������������ 0.
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			ValidateSDL2Errors();
		}

		const int imageFlags = IMG_INIT_JPG | IMG_INIT_PNG; // IMG_INIT_JPG | 
		// � ������ ������ ������������ ���������� �������� flags.
		if (IMG_Init(imageFlags) != imageFlags)
		{
			ValidateSDL2Errors();
		}
	}
}

void CUtils::ValidateSDL2Errors()
{
	std::string message = SDL_GetError();
	if (!message.empty())
	{
		std::cerr << "SDL2 error: " << message << std::endl;
		std::abort();
	}
}

void CUtils::ValidateOpenGLErrors()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::string message;
		switch (error)
		{
		case GL_INVALID_ENUM:
			message = "invalid enum passed to GL function (GL_INVALID_ENUM)";
			break;
		case GL_INVALID_VALUE:
			message = "invalid parameter passed to GL function (GL_INVALID_VALUE)";
			break;
		case GL_INVALID_OPERATION:
			message = "cannot execute some of GL functions in current state (GL_INVALID_OPERATION)";
			break;
		case GL_STACK_OVERFLOW:
			message = "matrix stack overflow occured inside GL (GL_STACK_OVERFLOW)";
			break;
		case GL_STACK_UNDERFLOW:
			message = "matrix stack underflow occured inside GL (GL_STACK_UNDERFLOW)";
			break;
		case GL_OUT_OF_MEMORY:
			message = "no enough memory to execute GL function (GL_OUT_OF_MEMORY)";
			break;
		default:
			message = "error in some GL extension (framebuffers, shaders, etc)";
			break;
		}
		std::cerr << "OpenGL error: " << message << std::endl;
		std::abort();
	}
}

void CUtils::FlipSurfaceVertically(SDL_Surface& surface)
{
	const auto rowSize = size_t(surface.w * surface.format->BytesPerPixel);
	std::vector<uint8_t> row(rowSize);

	// ��������� �������� ������� �� ��� Y,
	//  ���� ����� ����� �������� � ����������� ��������,
	//  ����������� ������ ������� ����������.
	for (size_t y = 0, height = size_t(surface.h); y < height / 2; ++y)
	{
		auto* pixels = reinterpret_cast<uint8_t*>(surface.pixels);
		auto* upperRow = pixels + rowSize * y;
		auto* lowerRow = pixels + rowSize * (height - y - 1);
		std::memcpy(row.data(), upperRow, rowSize);
		std::memcpy(upperRow, lowerRow, rowSize);
		std::memcpy(lowerRow, row.data(), rowSize);
	}
}

CChronometer::CChronometer()
	: m_lastTime(system_clock::now())
{
}

float CChronometer::GrabDeltaTime()
{
	auto newTime = system_clock::now();
	auto timePassed = duration_cast<milliseconds>(newTime - m_lastTime);
	m_lastTime = newTime;
	return 0.001f * float(timePassed.count());
};

void CChronometer::WaitNextFrameTime(const milliseconds& framePeriod)
{
	system_clock::time_point nextFrameTime = m_lastTime + framePeriod;
	std::this_thread::sleep_until(nextFrameTime);
}
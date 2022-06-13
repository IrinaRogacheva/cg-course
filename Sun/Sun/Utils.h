#pragma once

#include "stdafx.h"
#include <chrono>
#include <glm/fwd.hpp>
#include <memory>
#include <string>

namespace detail
{
	struct SDLWindowDeleter
	{
		void operator()(SDL_Window* ptr)
		{
			SDL_DestroyWindow(ptr);
		}
	};
	struct SDLGLContextDeleter
	{
		void operator()(SDL_GLContext ptr)
		{
			SDL_GL_DeleteContext(ptr);
		}
	};
	struct SDLSurfaceDeleter
	{
		void operator()(SDL_Surface* ptr)
		{
			SDL_FreeSurface(ptr);
		}
	};
} // namespace detail

// Используем unique_ptr с явно заданным функтором удаления вместо delete.
using SDLWindowPtr = std::unique_ptr<SDL_Window, detail::SDLWindowDeleter>;
using SDLGLContextPtr = std::unique_ptr<void, detail::SDLGLContextDeleter>;
using SDLSurfacePtr = std::unique_ptr<SDL_Surface, detail::SDLSurfaceDeleter>;

class CUtils
{
public:
	CUtils() = delete;

	static void InitOnceSDL2();
	static void ValidateSDL2Errors();
	static void ValidateOpenGLErrors();

	static void FlipSurfaceVertically(SDL_Surface& surface);
};

class CChronometer
{
public:
	CChronometer();
	float GrabDeltaTime();

	void WaitNextFrameTime(const std::chrono::milliseconds& framePeriod);

private:
	std::chrono::system_clock::time_point m_lastTime;
};
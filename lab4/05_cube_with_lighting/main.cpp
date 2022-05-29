#include "pch.h"
#include "GLFWInitializer.h"
#include "Window.h"

int main()
{
	GLFWInitializer initGLFW;
	Window window( 1000, 500, "3D Cube" );
	window.Run();
}

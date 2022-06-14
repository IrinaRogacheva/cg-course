#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#ifdef _WIN32
#define _USE_MATH_DEFINES
#include <Windows.h>
#endif

#define GLM_FORCE_RADIANS
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <vector>
#include <memory>


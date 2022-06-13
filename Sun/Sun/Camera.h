#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <SDL2/SDL_events.h>
#include <set>
#include <glm/ext/matrix_transform.hpp>

class CCamera
{
public:
    explicit CCamera(float distance);

    CCamera(const CCamera&) = delete;
    CCamera& operator=(const CCamera&) = delete;

    glm::mat4 GetMatrix() const;

    void Rotate(float xAngleRadians, float yAngleRadians);

private:
    float m_distance = 1;
    glm::mat4x4 m_cameraMatrix = {};
};
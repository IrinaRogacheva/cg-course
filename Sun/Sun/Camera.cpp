#include "Camera.h"
#include <glm/gtx/rotate_vector.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

namespace
{
    // Ортонормируем матрицу 4*4 (это должна быть аффинная матрица)
    glm::dmat4x4 Orthonormalize(const glm::dmat4x4& m)
    {
        // Извлекаем подматрицу 3*3 из матрицы m и ортонормируем её
        const auto normalizedMatrix = glm::orthonormalize(glm::dmat3x3{ m });
        // Заменяем 3 столбца исходной матрицы
        return {
            glm::dvec4{ normalizedMatrix[0], 0.0 },
            glm::dvec4{ normalizedMatrix[1], 0.0 },
            glm::dvec4{ normalizedMatrix[2], 0.0 },
            m[3]
        };
    }
}


CCamera::CCamera(float distance)
    : m_distance(distance)
{
    m_cameraMatrix = glm::lookAt(
        glm::vec3{ 0.0, 0.0, distance },
        glm::vec3{ 0.0, 0.0, 0.0 },
        glm::vec3{ 0.0, 1.0, 0.0 });
}

glm::mat4 CCamera::GetMatrix() const
{
    return m_cameraMatrix;
}

// Вращаем камеру вокруг начала координат
void CCamera::Rotate(float xAngleRadians, float yAngleRadians)
{
    // Извлекаем из 1 и 2 строки матрицы камеры направления осей вращения,
    // совпадающих с экранными осями X и Y.
    // Строго говоря, для этого надо извлекать столбцы их обратной матрицы камеры, но так как
    // матрица камеры ортонормированная, достаточно транспонировать её подматрицу 3*3
    const glm::vec3 xAxis{
        m_cameraMatrix[0][0], m_cameraMatrix[1][0], m_cameraMatrix[2][0]
    };
    const glm::vec3 yAxis{
        m_cameraMatrix[0][1], m_cameraMatrix[1][1], m_cameraMatrix[2][1]
    };
    m_cameraMatrix = glm::rotate(m_cameraMatrix, xAngleRadians, xAxis);
    m_cameraMatrix = glm::rotate(m_cameraMatrix, yAngleRadians, yAxis);

    m_cameraMatrix = Orthonormalize(m_cameraMatrix);
}
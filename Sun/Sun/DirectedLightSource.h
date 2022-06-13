#pragma once
#include "stdafx.h"

class CDirectedLightSource
{
public:
    /// @param index - один из GL_LIGHT*
    CDirectedLightSource(unsigned index);

    glm::vec4 GetAmbient() const;
    glm::vec4 GetDiffuse() const;
    glm::vec4 GetSpecular() const;
    void SetAmbient(const glm::vec4& color);
    void SetDiffuse(const glm::vec4& color);
    void SetSpecular(const glm::vec4& color);

    glm::vec3 GetDirection() const;
    void SetDirection(const glm::vec3& value);

    void Setup() const;

private:
    void SetupImpl()const;
    unsigned GetIndex()const;

    glm::vec4 m_direction;
    glm::vec4 m_ambient;
    glm::vec4 m_diffuse;
    glm::vec4 m_specular;
    unsigned m_index;
};
#pragma once
#include "stdafx.h"

class CPositionLightSource
{
public:
    /// @param index - один из GL_LIGHT*
    CPositionLightSource(unsigned index);

    glm::vec4 GetAmbient() const;
    glm::vec4 GetDiffuse() const;
    glm::vec4 GetSpecular() const;
    void SetAmbient(const glm::vec4& color);
    void SetDiffuse(const glm::vec4& color);
    void SetSpecular(const glm::vec4& color);

    glm::vec3 GetPosition() const;
    void SetPosition(const glm::vec3& value);

    void Setup() const;

private:
    void SetupImpl()const;
    unsigned GetIndex()const;

    glm::vec4 m_position;
    glm::vec4 m_ambient;
    glm::vec4 m_diffuse;
    glm::vec4 m_specular;
    unsigned m_index;
};
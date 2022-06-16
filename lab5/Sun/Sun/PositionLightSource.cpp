#include "PositionLightSource.h"

CPositionLightSource::CPositionLightSource(unsigned index)
    : m_index(index) 
    , m_position(0, 0, 0, 1)
{
}

glm::vec3 CPositionLightSource::GetPosition() const
{
    return { m_position.x, m_position.y, m_position.z };
}

void CPositionLightSource::SetPosition(const glm::vec3& value)
{
    m_position.x = value.x;
    m_position.y = value.y;
    m_position.z = value.z;
}

void CPositionLightSource::Setup() const
{
    SetupImpl();
    // Если GL_POSITION установить как (x, y, z, 1), т.е. как точку
    // в однородных координатах, источник света будет точечным.
    glLightfv(GetIndex(), GL_POSITION, glm::value_ptr(m_position));
}

glm::vec4 CPositionLightSource::GetAmbient() const
{
    return m_ambient;
}

glm::vec4 CPositionLightSource::GetDiffuse() const
{
    return m_diffuse;
}

glm::vec4 CPositionLightSource::GetSpecular() const
{
    return m_specular;
}

void CPositionLightSource::SetAmbient(const glm::vec4& color)
{
    m_ambient = color;
}

void CPositionLightSource::SetDiffuse(const glm::vec4& color)
{
    m_diffuse = color;
}

void CPositionLightSource::SetSpecular(const glm::vec4& color)
{
    m_specular = color;
}

void CPositionLightSource::SetupImpl() const
{
    // Включаем источник света с заданным индексом.
    // Источник действует только когда включен режим GL_LIGHTING!
    glEnable(GLenum(m_index));
    glLightfv(m_index, GL_AMBIENT, glm::value_ptr(m_ambient));
    glLightfv(m_index, GL_DIFFUSE, glm::value_ptr(m_diffuse));
    glLightfv(m_index, GL_SPECULAR, glm::value_ptr(m_specular));
}

unsigned CPositionLightSource::GetIndex() const
{
    return m_index;
}

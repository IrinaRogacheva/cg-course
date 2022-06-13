#include "DirectedLightSource.h"

CDirectedLightSource::CDirectedLightSource(unsigned index)
    : m_index(index)
    , m_direction(0, 0, 0, 0)
{
}

glm::vec3 CDirectedLightSource::GetDirection() const
{
    return { m_direction.x, m_direction.y, m_direction.z };
}

void CDirectedLightSource::SetDirection(const glm::vec3& value)
{
    m_direction.x = value.x;
    m_direction.y = value.y;
    m_direction.z = value.z;
}

void CDirectedLightSource::Setup() const
{
    SetupImpl();
    // Если GL_POSITION установить как (x, y, z, 0), т.е. как вектор
    // в однородных координатах, источник света будет направленным.
    glLightfv(GetIndex(), GL_POSITION, glm::value_ptr(m_direction));
}

glm::vec4 CDirectedLightSource::GetAmbient() const
{
    return m_ambient;
}

glm::vec4 CDirectedLightSource::GetDiffuse() const
{
    return m_diffuse;
}

glm::vec4 CDirectedLightSource::GetSpecular() const
{
    return m_specular;
}

void CDirectedLightSource::SetAmbient(const glm::vec4& color)
{
    m_ambient = color;
}

void CDirectedLightSource::SetDiffuse(const glm::vec4& color)
{
    m_diffuse = color;
}

void CDirectedLightSource::SetSpecular(const glm::vec4& color)
{
    m_specular = color;
}

void CDirectedLightSource::SetupImpl() const
{
    // Включаем источник света с заданным индексом.
    // Источник действует только когда включен режим GL_LIGHTING!
    glEnable(GLenum(m_index));
    glLightfv(m_index, GL_AMBIENT, glm::value_ptr(m_ambient));
    glLightfv(m_index, GL_DIFFUSE, glm::value_ptr(m_diffuse));
    glLightfv(m_index, GL_SPECULAR, glm::value_ptr(m_specular));
}

unsigned CDirectedLightSource::GetIndex() const
{
    return m_index;
}

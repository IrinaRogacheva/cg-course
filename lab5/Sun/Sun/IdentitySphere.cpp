#include "stdafx.h"
#include "IdentitySphere.h"
#include <algorithm>

namespace
{
    const unsigned MIN_PRECISION = 4;

    void CalculateTriangleStripIndicies(std::vector<uint32_t>& indicies,
        unsigned columnCount, unsigned rowCount)
    {
        indicies.clear();
        indicies.reserve((columnCount - 1) * rowCount * 2);
        // вычисляем индексы вершин.
        for (unsigned ci = 0; ci < columnCount - 1; ++ci)
        {
            if (ci % 2 == 0)
            {
                for (unsigned ri = 0; ri < rowCount; ++ri)
                {
                    unsigned index = ci * rowCount + ri;
                    indicies.push_back(index + rowCount);
                    indicies.push_back(index);
                }
            }
            else
            {
                for (unsigned ri = rowCount - 1; ri < rowCount; --ri)
                {
                    unsigned index = ci * rowCount + ri;
                    indicies.push_back(index);
                    indicies.push_back(index + rowCount);
                }
            }
        }
    }

    glm::vec3 GetPositionOnSphere(float u, float v)
    {
        const float radius = 1.f;
        const float latitude = float(M_PI) * (1.f - v); // 𝝅∙(𝟎.𝟓-𝒗)
        const float longitude = float(2.0 * M_PI) * u; // 𝟐𝝅∙𝒖
        const float latitudeRadius = radius * sinf(latitude);

        return { cosf(longitude) * latitudeRadius,
                 cosf(latitude) * radius,
                 sinf(longitude) * latitudeRadius };
    }
}

CIdentitySphere::CIdentitySphere(unsigned slices, unsigned stacks)
{
    Tesselate(slices, stacks);
}

void CIdentitySphere::Update(float deltaTime)
{
    m_rotationAngle = fmodf(m_rotationAngle + m_rotationSpeed * deltaTime, 360);
}

void CIdentitySphere::Draw() const
{
    glPushMatrix();
    glRotatef(m_rotationAngle, 0, 1, 0);
    glRotatef(m_inclinationAngle, 1, 0, 0);
    m_mesh.Draw();
    glPopMatrix();
}

void CIdentitySphere::SetRotationSpeed(float rotationSpeed)
{
    m_rotationSpeed = rotationSpeed;
}

void CIdentitySphere::SetRotationAngle(float rotationAngle)
{
    m_rotationAngle = rotationAngle;
}

void CIdentitySphere::SetInclinationAngle(float inclinationAngle)
{
    m_inclinationAngle = inclinationAngle;
}

void CIdentitySphere::Tesselate(unsigned slices, unsigned stacks)
{
    assert((slices >= MIN_PRECISION) && (stacks >= MIN_PRECISION));

    m_mesh.Clear(MeshType::TriangleStrip);
    m_mesh.m_vertices.reserve(slices * stacks);
    // вычисляем позиции вершин.
    for (unsigned ci = 0; ci < slices; ++ci)
    {
        const float u = float(ci) / float(slices - 1);
        for (unsigned ri = 0; ri < stacks; ++ri)
        {
            const float v = float(ri) / float(stacks - 1);

            SVertexP3NT2 vertex;
            vertex.position = GetPositionOnSphere(u, v);

            // Нормаль к сфере - это нормализованный вектор радиуса к данной точке
            // Поскольку координаты центра равны 0, координаты вектора радиуса
            // будут равны координатам вершины.
            // Благодаря радиусу, равному 1, нормализация не требуется.
            vertex.normal = vertex.position;

            // Обе текстурные координаты должны плавно изменяться от 0 до 1,
            // натягивая прямоугольную картинку на тело вращения.
            // При UV-параметризации текстурными координатами будут u и v.
            vertex.texCoord = { 1.f - u, v };

            m_mesh.m_vertices.push_back(vertex);
        }
    }

    CalculateTriangleStripIndicies(m_mesh.m_indicies, slices, stacks);
}

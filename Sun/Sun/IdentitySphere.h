#pragma once

#include "MeshP3NT2.h"

class CIdentitySphere
{
public:
    CIdentitySphere(unsigned slices, unsigned stacks);

    void Update(float deltaTime);
    void Draw()const;

    void SetInclinationAngle(float inclinationAngle);
    void SetRotationSpeed(float rotationSpeed);
    void SetRotationAngle(float rotationAngle);

private:
    void Tesselate(unsigned slices, unsigned stacks);

    SMeshP3NT2 m_mesh;

    float m_rotationAngle = 0;
    float m_inclinationAngle = 0;
    float m_rotationSpeed = 0;
};
#pragma once
#include <glm/fwd.hpp>
#include <cstdlib>
#include <cstdint>

class CShaderProgram;

class CVertexAttribute
{
public:
    explicit CVertexAttribute(int location);

    void EnablePointer();
    void DisablePointer();

    // �������� normalized - �������� �� �������������� ������������
    //   ��������� � `1.0`, ��� ����� ����������� ��� �������� ����� (color).
    void SetVec3Offset(size_t offset, size_t stride, bool needClamp);

    void SetVec2Offset(size_t offset, size_t stride);

private:
    int m_location = -1;
};

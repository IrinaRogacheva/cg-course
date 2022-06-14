#pragma once
#include <string>

class CProgramInfo
{
public:
    static int GetMaxVertexUniforms();
    static int GetMaxFragmentUniforms();

    explicit CProgramInfo(unsigned programId);

    /// ���������� ���������� uniform-���������� � �������� ���������.
    unsigned GetUniformCount()const;

    /// ������� ���������� � uniform-���������� � ����� stream.
    void PrintUniformInfo(unsigned index, std::ostream& stream)const;

    /// ������� ���������� � ���� uniform-���������� ���������.
    void PrintProgramInfo(std::ostream& stream)const;

private:
    unsigned m_programId = 0;
};

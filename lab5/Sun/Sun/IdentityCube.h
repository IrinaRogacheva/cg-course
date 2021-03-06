#pragma once

#include "MeshP3NT2.h"
#include "FloatRect.h"
#include <vector>
#include <array>

enum class CubeFace
{
    Front = 0,
    Back,
    Top,
    Bottom,
    Left,
    Right,

    NumFaces
};

class CIdentityCube
{
public:
    void Update(float);
    void Draw()const;

    void SetFaceTextureRect(CubeFace face, const CFloatRect& rect);

private:
    void Triangulate();

    static const size_t FACES_COUNT = static_cast<size_t>(CubeFace::NumFaces);

    SMeshP3NT2 m_mesh;
    std::array<CFloatRect, FACES_COUNT> m_textureRects;
    bool m_isDirty = true;
};

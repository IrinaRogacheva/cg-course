#pragma once

#include "IdentityCube.h"
#include "Texture.h"

class CSkybox
{
public:
    CSkybox();
    void Update(float dt);
    void Draw()const;

private:
    CIdentityCube m_cube;
    CTexture2DAtlas m_atlas;
};

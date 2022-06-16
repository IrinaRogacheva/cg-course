#pragma once
#include "DirectedLightSource.h"
#include "PositionLightSource.h"
#include "Skybox.h"
#include "AbstractWindow.h"
#include "IdentitySphere.h"
#include "Camera.h"
#include "Texture.h"
#include <vector>

class CWindow : public CAbstractWindow
{
public:
    CWindow();

protected:
    void OnWindowInit(const glm::ivec2& size) override;
    void OnUpdateWindow(float deltaSeconds) override;
    void OnDrawWindow(const glm::ivec2& size) override;

    void OnDragBegin(int button, const glm::vec2& pos) override;
    void OnDragMotion(const glm::vec2& pos) override;
    void OnDragEnd(const glm::vec2& pos) override;

private:
    void SetupView(const glm::ivec2& size);
    CTexture2DUniquePtr m_pSunTexture;
    CIdentitySphere m_sun;
    CTexture2DUniquePtr m_pEarthTexture;
    CIdentitySphere m_earth;
    CTexture2DUniquePtr m_pMoonTexture;
    CIdentitySphere m_moon;
    float m_earthRotationAngleAroundSun = 0;
    float m_moonRotationAngleAroundEarth = 0;
    std::unique_ptr<CSkybox> m_pSkybox;
    CCamera m_camera;
    CPositionLightSource m_light;
    bool m_leftButtonPressed = false;
    glm::vec2 m_mousePos = {};
};
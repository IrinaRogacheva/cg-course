#pragma once
#include "stdafx.h"
#include <vector>
#include "AbstractWindow.h"
#include "ShaderProgram.h"

class CWindowClient : public CAbstractWindowClient
{
public:
    CWindowClient(CWindow& window);

protected:
    // IWindowClient interface
    void OnUpdateWindow(float deltaSeconds) override;
    void OnDrawWindow() override;

private:
    void CheckOpenGLVersion();
    void SetupView(const glm::ivec2& size);

    void DrawLine() const;

    CShaderProgram m_program;
    float m_time = 0;
};
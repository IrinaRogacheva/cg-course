#include "WindowClient.h"
#include "FilesystemUtils.h"
#include "ProgramUniform.h"

namespace
{
    const float START_X = 0;
    const float END_X = 2.f * float(M_PI);
    const float STEP = float(M_PI) / 1000.f;
    const float Y = 0;

    void SetupOpenGLState()
    {
        // включаем механизмы трёхмерного мира.
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
    }
}

CWindowClient::CWindowClient(CWindow& window)
    : CAbstractWindowClient(window)
{
    const glm::vec4 WHITE = { 1.f, 1.f, 1.f, 1.f };
    const glm::vec4 GREEN = { 0.15f, 0.4f, 0.15f, 1.f };
    GetWindow().SetBackgroundColor(WHITE);
    CheckOpenGLVersion();
    SetupOpenGLState();

    const std::string vertexShader = CFilesystemUtils::LoadFileAsString("res/kanabola.vert");
    m_program.CompileShader(vertexShader, ShaderType::Vertex);
    m_program.Link();
}

void CWindowClient::OnUpdateWindow(float deltaSeconds)
{
    (void)deltaSeconds;
}

void CWindowClient::OnDrawWindow()
{
    SetupView(GetWindow().GetWindowSize());

    m_program.Use();

    DrawLine();
}

void CWindowClient::CheckOpenGLVersion()
{
    // В OpenGL 2.0 шейдерные программы вошли в спецификацию API.
    if (!GLEW_VERSION_2_0)
    {
        throw std::runtime_error("Sorry, but OpenGL 3.2 is not available");
    }
}

void CWindowClient::SetupView(const glm::ivec2& size)
{
    glViewport(0, 0, size.x, size.y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-M_PI, M_PI, -M_PI, M_PI, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void CWindowClient::DrawLine() const
{
    glBegin(GL_LINE_STRIP);
    {
        for (float x = START_X; x <= END_X; x += STEP)
        {
            glVertex2f(x, Y);
        }
    }
    glEnd();
}

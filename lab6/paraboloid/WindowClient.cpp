#include "WindowClient.h"
#include "FilesystemUtils.h"
#include "ProgramUniform.h"

namespace
{
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
    GetWindow().SetBackgroundColor(WHITE);
    CheckOpenGLVersion();
    SetupOpenGLState();

    gluLookAt(
        5, 4, 5,
        0, 0.5, 0,
        0, 1, 0);

    const std::string vertexShader = CFilesystemUtils::LoadFileAsString("res/paraboloid.vert");
    m_program.CompileShader(vertexShader, ShaderType::Vertex);
    m_program.Link();
}

void CWindowClient::OnUpdateWindow(float deltaSeconds)
{
    m_time += deltaSeconds;
}

void CWindowClient::OnDrawWindow()
{
    SetupView(GetWindow().GetWindowSize());

    m_program.Use();
    CProgramUniform time = m_program.FindUniform("u_time");
    time = m_time;

    DrawParaboloid();
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
    // Вычисляем соотношение сторон клиентской области окна
    double aspect = double(size.x) / double(size.y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-aspect, +aspect, -1, 1, 0, 10);
    glMatrixMode(GL_MODELVIEW);
}

void CWindowClient::DrawParaboloid() const
{
    glm::vec3 startPos = { -1, 0, -1 };
    glColor4d(0, 0, 0, 1);
    for (int i = 0; i < 20; ++i)
    {
        glm::vec3 currPos = startPos;
        currPos.x += 0.1f * i;

        for (int j = 0; j < 20; ++j)
        {
            glLineWidth(3);
            glBegin(GL_LINE_LOOP);
            {
                glVertex3f(currPos.x, currPos.y, currPos.z);
                glVertex3f(currPos.x + 0.1f, currPos.y, currPos.z);
                glVertex3f(currPos.x + 0.1f, currPos.y, currPos.z + 0.1f);
                glVertex3f(currPos.x, currPos.y, currPos.z + 0.1f);
            }
            glEnd();

            currPos.z += 0.1f;
        }
    }
}

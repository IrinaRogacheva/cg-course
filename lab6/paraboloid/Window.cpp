#include "Window.h"
#include "Utils.h"
#include <stdexcept>
#include "AbstractWindow.h"
#include <iostream>

namespace
{
    glm::vec2 GetMousePosition(const SDL_MouseButtonEvent& event)
    {
        return { event.x, event.y };
    }

    glm::vec2 GetMousePosition(const SDL_MouseMotionEvent& event)
    {
        return { event.x, event.y };
    }

    void DispatchEvent(const SDL_Event& event, IWindowClient& acceptor)
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            acceptor.OnKeyDown(event.key);
            break;
        case SDL_KEYUP:
            acceptor.OnKeyUp(event.key);
            break;
        case SDL_MOUSEBUTTONDOWN:
            acceptor.OnDragBegin(GetMousePosition(event.button));
            break;
        case SDL_MOUSEBUTTONUP:
            acceptor.OnDragEnd(GetMousePosition(event.button));
            break;
        case SDL_MOUSEMOTION:
            acceptor.OnDragMotion(GetMousePosition(event.motion));
            break;
        }
    }

    void SetupProfileAttributes(ContextProfile profile, ContextMode mode)
    {
        // �������� ����� ����������� � ������� �������������� ����������.
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

        // �������� ������ � ��������� ������������� ���������
        bool makeRobust = true;
        switch (profile)
        {
        case ContextProfile::Compatibility:
            makeRobust = false;
            break;
        case ContextProfile::RobustOpenGL_3_1:
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
            break;
        case ContextProfile::RobustOpenGL_3_2:
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
            break;
        case ContextProfile::RobustOpenGL_4_0:
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
            break;
        }

        unsigned flags = 0;
        if (mode == ContextMode::Debug)
        {
            // �������� ��������� ���������� �������
            //  � ����������� ��������� OpenGL.
            flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
        }
        if (makeRobust)
        {
            // ��������� ��������� ������ ������� �� ������ ������ OpenGL
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            flags |= SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
        }
        else
        {
            // �������� ��������� ���������� ��� �������� �������������
            // �� ������� �������� OpenGL.
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
        }
    }

    void GLAPIENTRY DebugOutputCallback(
        GLenum /*source*/,
        GLenum type,
        GLuint id,
        GLenum /*severity*/,
        GLsizei /*length*/,
        const GLchar* message,
        const void* /*userParam*/)
    {
        // �������� ��� ���������, ����� ������
        if (type != GL_DEBUG_TYPE_ERROR)
        {
            return;
        }
        std::string formatted = "OpenGL error #" + std::to_string(id) + ": " + message;
        std::cerr << formatted << std::endl;
    }

    void SetupDebugOutputCallback()
    {
        if (!GLEW_ARB_debug_output)
        {
            throw std::runtime_error("Cannot use debug output:"
                " it isn't supported by videodriver");
        }

        glEnable(GL_DEBUG_OUTPUT);

        // ���������� ����� ��������� ������ � ��������� ��������,
        //  � ������� ��������� ������.
        // ����� ����� �������� ������������������, �� �� ����
        //  ������ ������ Debug-������ ��� �������������.
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glDebugMessageCallback(DebugOutputCallback, nullptr);
        // ��������� ������������� ������ ������ ���� ��� ���������,
        //  GL_DEBUG_TYPE_ERROR.
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
}


class CWindow::Impl
{
public:
    Impl()
    {
    }

    void Show(const std::string& title, const glm::ivec2& size)
    {
        m_size = size;

        CUtils::InitOnceSDL2();

        // �������� ������ � ��������� ������������� OpenGL.
        //SetupProfileAttributes(m_profile, m_contextMode);

        // ����������� �������� SDL_WINDOWPOS_CENTERED ������ x � y �������� SDL2
        // ���������� ���� � ������ �������� �� ���� x � y.
        // ��� ������������� OpenGL �� ������ ������� ���� SDL_WINDOW_OPENGL.
        m_pWindow.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            size.x, size.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE));
        if (!m_pWindow)
        {
            const std::string reason = SDL_GetError();
            throw std::runtime_error("Cannot create window: " + reason);
        }

        // ������ �������� OpenGL, ��������� � �����.
        m_pGLContext.reset(SDL_GL_CreateContext(m_pWindow.get()));
        if (!m_pGLContext)
        {
            CUtils::ValidateSDL2Errors();
        }
        CUtils::InitOnceGLEW();
    }

    glm::ivec2 GetWindowSize() const
    {
        return m_size;
    }

    void SetClient(IWindowClient* pClient)
    {
        m_pClient = pClient;
    }

    void DoMainLoop()
    {
        const std::chrono::milliseconds FRAME_PERIOD(16);

        SDL_Event event;
        CChronometer chronometer;
        while (true)
        {
            while (SDL_PollEvent(&event) != 0)
            {
                if (!ConsumeEvent(event) && m_pClient)
                {
                    DispatchEvent(event, *m_pClient);
                }
            }
            if (m_isTerminated)
            {
                break;
            }
            // ���������� �����.
            if (m_pClient)
            {
                const float deltaSeconds = chronometer.GrabDeltaTime();
                m_pClient->OnUpdateWindow(deltaSeconds);
            }
            // ������� ������ �����, ���������� � ��������� �����, ����� ������ �����.
            Clear();
            m_pClient->OnDrawWindow();
            CUtils::ValidateOpenGLErrors();
            SwapBuffers();
            chronometer.WaitNextFrameTime(FRAME_PERIOD);
        }
    }

    void SetBackgroundColor(const glm::vec4& color)
    {
        m_clearColor = color;
    }

    void Clear()const
    {
        // ������� ����� ������ ���� ���������� OpenGL
        glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void SwapBuffers()const
    {
        // ����� ������������� ����� � ���� �� ������.
        // ��� ���� ������� ����� ������ ����� ��� ��������� ������ �����.
        // ����� ���� ������� ������ �������� ����� ��������� �� ������� ������� �������.
        SDL_GL_SwapWindow(m_pWindow.get());
    }

    bool ConsumeEvent(const SDL_Event& event)
    {
        bool consumed = false;
        if (event.type == SDL_QUIT)
        {
            m_isTerminated = true;
            consumed = true;
        }
        else if (event.type == SDL_WINDOWEVENT)
        {
            OnWindowEvent(event.window);
            consumed = true;
        }
        return consumed;
    }

private:
    void OnWindowEvent(const SDL_WindowEvent& event)
    {
        if (event.event == SDL_WINDOWEVENT_RESIZED)
        {
            m_size = { event.data1, event.data2 };
        }
    }

    bool m_isTerminated = false;
    ContextProfile m_profile;
    ContextMode m_contextMode;
    IWindowClient* m_pClient = nullptr;
    SDLWindowPtr m_pWindow;
    SDLGLContextPtr m_pGLContext;
    glm::ivec2 m_size;
    glm::vec4 m_clearColor;
};

CWindow::CWindow(ContextProfile profile, ContextMode mode)
    : m_pImpl(new Impl())
{
}

CWindow::~CWindow()
{
}

void CWindow::Show(const std::string& title, const glm::ivec2& size)
{
    m_pImpl->Show(title, size);
}

void CWindow::SetBackgroundColor(const glm::vec4& color)
{
    m_pImpl->SetBackgroundColor(color);
}

void CWindow::SetClient(IWindowClient* pClient)
{
    m_pImpl->SetClient(pClient);
}

glm::ivec2 CWindow::GetWindowSize() const
{
    return m_pImpl->GetWindowSize();
}

void CWindow::DoMainLoop()
{
    m_pImpl->DoMainLoop();
}

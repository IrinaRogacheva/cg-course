#include "AbstractWindow.h"
#include "Utils.h"

class CAbstractWindow::Impl
{
public:
    void Show(const std::string& title, const glm::ivec2& size)
    {
        m_size = size;

        CUtils::InitOnceSDL2();

        // �������� Compatiblity Profile
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

        // �������� ����� ����������� � ������� �������������� ����������.
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

        // ����������� �������� SDL_WINDOWPOS_CENTERED ������ x � y �������� SDL2
        // ���������� ���� � ������ �������� �� ���� x � y.
        // ��� ������������� OpenGL �� ������ ������� ���� SDL_WINDOW_OPENGL.
        m_pWindow.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            size.x, size.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE));

        // ������ �������� OpenGL, ��������� � �����.
        m_pGLContext.reset(SDL_GL_CreateContext(m_pWindow.get()));
        if (!m_pGLContext)
        {
            CUtils::ValidateSDL2Errors();
        }
    }

    glm::ivec2 GetWindowSize() const
    {
        return m_size;
    }

    bool IsTerminated() const
    {
        return m_isTerminated;
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

    SDLWindowPtr m_pWindow;
    SDLGLContextPtr m_pGLContext;
    glm::ivec2 m_size;
    glm::vec4 m_clearColor;
    bool m_isTerminated = false;
};

CAbstractWindow::CAbstractWindow()
    : m_pImpl(new Impl)
{
}

CAbstractWindow::~CAbstractWindow()
{
}

void CAbstractWindow::Show(const std::string& title, const glm::ivec2& size)
{
    m_pImpl->Show(title, size);
    OnWindowInit(size);
}

void CAbstractWindow::DoGameLoop()
{
    const std::chrono::milliseconds FRAME_PERIOD(16);
    SDL_Event event;
    CChronometer chronometer;
    while (true)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (!m_pImpl->ConsumeEvent(event))
            {
                OnWindowEvent(event);
            }
        }
        if (m_pImpl->IsTerminated())
        {
            break;
        }
        // ������� ������ �����, ���������� � ��������� �����, ����� ������ �����.
        m_pImpl->Clear();
        const float deltaSeconds = chronometer.GrabDeltaTime();
        OnUpdateWindow(deltaSeconds);
        OnDrawWindow(m_pImpl->GetWindowSize());

        CUtils::ValidateOpenGLErrors();
        m_pImpl->SwapBuffers();
        chronometer.WaitNextFrameTime(FRAME_PERIOD);
    }
}

void CAbstractWindow::SetBackgroundColor(const glm::vec4& color)
{
    m_pImpl->SetBackgroundColor(color);
}

glm::ivec2 CAbstractWindow::GetWindowSize() const
{
    return m_pImpl->GetWindowSize();
}

void CAbstractWindow::OnWindowEvent(const SDL_Event& event)
{
    DispatchEvent(event);
}

namespace
{
    glm::vec2 GetMousePosition(const SDL_MouseButtonEvent& event)
    {
        return { event.x, event.y };
    }

    Uint8 GetMouseButton(const SDL_MouseButtonEvent& event)
    {
        return event.button;
    }

    glm::vec2 GetMousePosition(const SDL_MouseMotionEvent& event)
    {
        return { event.x, event.y };
    }
}

void CAbstractWindow::DispatchEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        OnKeyDown(event.key);
        break;
    case SDL_KEYUP:
        OnKeyUp(event.key);
        break;
    case SDL_MOUSEBUTTONDOWN:
        OnDragBegin(GetMouseButton(event.button), GetMousePosition(event.button));
        break;
    case SDL_MOUSEBUTTONUP:
        OnDragEnd(GetMousePosition(event.button));
        break;
    case SDL_MOUSEMOTION:
        OnDragMotion(GetMousePosition(event.motion));
        break;
    }
}
#pragma once

#include "stdafx.h"
#include <memory>
#include <glm/fwd.hpp>
#include <SDL2/SDL_events.h>

class CAbstractWindow
{
public:
    CAbstractWindow();
    virtual ~CAbstractWindow();

    CAbstractWindow(const CAbstractWindow&) = delete;
    CAbstractWindow& operator=(const CAbstractWindow&) = delete;

    void Show(const std::string& title, glm::ivec2 const& size);
    void DoGameLoop();

protected:
    void SetBackgroundColor(glm::vec4 const& color);
    glm::ivec2 GetWindowSize()const;

    virtual void OnWindowInit(glm::ivec2 const& size) = 0;
    void OnWindowEvent(const SDL_Event& event);
    virtual void OnUpdateWindow(float deltaSeconds) = 0;
    virtual void OnDrawWindow(glm::ivec2 const& size) = 0;

    // Совершает диспетчеризацию событий SDL по категориям.
    void DispatchEvent(const SDL_Event& event);

    virtual void OnDragBegin(int button, const glm::vec2& pos) { (void)pos; }
    virtual void OnDragMotion(const glm::vec2& pos) { (void)pos; }
    virtual void OnDragEnd(const glm::vec2& pos) { (void)pos; }
    virtual void OnKeyDown(const SDL_KeyboardEvent&) {}
    virtual void OnKeyUp(const SDL_KeyboardEvent&) {}

private:
    class Impl;
    std::unique_ptr<Impl> m_pImpl;
};

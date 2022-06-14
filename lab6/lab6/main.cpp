#include "stdafx.h"
#include "WindowClient.h"

#ifdef _WIN32
#undef main
#endif

int main(int, char* [])
{
    try
    {
        CWindow window;
        window.Show("Kanabola", { 800, 600 });
        CWindowClient client(window);
        window.DoMainLoop();
    }
    catch (const std::exception& ex)
    {
        const char* title = "Fatal Error";
        const char* message = ex.what();
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, nullptr);
    }

    return 0;
}
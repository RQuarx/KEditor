#include "log.hh"
#include "sdl/window.hh"

using sdl::Window;
using enum LogLevel;


auto
Window::create(const std::string &title, SDL_WindowFlags flags)
    -> std::optional<Window>
{
    Window window;
    window.m_object = SDL_CreateWindow(title.c_str(), 600, 300, flags);

    if (window.m_object == nullptr) return std::nullopt;

    return window;
}


auto
Window::get_size() -> std::optional<WindowSize>
{
    WindowSize size;

    if (!SDL_GetWindowSizeInPixels(m_object, &size.w, &size.h))
        return std::nullopt;
    return size;
}

#include <SDL3/SDL_mouse.h>

#include "exceptions.hh"
#include "sdl/instance.hh"
#include "sdl/window.hh"

using sdl::Window;


Window::Window(const std::string &title, SDL_WindowFlags flags)
{
    m_object = SDL_CreateWindow(title.c_str(), 600, 300, flags);

    if (m_object == nullptr)
        throw sdl::Exception { "Window::Window(): {}", get_error() };
}


auto
Window::get_size() -> std::optional<WindowSize>
{
    WindowSize size;

    if (!SDL_GetWindowSizeInPixels(m_object, &size.w, &size.h))
        return std::nullopt;
    return size;
}


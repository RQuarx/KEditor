#include <functional>

#include <SDL3/SDL_mouse.h>

#include "sdl/instance.hh"
#include "sdl/window.hh"

using sdl::Window;


Window::Window(const std::string &title, SDL_WindowFlags flags)
{
    m_object = SDL_CreateWindow(title.c_str(), 600, 300, flags);

    if (m_object == nullptr) throw sdl::Exception {};
}


auto
Window::get_size() -> std::optional<WindowSize>
{
    WindowSize size;

    if (!SDL_GetWindowSizeInPixels(m_object, &size.w, &size.h))
        return std::nullopt;
    return size;
}


void
Window::set_text_input_state(bool state)
{
    std::function<bool(SDL_Window *)> func { state ? SDL_StartTextInput
                                                   : SDL_StopTextInput };
    if (!func(m_object)) throw sdl::Exception {};
}


auto
Window::get_text_input_state() const noexcept -> bool
{
    return SDL_TextInputActive(m_object);
}

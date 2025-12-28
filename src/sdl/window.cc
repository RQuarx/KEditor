#include <functional>

#include <SDL3/SDL_mouse.h>

#include "sdl/instance.hh"
#include "sdl/window.hh"

using sdl::window;


window::window(const std::string &title, SDL_WindowFlags flags)
{
    m_object = SDL_CreateWindow(title.c_str(), 600, 300, flags);

    if (m_object == nullptr) throw sdl::exception {};
}


auto
window::get_size() -> std::optional<window_size>
{
    window_size size;

    if (!SDL_GetWindowSizeInPixels(m_object, &size.w, &size.h))
        return std::nullopt;
    return size;
}


void
window::set_text_input_state(bool state)
{
    std::function<bool(SDL_Window *)> func { state ? SDL_StartTextInput
                                                   : SDL_StopTextInput };
    if (!func(m_object)) throw sdl::exception {};
}


auto
window::get_text_input_state() const noexcept -> bool
{
    return SDL_TextInputActive(m_object);
}

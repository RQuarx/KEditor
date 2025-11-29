#ifndef _KEDITOR_SDL_WINDOW_HH
#define _KEDITOR_SDL_WINDOW_HH
#include <optional>
#include <string>

#include <SDL3/SDL_video.h>

#include "sdl/resource.hh"


namespace sdl
{
    struct WindowSize
    {
        int w;
        int h;
    };


    /* Holds and manage an SDL_Window */
    class Window : public Resource<SDL_Window, SDL_DestroyWindow>
    {
    public:
        using Resource::Resource;


        [[nodiscard]]
        static auto create(const std::string &title, SDL_WindowFlags flags)
            -> std::optional<Window>;


        [[nodiscard]]
        auto get_size() -> std::optional<WindowSize>;
    };

}

#endif /* _KEDITOR_SDL_WINDOW_HH */

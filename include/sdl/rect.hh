#ifndef _KEDITOR_SDL_RECT_HH
#define _KEDITOR_SDL_RECT_HH
#include <SDL3/SDL_rect.h>


namespace sdl
{
    using FPoint = SDL_FPoint;
    using FRect  = SDL_FRect;

    struct FSize
    {
        float w;
        float h;
    };
}

#endif /* _KEDITOR_SDL_RECT_HH */

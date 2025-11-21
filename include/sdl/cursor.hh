#ifndef _KEDITOR_SDL_CURSOR_HH
#define _KEDITOR_SDL_CURSOR_HH
#include <SDL3/SDL_mouse.h>


namespace sdl
{
    auto set_cursor(SDL_SystemCursor cursor) -> bool;
    auto set_cursor() -> bool;
}

#endif /* _KEDITOR_SDL_CURSOR_HH */

#ifndef _KEDITOR_SDL_CURSOR_HH
#define _KEDITOR_SDL_CURSOR_HH
#include <SDL3/SDL_mouse.h>


namespace sdl
{
    void set_cursor(SDL_SystemCursor cursor);
    void set_cursor();
}

#endif /* _KEDITOR_SDL_CURSOR_HH */

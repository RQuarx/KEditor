#include <unordered_map>

#include "sdl/cursor.hh"
#include "sdl/instance.hh"
#include "sdl/resource.hh"


namespace
{
    std::unordered_map<SDL_SystemCursor,
                       sdl::resource<SDL_Cursor, SDL_DestroyCursor>>
        CURSORS;
}


namespace sdl
{
    using cursor = sdl::resource<SDL_Cursor, SDL_DestroyCursor>;


    void
    set_cursor(SDL_SystemCursor c)
    {
        if (!CURSORS.contains(c))
            CURSORS[c] = cursor { SDL_CreateSystemCursor(c) };

        if (!SDL_SetCursor(CURSORS[c])) throw sdl::exception {};
    }


    void
    set_cursor()
    {
        if (!CURSORS.contains(SDL_SYSTEM_CURSOR_DEFAULT))
            CURSORS[SDL_SYSTEM_CURSOR_DEFAULT]
                = cursor { SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT) };

        if (!SDL_SetCursor(CURSORS[SDL_SYSTEM_CURSOR_DEFAULT]))
            throw sdl::exception {};
    }
}



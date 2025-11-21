#include <unordered_map>

#include "sdl/cursor.hh"
#include "sdl/resource.hh"


namespace
{
    std::unordered_map<SDL_SystemCursor,
                       sdl::Resource<SDL_Cursor, SDL_DestroyCursor>>
        CURSORS;
}


namespace sdl
{
    using Cursor = sdl::Resource<SDL_Cursor, SDL_DestroyCursor>;


    auto
    set_cursor(SDL_SystemCursor cursor) -> bool
    {
        if (!CURSORS.contains(cursor))
            CURSORS[cursor] = Cursor { SDL_CreateSystemCursor(cursor) };

        return SDL_SetCursor(CURSORS[cursor].raw());
    }


    auto
    set_cursor() -> bool
    {
        if (!CURSORS.contains(SDL_SYSTEM_CURSOR_DEFAULT))
            CURSORS[SDL_SYSTEM_CURSOR_DEFAULT]
                = Cursor { SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT) };

        return SDL_SetCursor(CURSORS[SDL_SYSTEM_CURSOR_DEFAULT].raw());
    }
}



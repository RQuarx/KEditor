#include <unordered_map>

#include "exceptions.hh"
#include "sdl/cursor.hh"
#include "sdl/instance.hh"
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


    void
    set_cursor(SDL_SystemCursor cursor)
    {
        if (!CURSORS.contains(cursor))
            CURSORS[cursor] = Cursor { SDL_CreateSystemCursor(cursor) };

        if (!SDL_SetCursor(CURSORS[cursor].raw())) throw sdl::Exception {};
    }


    void
    set_cursor()
    {
        if (!CURSORS.contains(SDL_SYSTEM_CURSOR_DEFAULT))
            CURSORS[SDL_SYSTEM_CURSOR_DEFAULT]
                = Cursor { SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT) };

        if (!SDL_SetCursor(CURSORS[SDL_SYSTEM_CURSOR_DEFAULT].raw()))
            throw sdl::Exception {};
    }
}



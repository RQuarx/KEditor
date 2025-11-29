#ifndef _KEDITOR_SDL_TEXTURE_HH
#define _KEDITOR_SDL_TEXTURE_HH
#include <SDL3/SDL_render.h>

#include "sdl/resource.hh"


namespace sdl
{
    class Texture : Resource<SDL_Texture, SDL_DestroyTexture>
    {
    public:
    };
}

#endif /* _KEDITOR_SDL_TEXTURE_HH */

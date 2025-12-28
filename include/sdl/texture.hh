#pragma once
#include <SDL3/SDL_render.h>

#include "sdl/resource.hh"


namespace sdl
{
    class texture : resource<SDL_Texture, SDL_DestroyTexture>
    {
    public:
    };
}

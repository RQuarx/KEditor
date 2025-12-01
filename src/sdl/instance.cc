#include <format>

#include <SDL3_ttf/SDL_ttf.h>

#include "sdl/instance.hh"

using sdl::Instance;


auto
sdl::get_error() noexcept -> std::string
{
    return SDL_GetError();
}


Instance::Instance(SDL_InitFlags flags)
{
    if (Instance::initialized) return;

    if (!SDL_Init(flags))
        throw InitError { "Failed to initialize SDL: {}", SDL_GetError() };

    if (!TTF_Init())
        throw InitError { "Failed to initialize SDL_TTF: {}",
                               SDL_GetError() };
}


Instance::~Instance()
{
    TTF_Quit();
    SDL_Quit();
}

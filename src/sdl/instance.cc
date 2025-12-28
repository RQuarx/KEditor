#include <format>

#include <SDL3_ttf/SDL_ttf.h>

#include "sdl/instance.hh"

using sdl::instance;


auto
sdl::get_error() noexcept -> std::string
{
    return SDL_GetError();
}


instance::instance(SDL_InitFlags flags)
{
    if (instance::initialized) return;

    if (!SDL_Init(flags))
        throw init_error { "Failed to initialize SDL: {}", SDL_GetError() };

    if (!TTF_Init())
        throw init_error { "Failed to initialize SDL_TTF: {}", SDL_GetError() };
}


instance::~instance()
{
    TTF_Quit();
    SDL_Quit();
}

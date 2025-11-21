#ifndef _KEDITOR_SDL_INSTANCE_HH
#define _KEDITOR_SDL_INSTANCE_HH
#include <string>

#include <SDL3/SDL_init.h>


namespace sdl
{
    [[nodiscard]]
    auto get_error() noexcept -> std::string;


    class Instance
    {
    public:
        inline static bool initialized { false };


        explicit Instance(SDL_InitFlags flags);
        ~Instance();

        Instance(Instance &)  = delete;
        Instance(Instance &&) = delete;

        auto operator=(Instance &)  = delete;
        auto operator=(Instance &&) = delete;
    };
}

#endif /* _KEDITOR_SDL_INSTANCE_HH */

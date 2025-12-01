#ifndef _KEDITOR_SDL_INSTANCE_HH
#define _KEDITOR_SDL_INSTANCE_HH
#include <string>

#include <SDL3/SDL_init.h>

#include "exceptions.hh"


namespace sdl
{
    [[nodiscard]]
    auto get_error() noexcept -> std::string;


    class Exception : public kei::Exception
    {
    public:
        using kei::Exception::Exception;

        Exception() : kei::Exception("{}", sdl::get_error()) {}
    };


#define EXCEPTION_OBJ(name)                \
class name : public Exception /* NOLINT */ \
{                                          \
public:                                    \
    using Exception::Exception;            \
}

    EXCEPTION_OBJ(InitError);


#undef EXCEPTION_OBJ


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

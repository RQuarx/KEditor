#pragma once
#include <string>

#include <SDL3/SDL_init.h>

#include "exceptions.hh"


namespace sdl
{
    [[nodiscard]]
    auto get_error() noexcept -> std::string;


    class exception : public kei::exception
    {
    public:
        using kei::exception::exception;

        exception() : kei::exception("{}", sdl::get_error()) {}
    };


#define EXCEPTION_OBJ(name)                \
class name : public exception /* NOLINT */ \
{                                          \
public:                                    \
    using exception::exception;            \
}

    EXCEPTION_OBJ(init_error);


#undef EXCEPTION_OBJ


    class instance
    {
    public:
        inline static bool initialized { false };


        explicit instance(SDL_InitFlags flags);
        ~instance();

        instance(instance &)  = delete;
        instance(instance &&) = delete;

        auto operator=(instance &)  = delete;
        auto operator=(instance &&) = delete;
    };
}

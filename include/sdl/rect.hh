#ifndef _KEDITOR_SDL_RECT_HH
#define _KEDITOR_SDL_RECT_HH
#include <format>
#include <string_view>

#include <SDL3/SDL_rect.h>


namespace sdl
{
    using FPoint = SDL_FPoint;
    using FRect  = SDL_FRect;

    struct FSize
    {
        float w;
        float h;
    };
}


template <> struct std::formatter<sdl::FRect> : std::formatter<std::string_view>
{
    auto
    format(sdl::FRect r, std::format_context &ctx) const
    {
        return std::formatter<std::string_view>::format(
            std::format("{{x: {}, y: {}, w: {}, h: {}}}", r.x, r.y, r.w, r.h),
            ctx);
    }
};


template <>
struct std::formatter<sdl::FPoint> : std::formatter<std::string_view>
{
    auto
    format(sdl::FPoint p, std::format_context &ctx) const
    {
        return std::formatter<std::string_view>::format(
            std::format("{{x: {}, y: {}}}", p.x, p.y), ctx);
    }
};

#endif /* _KEDITOR_SDL_RECT_HH */

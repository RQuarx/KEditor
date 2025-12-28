#pragma once
#include <format>
#include <string_view>

#include <SDL3/SDL_rect.h>


namespace sdl
{
    using fpoint = SDL_FPoint;
    using frect  = SDL_FRect;

    struct fsize
    {
        float w;
        float h;
    };


    [[nodiscard]]
    constexpr auto
    is_point_in_rect(const sdl::frect &rect, const sdl::fpoint &point) noexcept
        -> bool
    {
        float x2 { rect.x + rect.w };
        float y2 { rect.y + rect.h };

        return (point.x >= rect.x && point.x < x2 && point.y >= rect.y
                && point.y < y2);
    }
}


template <> struct std::formatter<sdl::frect> : std::formatter<std::string_view>
{
    auto
    format(sdl::frect r, std::format_context &ctx) const
    {
        return std::formatter<std::string_view>::format(
            std::format("{{x: {}, y: {}, w: {}, h: {}}}", r.x, r.y, r.w, r.h),
            ctx);
    }
};


template <>
struct std::formatter<sdl::fpoint> : std::formatter<std::string_view>
{
    auto
    format(sdl::fpoint p, std::format_context &ctx) const
    {
        return std::formatter<std::string_view>::format(
            std::format("{{x: {}, y: {}}}", p.x, p.y), ctx);
    }
};

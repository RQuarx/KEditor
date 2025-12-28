#pragma once
#include <cmath>
#include <cstdint>

#include <SDL3/SDL_pixels.h>

#include "exceptions.hh"

#define COLOR_TO_PARAM(color) \
    (color).r, (color).g, (color).b, (color).a


namespace sdl
{
    struct alignas(alignof(std::uint32_t)) color
    {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t a { 255 };

        constexpr color() = default;

        constexpr color(std::uint8_t r,
                        std::uint8_t g,
                        std::uint8_t b,
                        std::uint8_t a = 255)
            : r(r), g(g), b(b), a(a)
        {
        }

        color(std::string_view string)
        {
            if (!string.starts_with('#'))
                throw kei::invalid_argument { "Provided string is not a hex" };

            *this = from_hex(string.data(), string.length());
        }


        static constexpr auto
        from_rgb(std::uint32_t value) -> color
        {
            return color { static_cast<std::uint8_t>((value >> 16) & 0xFF),
                           static_cast<std::uint8_t>((value >> 8) & 0xFF),
                           static_cast<std::uint8_t>((value) & 0xFF), 255 };
        }


        static constexpr auto
        from_rgba(std::uint32_t value) -> color
        {
            return color {
                static_cast<std::uint8_t>((value >> 24) & 0xFF),
                static_cast<std::uint8_t>((value >> 16) & 0xFF),
                static_cast<std::uint8_t>((value >> 8) & 0xFF),
                static_cast<std::uint8_t>((value) & 0xFF),
            };
        }

        static constexpr auto
        from_rgb_hex(const char *hex) -> color
        {
            return from_hex(hex, 6);
        }

        static constexpr auto
        from_rgba_hex(const char *hex) -> color
        {
            return from_hex(hex, 8);
        }


        [[nodiscard]]
        constexpr auto
        to_rgb_uint() const -> std::uint32_t
        {
            return (r << 16) | (g << 8) | b;
        }


        [[nodiscard]]
        constexpr auto
        to_rgba_uint() const -> std::uint32_t
        {
            return (r << 24) | (g << 16) | (b << 8) | a;
        }


        [[nodiscard]]
        constexpr auto
        to_color() const -> SDL_Color
        {
            return { r, g, b, a };
        }


        [[nodiscard]]
        constexpr auto
        to_fcolor() const -> SDL_FColor
        {
            return { r / 255.0F, g / 255.0F, b / 255.0F, a / 255.0F };
        }


        [[nodiscard]]
        static constexpr auto
        lerp(color a, color b, float t) -> color
        {
            auto lerp8 {
                [](std::uint8_t a, std::uint8_t b, float t) -> std::uint8_t
                {
                    return static_cast<std::uint8_t>(std::lerp(
                        static_cast<float>(a), static_cast<float>(b), t));
                }
            };

            return {
                lerp8(a.r, b.r, t),
                lerp8(a.g, b.g, t),
                lerp8(a.b, b.b, t),
                lerp8(a.a, b.a, t),
            };
        }

    private:
        static constexpr auto
        hex_val(char c) -> int
        {
            return (c >= '0' && c <= '9') ? c - '0'
                 : (c >= 'A' && c <= 'F') ? c - 'A' + 10
                 : (c >= 'a' && c <= 'f') ? c - 'a' + 10
                                          : -1;
        }


        static constexpr auto
        from_hex(const char *s, int len) -> color
        {
            int actual { 0 };
            while (s[actual] != '\0') actual++;

            if (actual != len)
                throw kei::invalid_argument {
                    "Hexadecimal literal has wrong length"
                };

            auto read_byte {
                [&s](int pos) -> std::uint8_t
                {
                    int hi { hex_val(s[pos]) };
                    int lo { hex_val(s[pos + 1]) };

                    if (hi < 0 || lo < 0)
                        throw kei::invalid_argument { "Invalid hex digit" };

                    return static_cast<std::uint8_t>((hi * 16) + lo);
                }
            };

            if (len == 6)
                return color { read_byte(0), read_byte(2), read_byte(4), 255 };
            return color { read_byte(0), read_byte(2), read_byte(4),
                           read_byte(6) };
        }
    };
}


static constexpr auto
operator""_rgb(const unsigned long long color) -> sdl::color
{
    return sdl::color::from_rgb(color);
}


static constexpr auto
operator""_rgba(const unsigned long long color) -> sdl::color
{
    return sdl::color::from_rgba(color);
}


static constexpr auto
operator""_rgb(const char *hex, std::size_t len) -> sdl::color
{
    if (len != 6)
        throw kei::invalid_argument {
            "Hex literals must be 6 characters for _rgb"
        };

    return sdl::color::from_rgb_hex(hex);
}


static constexpr auto
operator""_rgba(const char *hex, std::size_t len) -> sdl::color
{
    if (len != 8)
        throw kei::invalid_argument {
            "Hex literals must be 8 characters for _rgba"
        };

    return sdl::color::from_rgba_hex(hex);
}

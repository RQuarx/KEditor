#pragma once
#include "widgets/box.hh"


namespace widget
{
    class frame : public box
    {
    public:
        frame(sdl::frect rect) noexcept;

        auto render(sdl::renderer &render) -> frame & override;


        auto set_border_color(sdl::color color) noexcept -> frame &;
        auto set_border_width(std::int32_t px) noexcept -> frame &;

        [[nodiscard]] auto get_border_color() const noexcept -> sdl::color;
        [[nodiscard]] auto get_border_width() const noexcept -> std::int32_t;

    protected:
        sdl::color   mp_border_color;
        std::int32_t mp_border_width;
    };
}

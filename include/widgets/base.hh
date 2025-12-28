#pragma once
#include "sdl/event.hh"
#include "sdl/renderer.hh"


namespace widget
{
    class base
    {
    public:
        base(sdl::frect rect) noexcept;

        virtual auto add_event_callbacks(sdl::event_handler &handler) noexcept
            -> base &;
        virtual auto render(sdl::renderer &render) -> base & = 0;


        auto set_position(sdl::fpoint pos) noexcept -> base &;
        auto set_visible(bool visible) noexcept -> base &;
        auto set_color(sdl::color color) noexcept -> base &;
        auto set_rect(sdl::frect rect) noexcept -> base &;
        auto set_size(sdl::fsize size) noexcept -> base &;


        [[nodiscard]] auto get_position() const noexcept -> sdl::fpoint;
        [[nodiscard]] auto get_visible() const noexcept -> bool;
        [[nodiscard]] auto get_color() const noexcept -> sdl::color;
        [[nodiscard]] auto get_rect() const noexcept -> sdl::frect;
        [[nodiscard]] auto get_size() const noexcept -> sdl::fsize;

    protected:
        sdl::frect mp_rect;
        sdl::color mp_color;

        bool mp_visible;
    };
}

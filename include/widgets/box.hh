#ifndef _KEDITOR_WIDGETS_BOX_HH
#define _KEDITOR_WIDGETS_BOX_HH
#include "sdl/color.hh"
#include "sdl/event.hh"
#include "sdl/rect.hh"
#include "sdl/renderer.hh"


namespace widget
{
    class Box
    {
    public:
        Box(sdl::FRect                widget_area,
            sdl::Color                widget_color,
            std::optional<sdl::Color> border_color = std::nullopt) noexcept;


        virtual ~Box() = default;


        [[nodiscard]]
        auto get_area(bool include_border = true) const noexcept -> sdl::FRect;

        [[nodiscard]]
        auto get_color() const noexcept -> sdl::Color;


        [[nodiscard]]
        auto get_border_color() const noexcept -> std::optional<sdl::Color>;


        void set_position(sdl::FPoint new_position) noexcept;
        void set_size(sdl::FSize new_size) noexcept;

        void set_color(sdl::Color new_color) noexcept;
        void set_border_color(sdl::Color new_color) noexcept;

        void set_border_width(unsigned int px) noexcept;

        [[nodiscard]]
        auto get_border_width() const noexcept -> unsigned int;


        [[nodiscard]]
        auto is_visible() const noexcept -> bool;
        void set_visible(bool visible) noexcept;

        virtual void add_event_callbacks(sdl::EventHandler &handler);

        virtual void render(sdl::Renderer &render);

    private:
        sdl::FRect m_area;

        sdl::Color                m_color;
        std::optional<sdl::Color> m_border_color;

        unsigned int m_border_width_px;

        bool m_visible;
    };
}

#endif /* _KEDITOR_WIDGETS_BOX_HH */

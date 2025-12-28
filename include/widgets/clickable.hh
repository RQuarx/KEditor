#pragma once
#include "widgets/hoverable.hh"


namespace widget
{
    class clickable : public hoverable
    {
    public:
        enum class click_type : std::uint8_t
        {
            up,
            down
        };


        clickable(sdl::frect rect) noexcept;

        auto add_event_callbacks(sdl::event_handler &handler) noexcept
            -> clickable & override;
        auto render(sdl::renderer &render) -> clickable & override;


        [[nodiscard]]
        auto signal_clicked() noexcept -> sdl::signal<void, click_type> &;


        auto set_clicked_color(sdl::color color) noexcept -> clickable &;
        [[nodiscard]] auto get_clicked_color() const noexcept -> sdl::color;
        [[nodiscard]] auto get_clicked() const noexcept -> bool;

    protected:
        sdl::color mp_clicked_color;
        bool       mp_clicked;

    private:
        sdl::signal<void, click_type> m_signal_clicked;

        sdl::connection m_button_up_OMC;
        sdl::connection m_button_down_OMC;


        auto mf_on_mouse_click(const sdl::event &event, sdl::renderer &render)
            -> sdl::event_return_type;
    };
}

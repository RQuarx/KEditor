#pragma once
#include "widgets/clickable.hh"
#include "widgets/frame.hh"
#include "widgets/label.hh"


namespace widget
{
    class button final : public frame
    {
    public:
        button(sdl::frect                        rect,
               const std::shared_ptr<sdl::font> &font   = {},
               std::string                       string = {});

        auto add_event_callbacks(sdl::event_handler &handler) noexcept
            -> button & override;
        auto render(sdl::renderer &render) -> frame & override;


        [[nodiscard]]
        auto signal_clicked() noexcept -> sdl::signal<void> &;


        auto set_font(const std::shared_ptr<sdl::font> &font) noexcept
            -> button &;
        auto set_string(std::string string) noexcept -> button &;

        auto set_text_color(sdl::color color) noexcept -> button &;
        auto set_hover_text_color(sdl::color color) noexcept -> button &;
        auto set_clicked_text_color(sdl::color color) noexcept -> button &;
        auto set_hover_color(sdl::color color) noexcept -> button &;
        auto set_clicked_color(sdl::color color) noexcept -> button &;

        auto set_fadeout_time(std::uint64_t ms) noexcept -> button &;
        auto set_fadein_time(std::uint64_t ms) noexcept -> button &;

        auto set_discard_click_time(std::uint64_t ms) noexcept -> button &;


        [[nodiscard]]
        auto get_font() const noexcept -> std::shared_ptr<sdl::font>;
        [[nodiscard]] auto get_string() const noexcept -> std::string;

        [[nodiscard]] auto get_text_color() const noexcept -> sdl::color;
        [[nodiscard]] auto get_hover_text_color() const noexcept -> sdl::color;
        [[nodiscard]] auto get_clicked_text_color() const noexcept
            -> sdl::color;
        [[nodiscard]] auto get_hover_color() const noexcept -> sdl::color;
        [[nodiscard]] auto get_clicked_color() const noexcept -> sdl::color;

        [[nodiscard]] auto get_fadeout_time() const noexcept -> std::uint64_t;
        [[nodiscard]] auto get_fadein_time() const noexcept -> std::uint64_t;

        [[nodiscard]]
        auto get_discard_click_time() const noexcept -> std::uint64_t;

    private:
        clickable            m_clickable;
        std::optional<label> m_label;

        sdl::color m_hover_text_color { 0xffffff_rgb };
        sdl::color m_clicked_text_color { 0xffffff_rgb };

        sdl::signal<void> m_signal_clicked;
        sdl::connection   m_OC_connection;

        std::uint64_t m_discard_click_ms;
        std::uint64_t m_down_button_clicked_ms;


        void mf_on_click(clickable::click_type type);
    };
}

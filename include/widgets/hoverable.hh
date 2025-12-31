#pragma once
#include "widgets/base.hh"


namespace widget
{
    class hoverable : public base
    {
        enum class fade_state : std::uint8_t
        {
            none,
            to_hover, /* fade-in */
            to_normal /* fade-out */
        };

    public:
        hoverable(sdl::frect rect) noexcept;


        auto add_event_callbacks(sdl::event_handler &handler) noexcept
            -> hoverable & override;
        auto render(sdl::renderer &render) -> hoverable & override;


        [[nodiscard]]
        auto signal_on_hover_entry() noexcept -> sdl::signal<void> &;

        [[nodiscard]]
        auto signal_on_hover_exit() noexcept -> sdl::signal<void> &;


        auto set_fadeout_time(std::uint64_t ms) noexcept -> hoverable &;
        auto set_fadein_time(std::uint64_t ms) noexcept -> hoverable &;
        auto set_hover_color(sdl::color color) noexcept -> hoverable &;

        [[nodiscard]] auto get_fadeout_time() const noexcept -> std::uint64_t;
        [[nodiscard]] auto get_fadein_time() const noexcept -> std::uint64_t;
        [[nodiscard]] auto get_hover_color() const noexcept -> sdl::color;
        [[nodiscard]] auto get_hovered() const noexcept -> bool;

    protected:
        sdl::color mp_hover_color;
        bool       mp_hovered;

        std::uint64_t mp_fadeout_time_ms;
        std::uint64_t mp_fadein_time_ms;

    private:
        fade_state m_fade_state;

        sdl::color m_current_color;
        sdl::color m_fade_start_color;
        sdl::color m_fade_target_color;

        std::uint64_t m_previous_frame_time;
        float         m_fade_elapsed;

        sdl::connection m_OMM_connection;

        sdl::signal<void> m_on_hover_entry_signal;
        sdl::signal<void> m_on_hover_exit_signal;


        auto mf_on_mouse_motion(const sdl::event &event, sdl::renderer &render)
            -> sdl::event_return_type;


        void mf_render_fade(sdl::renderer &render);
    };
}

#include <algorithm>

#include "widgets/hoverable.hh"

using widget::hoverable;


hoverable::hoverable(sdl::frect rect) noexcept
    : base { rect }, mp_fadeout_time_ms { 500 }, mp_fadein_time_ms { 250 },
      m_fade_state { fade_state::none }, m_current_color { mp_color }
{
}


auto
hoverable::add_event_callbacks(sdl::event_handler &handler) noexcept
    -> hoverable &
{
    m_OMM_connection = handler.connect(SDL_EVENT_MOUSE_MOTION, this,
                                       &hoverable::mf_on_mouse_motion);
    return *this;
}


auto
hoverable::render(sdl::renderer &render) -> hoverable &
{
    if (!mp_visible) return *this;

    if (m_fade_state != fade_state::none)
    {
        mf_render_fade(render);
        return *this;
    }

    render.set_draw_color(mp_hovered ? mp_hover_color : mp_color);
    render.render_rect(mp_rect);
    return *this;
}


void
hoverable::mf_render_fade(sdl::renderer &render)
{
    if (m_fade_state == fade_state::none) return;

    std::uint64_t now { SDL_GetTicks() };
    auto delta_time { static_cast<float>(now - m_previous_frame_time) };

    m_fade_elapsed += delta_time;

    std::uint64_t duration { m_fade_state == fade_state::to_hover
                                 ? mp_fadein_time_ms
                                 : mp_fadeout_time_ms };

    auto t { std::min(m_fade_elapsed / duration, 1.0F) };

    m_current_color
        = sdl::color::lerp(m_fade_start_color, m_fade_target_color, t);

    render.set_draw_color(m_current_color);
    render.render_rect(mp_rect);

    m_previous_frame_time = now;
    if (t >= 1.0F) m_fade_state = fade_state::none;
}


auto
hoverable::mf_on_mouse_motion(const sdl::event &event,
                              sdl::renderer & /* render */)
    -> sdl::event_return_type
{
    bool new_hovered { sdl::is_point_in_rect(
        mp_rect, { event.motion.x, event.motion.y }) };

    if (new_hovered == mp_hovered) return sdl::event_return_type::CONTINUE;

    mp_hovered = new_hovered;

    m_fade_elapsed        = 0.0F;
    m_previous_frame_time = SDL_GetTicks();

    m_fade_start_color = m_current_color;

    if (mp_hovered)
    {
        m_fade_state        = fade_state::to_hover;
        m_fade_target_color = mp_hover_color;
    }
    else
    {
        m_fade_state        = fade_state::to_normal;
        m_fade_target_color = mp_color;
    }

    return sdl::event_return_type::CONTINUE;
}


auto
hoverable::set_hover_color(sdl::color color) noexcept -> hoverable &
{
    mp_hover_color = color;
    return *this;
}


auto
hoverable::set_fadeout_time(std::uint64_t ms) noexcept -> hoverable &
{
    mp_fadeout_time_ms = ms;
    return *this;
}


auto
hoverable::set_fadein_time(std::uint64_t ms) noexcept -> hoverable &
{
    mp_fadein_time_ms = ms;
    return *this;
}


auto
hoverable::get_hover_color() const noexcept -> sdl::color
{
    return mp_hover_color;
}


auto
hoverable::get_fadeout_time() const noexcept -> std::uint64_t
{
    return mp_fadeout_time_ms;
}


auto
hoverable::get_fadein_time() const noexcept -> std::uint64_t
{
    return mp_fadein_time_ms;
}


auto
hoverable::get_hovered() const noexcept -> bool
{
    return mp_hovered;
}

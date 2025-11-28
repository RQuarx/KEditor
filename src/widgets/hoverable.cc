#include <algorithm>

#include "widgets/hoverable.hh"

using widget::Hoverable;


namespace
{
    [[nodiscard]]
    auto
    get_bright_color(sdl::Color color, std::uint8_t brightness_factor)
        -> sdl::Color
    {
        color.r = std::clamp(color.r + brightness_factor, 0, 255);
        color.g = std::clamp(color.g + brightness_factor, 0, 255);
        color.b = std::clamp(color.b + brightness_factor, 0, 255);

        return color;
    }


    [[nodiscard]]
    auto
    is_in_bound(sdl::FRect area, sdl::FPoint position, int exclude_amount = 0)
        -> bool
    {
        if (exclude_amount * 2.0F >= area.w || exclude_amount * 2.0F >= area.h)
            return false;

        sdl::FRect actual_area { area.x + exclude_amount,
                                 area.y + exclude_amount,
                                 area.w - (exclude_amount * 2.0F),
                                 area.h - (exclude_amount * 2.0F) };

        float x2 { actual_area.x + actual_area.w };
        float y2 { actual_area.y + actual_area.h };

        return (position.x >= actual_area.x && position.x < x2
                && position.y >= actual_area.y && position.y < y2);
    }
}


Hoverable::Hoverable(sdl::FRect                area,
                     sdl::Color                color,
                     std::optional<sdl::Color> border_color)
    : Box(area, color, border_color),
      m_hovered_color(get_bright_color(get_color(), BRIGHTNESS_FACTOR))
{
}


auto
Hoverable::add_event_callbacks(sdl::EventHandler &handler,
                               sdl::Renderer & /* render */) -> bool
{
    handler.connect(SDL_EVENT_MOUSE_MOTION, this,
                    &Hoverable::mf_on_mouse_motion);
    return true;
}


auto
Hoverable::render(sdl::Renderer &render) -> bool
{
    if (!is_visible()) return true;
    if (!m_fading_out)
    {
        sdl::FRect area { get_area() };
        sdl::Color color { m_hovered ? m_hovered_color : get_color() };

        if (!get_border_color())
            return render.set_draw_color(color) && render.render_area(area);

        sdl::FRect inside_area { get_area(false) };

        return render.set_draw_color(*get_border_color())
            && render.render_area(area, false) && render.set_draw_color(color)
            && render.render_area(inside_area);
    }

    return mf_render_fade(render);
}


auto
Hoverable::mf_on_mouse_motion(const sdl::Event &event,
                              sdl::Renderer & /* render */)
    -> sdl::EventReturnType
{
    bool new_hovered { is_in_bound(get_area(false),
                                   { event.motion.x, event.motion.y }) };

    if (new_hovered != m_hovered)
    {
        m_hovered         = new_hovered;
        m_fade_elapsed    = 0.0F;
        m_fading_out      = !m_hovered;
        m_prev_frame_time = SDL_GetTicks();
    }

    return sdl::EventReturnType::CONTINUE;
}


auto
Hoverable::mf_render_fade(sdl::Renderer &render) -> bool
{
    if (!m_fading_out) return true;

    std::uint64_t now { SDL_GetTicks() };
    auto          delta_time { static_cast<float>(now - m_prev_frame_time) };

    m_fade_elapsed += delta_time;
    float t { std::min(m_fade_elapsed / FADEOUT_TIME_MS, 1.0F) };

    sdl::Color render_color { sdl::Color::lerp(m_hovered_color, get_color(),
                                               t) };

    sdl::FRect area { get_area() };
    if (!get_border_color())
    {
        render.set_draw_color(render_color);
        render.render_area(area);
    }
    else
    {
        sdl::FRect inside_area { get_area(false) };
        render.set_draw_color(*get_border_color());
        render.render_area(area, false);
        render.set_draw_color(render_color);
        render.render_area(inside_area);
    }

    m_prev_frame_time = SDL_GetTicks();

    if (t >= 1.0F) m_fading_out = false;

    return true;
}

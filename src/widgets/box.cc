#include "widgets/box.hh"

using widget::Box;


Box::Box(sdl::FRect                widget_area,
         sdl::Color                widget_color,
         std::optional<sdl::Color> border_color)
    : m_area(widget_area), m_color(widget_color), m_border_color(border_color),
      m_border_width_px(1), m_visible(true)
{
}


auto
Box::get_area(bool include_border) const noexcept -> sdl::FRect
{
    if (include_border) return m_area;
    return { m_area.x + m_border_width_px, m_area.y + m_border_width_px,
             m_area.w - (m_border_width_px * 2),
             m_area.h - (m_border_width_px * 2) };
}


auto
Box::get_color() const noexcept -> sdl::Color
{
    return m_color;
}


auto
Box::get_border_color() const noexcept -> std::optional<sdl::Color>
{
    return m_border_color;
}


void
Box::set_position(sdl::FPoint new_position)
{
    m_area.x = new_position.x;
    m_area.y = new_position.y;
}


void
Box::set_size(sdl::FSize new_size)
{
    m_area.w = new_size.w;
    m_area.h = new_size.h;
}


void
Box::set_color(sdl::Color new_color)
{
    m_color = new_color;
}


void
Box::set_border_color(sdl::Color new_color)
{
    m_border_color = new_color;
}


void
Box::set_border_width(unsigned int px)
{
    m_border_width_px = px;
}


auto
Box::get_border_width() const -> unsigned int
{
    return m_border_width_px;
}


[[nodiscard]]
auto
Box::is_visible() const noexcept -> bool
{
    return m_visible;
}


void
Box::set_visible(bool visible)
{
    m_visible = visible;
}


void
Box::render(sdl::Renderer &render)
{
    if (!m_visible) return;

    if (!m_border_color)
    {
        render.set_draw_color(m_color);
        render.render_rect(m_area);

        return;
    }

    sdl::FRect inside_rect { m_area.x + m_border_width_px,
                             m_area.y + m_border_width_px,
                             m_area.w - (m_border_width_px * 2),
                             m_area.h - (m_border_width_px * 2) };

    render.set_draw_color(*m_border_color);
    render.render_rect(m_area, false);
    render.set_draw_color(m_color);
    render.render_rect(inside_rect);
}

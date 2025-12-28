#include "widgets/frame.hh"

using widget::frame;


namespace
{
    [[nodiscard]]
    auto
    add_frect_by_value(sdl::frect rect, std::int32_t value) noexcept
        -> sdl::frect
    {
        rect.x -= value;
        rect.y -= value;
        rect.w += (value * 2);
        rect.h += (value * 2);
        return rect;
    }
}


frame::frame(sdl::frect rect) noexcept
    : base { rect }, box { rect }, mp_border_color { 0xffffff_rgb },
      mp_border_width { 1 }
{
}


auto
frame::render(sdl::renderer &render) -> frame &
{
    if (!mp_visible) return *this;

    render.set_draw_color(mp_border_color);

    auto border_rect { add_frect_by_value(mp_rect, mp_border_width) };
    render.render_rect(border_rect);

    box::render(render);
    return *this;
}


auto
frame::set_border_color(sdl::color color) noexcept -> frame &
{
    mp_border_color = color;
    return *this;
}


auto
frame::set_border_width(std::int32_t px) noexcept -> frame &
{
    mp_border_width = px;
    return *this;
}


auto
frame::get_border_color() const noexcept -> sdl::color
{
    return mp_border_color;
}


auto
frame::get_border_width() const noexcept -> std::int32_t
{
    return mp_border_width;
}

#include "widgets/box.hh"

using widget::box;


box::box(sdl::frect rect) noexcept :
    base { rect }
{
}


auto
box::render(sdl::renderer &render) -> box &
{
    if (!mp_visible) return *this;

    render.set_draw_color(mp_color);
    render.render_rect(mp_rect);
    return *this;
}

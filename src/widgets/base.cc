#include "widgets/base.hh"

using widget::base;


base::base(sdl::frect rect) noexcept : mp_rect { rect }, mp_visible { true } {}


auto
base::add_event_callbacks(sdl::event_handler & /* handler */) noexcept -> base &
{
    return *this;
}


auto
base::set_position(sdl::fpoint pos) noexcept -> base &
{
    mp_rect.x = pos.x;
    mp_rect.y = pos.y;
    return *this;
}


auto
base::set_visible(bool visible) noexcept -> base &
{
    mp_visible = visible;
    return *this;
}


auto
base::set_color(sdl::color color) noexcept -> base &
{
    mp_color = color;
    return *this;
}


auto
base::set_rect(sdl::frect rect) noexcept -> base &
{
    mp_rect = rect;
    return *this;
}


auto
base::set_size(sdl::fsize size) noexcept -> base &
{
    mp_rect.w = size.w;
    mp_rect.h = size.h;
    return *this;
}


auto
base::get_position() const noexcept -> sdl::fpoint
{
    return { mp_rect.x, mp_rect.y };
}


auto
base::get_visible() const noexcept -> bool
{
    return mp_visible;
}


auto
base::get_color() const noexcept -> sdl::color
{
    return mp_color;
}


auto
base::get_rect() const noexcept -> sdl::frect
{
    return mp_rect;
}


auto
base::get_size() const noexcept -> sdl::fsize
{
    return { .w = mp_rect.w, .h = mp_rect.h };
}

#include "widgets/clickable.hh"

using widget::clickable;


clickable::clickable(sdl::frect rect) noexcept
    : base { rect }, hoverable { rect }
{
}


auto
clickable::add_event_callbacks(sdl::event_handler &handler) noexcept
    -> clickable &
{
    hoverable::add_event_callbacks(handler);

    m_button_down_OMC = handler.connect(SDL_EVENT_MOUSE_BUTTON_DOWN, this,
                                        &clickable::mf_on_mouse_click);

    m_button_up_OMC = handler.connect(SDL_EVENT_MOUSE_BUTTON_UP, this,
                                      &clickable::mf_on_mouse_click);
    return *this;
}


auto
clickable::render(sdl::renderer &render) -> clickable &
{
    if (!mp_visible) return *this;

    if (!mp_clicked)
    {
        hoverable::render(render);
        return *this;
    }

    render.set_draw_color(mp_clicked_color);
    render.render_rect(mp_rect);
    return *this;
}


auto
clickable::signal_clicked() noexcept -> sdl::signal<void, click_type> &
{
    return m_signal_clicked;
}


auto
clickable::set_clicked_color(sdl::color color) noexcept -> clickable &
{
    mp_clicked_color = color;
    return *this;
}


auto
clickable::get_clicked_color() const noexcept -> sdl::color
{
    return mp_clicked_color;
}


auto
clickable::get_clicked() const noexcept -> bool
{
    return mp_clicked;
}


auto
clickable::mf_on_mouse_click(const sdl::event &event,
                             sdl::renderer & /* render */)
    -> sdl::event_return_type
{
    bool inside { sdl::is_point_in_rect(mp_rect,
                                        { event.button.x, event.button.y }) };

    if (inside && event.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        mp_clicked = true;
        m_signal_clicked.emit(click_type::down);
    }
    else if (mp_clicked && event.button.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        if (mp_clicked) m_signal_clicked.emit(click_type::up);

        mp_clicked = false;
    }

    return sdl::event_return_type::CONTINUE;
}

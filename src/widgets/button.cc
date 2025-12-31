#include <print>

#include "widgets/button.hh"

using widget::button;


button::button(sdl::frect                        rect,
               const std::shared_ptr<sdl::font> &font,
               std::string                       string)
    : frame { rect }, m_clickable { rect }, m_discard_click_ms { 1500 }
{
    if (font)
    {
        m_label = label {
            { rect.x, rect.y },
            font, std::move(string)
        };

        set_size(m_label->get_size());
        m_clickable.set_rect(mp_rect);
    }

    m_OC_connection
        = m_clickable.signal_clicked().connect(this, &button::mf_on_click);
}


auto
button::add_event_callbacks(sdl::event_handler &handler) noexcept -> button &
{
    m_clickable.add_event_callbacks(handler);
    return *this;
}


auto
button::render(sdl::renderer &render) -> frame &
{
    if (!mp_visible) return *this;

    frame::render(render);
    m_clickable.render(render);

    if (!m_label) return *this;

    if (m_clickable.get_hovered()) m_label->set_text_color(m_hover_text_color);
    if (m_clickable.get_clicked())
        m_label->set_text_color(m_clicked_text_color);
    m_label->render(render);

    return *this;
}


void
button::mf_on_click(clickable::click_type type)
{
    if (type == clickable::click_type::down)
    {
        m_down_button_clicked_ms = SDL_GetTicks();
        return;
    }

    std::uint64_t delta_time { SDL_GetTicks() - m_down_button_clicked_ms };

    if (delta_time > m_discard_click_ms) return;

    m_signal_clicked.emit();
}


auto
button::signal_clicked() noexcept -> sdl::signal<void> &
{
    return m_signal_clicked;
}


auto
button::set_font(const std::shared_ptr<sdl::font> &font) noexcept -> button &
{
    m_label->set_font(font);
    return *this;
}


auto
button::set_string(std::string string) noexcept -> button &
{
    m_label->set_string(std::move(string));
    return *this;
}


auto
button::set_text_color(sdl::color color) noexcept -> button &
{
    m_label->set_text_color(color);
    return *this;
}


auto
button::set_hover_text_color(sdl::color color) noexcept -> button &
{
    m_hover_text_color = color;
    return *this;
}


auto
button::set_clicked_text_color(sdl::color color) noexcept -> button &
{
    m_clicked_text_color = color;
    return *this;
}


auto
button::set_hover_color(sdl::color color) noexcept -> button &
{
    m_clickable.set_hover_color(color);
    return *this;
}


auto
button::set_clicked_color(sdl::color color) noexcept -> button &
{
    m_clickable.set_clicked_color(color);
    return *this;
}


auto
button::set_fadeout_time(std::uint64_t ms) noexcept -> button &
{
    m_clickable.set_fadeout_time(ms);
    return *this;
}


auto
button::set_fadein_time(std::uint64_t ms) noexcept -> button &
{
    m_clickable.set_fadein_time(ms);
    return *this;
}


auto
button::set_discard_click_time(std::uint64_t ms) noexcept -> button &
{
    m_discard_click_ms = ms;
    return *this;
}


auto
button::get_font() const noexcept -> std::shared_ptr<sdl::font>
{
    return m_label ? m_label->get_font() : nullptr;
}


auto
button::get_string() const noexcept -> std::string
{
    return m_label ? m_label->get_string() : "";
}


auto
button::get_text_color() const noexcept -> sdl::color
{
    return m_label ? m_label->get_text_color() : 0x000000_rgb;
}


auto
button::get_hover_text_color() const noexcept -> sdl::color
{
    return m_hover_text_color;
}


auto
button::get_clicked_text_color() const noexcept -> sdl::color
{
    return m_clicked_text_color;
}


auto
button::get_hover_color() const noexcept -> sdl::color
{
    return m_clickable.get_hover_color();
}


auto
button::get_clicked_color() const noexcept -> sdl::color
{
    return m_clickable.get_clicked_color();
}


auto
button::get_fadeout_time() const noexcept -> std::uint64_t
{
    return m_clickable.get_fadeout_time();
}


auto
button::get_fadein_time() const noexcept -> std::uint64_t
{
    return m_clickable.get_fadein_time();
}


auto
button::get_discard_click_time() const noexcept -> std::uint64_t
{
    return m_discard_click_ms;
}

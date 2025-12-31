#include <utility>

#include "widgets/label.hh"

using widget::label;


label::label(sdl::fpoint                       position,
              const std::shared_ptr<sdl::font> &font,
              std::string                       string)
    : base { { position.x, position.y, 0, 0 } },
      mp_text_color { 0xffffff_rgb }, mp_string { std::move(string) },
      mp_font { font }
{
    sdl::fsize string_size { mp_font->get_string_size(mp_string) };
    set_size(string_size);
}


auto
label::render(sdl::renderer &render) -> label &
{
    if (!mp_visible) return *this;

    if (mp_text.raw() == nullptr || mp_text.get_string() != mp_string)
    {
        auto engine { render.get_text_engine() };
        mp_text = sdl::text { render, *mp_font.get(), mp_string };

        mp_text.set_color(mp_text_color);
    }

    mp_text.set_color(mp_text_color);
    mp_text.render(get_position());
    return *this;
}


auto
label::set_font(const std::shared_ptr<sdl::font> &font) noexcept -> label &
{
    mp_font = font;
    return *this;
}


auto
label::set_text_color(sdl::color color) noexcept -> label &
{
    mp_text_color = color;
    return *this;
}


auto
label::set_string(std::string string) noexcept -> label &
{
    mp_string = std::move(string);
    return *this;
}


auto
label::get_font() const noexcept -> std::shared_ptr<sdl::font>
{
    return mp_font;
}


auto
label::get_text_color() const noexcept -> sdl::color
{
    return mp_color;
}


auto
label::get_string() const noexcept -> std::string
{
    return mp_string;
}

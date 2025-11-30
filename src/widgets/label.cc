#include <utility>

#include "widgets/label.hh"

using widget::Label;


Label::Label(sdl::FPoint                position,
             std::string                string,
             sdl::Color                 background_color,
             sdl::Color                 text_color,
             std::shared_ptr<sdl::Font> font)
    : Box{{ position.x, position.y, 0, 0 }, background_color},
      m_text_color{text_color}, m_string{std::move(string)},
      m_font{std::move(font)}
{
    int w { 0 };
    int h { 0 };

    if (!TTF_GetStringSize(m_font->raw(), m_string.c_str(), m_string.length(),
                           &w, &h))
        throw sdl::Exception("TTF_GetStringSize failed");

    m_string_size.w = static_cast<float>(w);
    m_string_size.h = static_cast<float>(h);

    set_size(m_string_size);
}


void
Label::render(sdl::Renderer &render)
{
    if (!is_visible()) return;

    if (m_text.raw() == nullptr)
    {
        auto engine { render.get_text_engine() };
        m_text = sdl::Text { render, *m_font.get(), m_string };

        m_text.set_color(m_text_color);
    }

    Box::render(render);
    m_text.render({ get_area().x, get_area().y });
}

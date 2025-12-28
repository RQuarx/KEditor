#include "sdl/instance.hh"
#include "sdl/text.hh"

using sdl::text;


text::text(renderer &render, font &font, std::string string)
{
    m_object = TTF_CreateText(render.get_text_engine()->raw(), font.raw(),
                              string.c_str(), string.length());

    if (m_object == nullptr) throw sdl::exception {};

    m_string = std::move(string);
}


void
text::set_color(color color)
{
    if (!TTF_SetTextColor(m_object, COLOR_TO_PARAM(color)))
        throw sdl::exception {};
}


void
text::render(fpoint position)
{
    if (!TTF_DrawRendererText(m_object, position.x, position.y))
        throw sdl::exception {};
}


auto
text::get_string() const -> std::string
{
    return m_string;
}


void
text::set_string(std::string string)
{
    if (!TTF_SetTextString(m_object, string.c_str(), string.length()))
        throw sdl::exception {};
    m_string = std::move(string);
}

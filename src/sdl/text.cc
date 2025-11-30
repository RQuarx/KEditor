#include "sdl/instance.hh"
#include "sdl/text.hh"

using sdl::Text;


Text::Text(Renderer &render, Font &font, std::string string)
{
    m_object = TTF_CreateText(render.get_text_engine()->raw(), font.raw(),
                                   string.c_str(), string.length());

    if (m_object == nullptr)
        throw sdl::Exception { "Text::Text(): {}", get_error() };

    m_string = std::move(string);
}


void
Text::set_color(Color color)
{
    if (!TTF_SetTextColor(m_object, COLOR_TO_PARAM(color)))
        throw sdl::Exception { "Text::set_color(): {}", get_error() };
}


void
Text::render(FPoint position)
{
    if (!TTF_DrawRendererText(m_object, position.x, position.y))
        throw sdl::Exception { "Text::render(): {}", get_error() };
}


auto
Text::get_string() const -> std::string
{
    return m_string;
}


void
Text::set_string(std::string string)
{
    if (!TTF_SetTextString(m_object, string.c_str(), string.length()))
        throw sdl::Exception { "Text::set_string(): {}", get_error() };
    m_string = std::move(string);
}

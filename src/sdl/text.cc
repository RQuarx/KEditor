#include "sdl/instance.hh"
#include "sdl/text.hh"

using sdl::Text;


auto
Text::create(Renderer &render, Font &font, std::string string)
    -> std::optional<Text>
{
    Text text;
    text.m_object = TTF_CreateText(render.get_text_engine()->raw(), font.raw(),
                                   string.c_str(), string.length());

    if (text.m_object == nullptr) return std::nullopt;

    text.m_string = std::move(string);
    return text;
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

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


auto
Text::set_color(Color color) -> bool
{
    return TTF_SetTextColor(m_object, COLOR_TO_PARAM(color));
}


auto
Text::render(FPoint position) -> bool
{
    return TTF_DrawRendererText(m_object, position.x, position.y);
}


auto
Text::get_string() const -> std::string
{
    return m_string;
}


auto
Text::set_string(std::string string) -> bool
{
    if (!TTF_SetTextString(m_object, string.c_str(), string.length()))
        return false;
    m_string = std::move(string);
    return true;
}
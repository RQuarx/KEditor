#include "sdl/text.hh"

using sdl::Text;


auto
Text::create(Renderer &render, Font &font, const std::string &string)
    -> std::optional<Text>
{
    Text text;
    text.m_object = TTF_CreateText(render.get_text_engine().raw(), font.raw(),
                                   string.c_str(), string.length());

    if (text.m_object == nullptr) return std::nullopt;
    return text;
}


auto
Text::set_color(Color color) -> bool
{
    return TTF_SetTextColorFloat(m_object, COLOR_TO_PARAM(color));
}


auto
Text::render(FPoint position) -> bool
{
    return TTF_DrawRendererText(m_object, position.x, position.y);
}

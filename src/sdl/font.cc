#include "sdl/font.hh"

using sdl::Font;


auto
Font::create(std::filesystem::path font_path, float ptsize)
    -> std::optional<Font>
{
    Font font;
    font.m_object = TTF_OpenFont(font_path.c_str(), ptsize);

    if (font.m_object == nullptr) return std::nullopt;
    font.m_font_path = std::move(font_path);
    return font;
}

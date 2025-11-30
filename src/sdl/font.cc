#include "exceptions.hh"
#include "sdl/font.hh"
#include "sdl/instance.hh"

using sdl::Font;


Font::Font(std::filesystem::path font_path, float ptsize)
{
    m_object = TTF_OpenFont(font_path.c_str(), ptsize);

    if (m_object == nullptr)
        throw sdl::Exception { "Font::Font(): {}", get_error() };

    m_font_path = std::move(font_path);

    TTF_SetFontHinting(m_object, TTF_HINTING_LIGHT_SUBPIXEL);
}

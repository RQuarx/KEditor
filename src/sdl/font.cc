#include "sdl/font.hh"
#include "sdl/instance.hh"

using sdl::font;


font::font(std::filesystem::path font_path, float ptsize)
{
    m_object = TTF_OpenFont(font_path.c_str(), ptsize);

    if (m_object == nullptr) throw sdl::exception {};

    m_font_path = std::move(font_path);

    TTF_SetFontHinting(m_object, TTF_HINTING_LIGHT_SUBPIXEL);
}


auto
font::get_string_size(std::string_view string) -> sdl::fsize
{
    int w { 0 };
    int h { 0 };

    if (!TTF_GetStringSize(m_object, string.data(), string.length(), &w, &h))
        throw sdl::exception {};

    return { .w = static_cast<float>(w), .h = static_cast<float>(h) };
}


auto
font::get_family_name() const -> std::string
{
    return TTF_GetFontFamilyName(m_object);
}


auto
font::get_ptsize() const -> float
{
    return TTF_GetFontSize(m_object);
}
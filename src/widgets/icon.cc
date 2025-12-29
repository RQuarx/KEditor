#include <SDL3_image/SDL_image.h>

#include "sdl/instance.hh"
#include "widgets/icon.hh"

using widget::icon;


icon::icon(sdl::frect rect, std::filesystem::path path_to_icon) :
    base { { rect.x, rect.y, 0, 0 } }
{
    set_icon_path(std::move(path_to_icon));

    if (rect.w > 0) mp_rect.w = rect.w;
    if (rect.h > 0) mp_rect.h = rect.h;
}


auto
icon::render(sdl::renderer &render) -> icon &
{
    if (!mp_visible) return *this;

    if (!m_icon_texture)
    {
        m_icon_texture = sdl::texture { SDL_CreateTextureFromSurface(
            render, m_icon_surface) };
        if (!m_icon_surface)
            throw sdl::exception { "SDL_CreateTextureFromSurface(): {}",
                                   sdl::get_error() };

        if (mp_rect.w <= 0 || mp_rect.w <= 0)
            SDL_GetTextureSize(m_icon_texture, &mp_rect.w, &mp_rect.h);
    }

    render.render_texture(m_icon_texture, mp_rect);
    return *this;
}


auto
icon::set_icon_path(std::filesystem::path path_to_icon) -> icon &
{
    mp_path_to_icon = std::move(path_to_icon);
    m_icon_surface  = sdl::surface { IMG_Load(mp_path_to_icon.c_str()) };
    if (!m_icon_surface)
        throw sdl::exception { "IMG_Load(): {}", sdl::get_error() };
    return *this;
}


auto
icon::get_icon_path() const noexcept -> std::filesystem::path
{
    return mp_path_to_icon;
}

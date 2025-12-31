#include <SDL3_image/SDL_image.h>

#include "sdl/instance.hh"
#include "widgets/icon.hh"

using widget::icon;


icon::cache_type icon::m_cache {};


/* clang-format off */
icon::icon(sdl::frect rect, std::filesystem::path path_to_icon)
    : base { { rect.x, rect.y, 0, 0 } }
{
    set_icon_path(std::move(path_to_icon));
    if (rect.w > 0) mp_rect.w = rect.w;
    if (rect.h > 0) mp_rect.h = rect.h;
}
/* clang-format on */


auto
icon::render(sdl::renderer &render) -> icon &
{
    if (!mp_visible) return *this;

    sdl::texture *text { nullptr };

    {
        std::scoped_lock lock { m_cache_mtx };
        if (auto it { m_cache.find(mp_path_to_icon) }; it != m_cache.end())
            text = &it->second;
    }

    if (text == nullptr)
    {
        sdl::surface surf { IMG_Load(mp_path_to_icon.c_str()) };
        if (!surf) throw sdl::exception { "IMG_Load(): {}", sdl::get_error() };

        sdl::texture new_tex { SDL_CreateTextureFromSurface(render, surf) };

        if (!new_tex)
            throw sdl::exception { "SDL_CreateTextureFromSurface(): {}",
                                   sdl::get_error() };

        {
            std::scoped_lock lock(m_cache_mtx);
            text = &m_cache.emplace(mp_path_to_icon, std::move(new_tex))
                        .first->second;
        }
    }

    render.render_texture(*text, mp_rect);
    return *this;
}


auto
icon::set_icon_path(std::filesystem::path path_to_icon) -> icon &
{
    mp_path_to_icon = std::move(path_to_icon);
    return *this;
}


auto
icon::get_icon_path() const noexcept -> std::filesystem::path
{
    return mp_path_to_icon;
}

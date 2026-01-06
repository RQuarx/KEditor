#include "sdl/instance.hh"
#include "sdl/renderer.hh"

using sdl::renderer;
using sdl::text_engine;


text_engine::text_engine(renderer &render)
{
    m_object = TTF_CreateRendererTextEngine(render);
    if (m_object == nullptr) throw sdl::exception {};
}


renderer::renderer(window &&window, const std::string &device)
{
    m_object
        = SDL_CreateRenderer(window, device.empty() ? nullptr : device.c_str());

    if (m_object == nullptr)
        throw sdl::exception { "renderer::renderer(): {}", get_error() };

    if (!SDL_SetRenderDrawBlendMode(m_object, SDL_BLENDMODE_BLEND))
        throw sdl::exception { "renderer::renderer(): {}", get_error() };

    if (!SDL_SetRenderVSync(m_object, 1))
        throw sdl::exception { "renderer::renderer(): {}", get_error() };

    m_engine = std::make_shared<text_engine>(*this);
    m_window = std::move(window);
}


auto
renderer::get_window() -> sdl::window &
{
    return m_window;
}


void
renderer::set_draw_color(color color)
{
    if (!SDL_SetRenderDrawColorFloat(m_object,
                                     COLOR_TO_PARAM(color.to_fcolor())))
        throw sdl::exception { "renderer::set_draw_color(): {}", get_error() };
}


void
renderer::set_clip_rect(const frect &rect)
{
    SDL_Rect r { static_cast<int>(rect.x), static_cast<int>(rect.y),
                 static_cast<int>(rect.w), static_cast<int>(rect.h) };
    if (!SDL_SetRenderClipRect(m_object, &r)) throw sdl::exception {};
}


void
renderer::clear_clip_rect()
{
    if (!SDL_SetRenderClipRect(m_object, nullptr)) throw sdl::exception {};
}


void
renderer::clear()
{
    if (!SDL_RenderClear(m_object))
        throw sdl::exception { "renderer::clear(): {}", get_error() };
}


void
renderer::present()
{
    if (!SDL_RenderPresent(m_object))
        throw sdl::exception { "renderer::present(): {}", get_error() };
}


void
renderer::render_rect(sdl::frect &area, bool fill)
{
    if (fill)
    {
        if (SDL_RenderFillRect(m_object, &area)) return;
        throw sdl::exception { "renderer::render_rect(): {}", get_error() };
    }

    if (!SDL_RenderRect(m_object, &area))
        throw sdl::exception { "renderer::render_rect(): {}", get_error() };
}


void
renderer::render_texture(texture &texture, sdl::frect &dst)
{
    if (!SDL_RenderTexture(m_object, texture, nullptr, &dst))
        throw sdl::exception { "renderer::render_texture(): {}", get_error() };
}


auto
renderer::get_text_engine() const -> std::shared_ptr<text_engine>
{
    return m_engine;
}

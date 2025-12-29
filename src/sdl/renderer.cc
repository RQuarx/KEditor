#include "sdl/instance.hh"
#include "sdl/renderer.hh"

using sdl::renderer;
using sdl::text_engine;


text_engine::text_engine(renderer &render)
{
    m_object = TTF_CreateRendererTextEngine(render.raw());
    if (m_object == nullptr) throw sdl::exception {};
}


renderer::renderer(window &&window, const std::string &device)
{
    m_object = SDL_CreateRenderer(window.raw(),
                                  device.empty() ? nullptr : device.c_str());

    if (m_object == nullptr) throw sdl::exception {};

    if (!SDL_SetRenderDrawBlendMode(m_object, SDL_BLENDMODE_BLEND))
        throw sdl::exception {};

    if (!SDL_SetRenderVSync(m_object, 1)) throw sdl::exception {};

    m_engine = std::make_shared<text_engine>(*this);
    m_window = std::move(window);
}


auto
renderer::get_render_nexts() -> signal<render_return_type, sdl::renderer &> &
{
    return m_render_nexts;
}


auto
renderer::get_window() -> sdl::window &
{
    return m_window;
}


auto
renderer::run_render_queue() -> render_return_type
{
    for (auto &slot : m_render_nexts.get_slots())
    {
        auto res { slot.slot(*this) };

        if (res == render_return_type::FAILURE) return render_return_type::FAILURE;
    }

    return render_return_type::SUCCESS;
}


void
renderer::set_draw_color(color color)
{
    if (!SDL_SetRenderDrawColorFloat(m_object,
                                     COLOR_TO_PARAM(color.to_fcolor())))
        throw sdl::exception { "renderer::set_draw_color(): {}", get_error() };
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

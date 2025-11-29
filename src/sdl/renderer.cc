#include "sdl/instance.hh"
#include "sdl/renderer.hh"

using sdl::Renderer;
using sdl::TextEngine;


auto
TextEngine::create(Renderer &render) -> std::optional<TextEngine>
{
    TextEngine engine;

    engine.m_object = TTF_CreateRendererTextEngine(render.raw());
    if (engine.m_object == nullptr) return std::nullopt;
    return engine;
}


auto
Renderer::create(Window &&window, const std::string &device)
    -> std::optional<Renderer>
{
    Renderer render;

    render.m_object = SDL_CreateRenderer(
        window.raw(), device.empty() ? nullptr : device.c_str());

    if (render.m_object == nullptr) return std::nullopt;

    if (!SDL_SetRenderDrawBlendMode(render.m_object, SDL_BLENDMODE_BLEND))
        return std::nullopt;
    if (!SDL_SetRenderVSync(render.m_object, 1)) return std::nullopt;

    auto engine { TextEngine::create(render) };

    /* renderer will destroy itself */
    if (!engine) return std::nullopt;

    render.m_engine = std::make_shared<TextEngine>(std::move(*engine));
    render.m_window = std::move(window);
    return render;
}


auto
Renderer::get_render_nexts() -> Signal<RenderReturnType, sdl::Renderer &> &
{
    return m_render_nexts;
}


auto
Renderer::get_window() -> sdl::Window &
{
    return m_window;
}


auto
Renderer::run_render_queue() -> RenderReturnType
{
    for (auto &slot : m_render_nexts.get_slots())
    {
        auto res { slot.slot(*this) };

        if (res == RenderReturnType::FAILURE) return RenderReturnType::FAILURE;
    }

    return RenderReturnType::SUCCESS;
}


void
Renderer::set_draw_color(Color color)
{
    if (!SDL_SetRenderDrawColorFloat(m_object,
                                     COLOR_TO_PARAM(color.to_fcolor())))
        throw sdl::Exception { "Renderer::set_draw_color(): {}", get_error() };
}


void
Renderer::clear()
{
    if (!SDL_RenderClear(m_object))
        throw sdl::Exception { "Renderer::clear(): {}", get_error() };
}


void
Renderer::present()
{
    if (!SDL_RenderPresent(m_object))
        throw sdl::Exception { "Renderer::present(): {}", get_error() };
}


void
Renderer::render_rect(sdl::FRect &area, bool fill)
{
    if (fill)
    {
        if (SDL_RenderFillRect(m_object, &area)) return;
        throw sdl::Exception { "Renderer::render_rect(): {}", get_error() };
    }

    if (!SDL_RenderRect(m_object, &area))
        throw sdl::Exception { "Renderer::render_rect(): {}", get_error() };
}


auto
Renderer::get_text_engine() const -> std::shared_ptr<TextEngine>
{
    return m_engine;
}

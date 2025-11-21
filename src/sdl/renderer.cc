#include "log.hh"
#include "sdl/renderer.hh"

using sdl::Renderer;
using sdl::TextEngine;
using enum LogLevel;


auto
TextEngine::create(Renderer &render) -> std::optional<TextEngine>
{
    TextEngine engine;

    engine.m_object = TTF_CreateRendererTextEngine(render.raw());
    if (engine.m_object == nullptr) return std::nullopt;
    return engine;
}


auto
Renderer::create(Window &window, const std::string &device)
    -> std::optional<Renderer>
{
    Renderer render;

    render.m_object = SDL_CreateRenderer(
        window.raw(), device.empty() ? nullptr : device.c_str());

    if (render.m_object == nullptr) return std::nullopt;

    auto engine { TextEngine::create(render) };

    /* renderer will destroy itself */
    if (!engine) return std::nullopt;

    render.m_engine = std::move(*engine);
    return render;
}


auto
Renderer::set_draw_color(Color color) -> bool
{
    return SDL_SetRenderDrawColorFloat(m_object,
                                       COLOR_TO_PARAM(color.to_fcolor()));
}


auto
Renderer::clear() -> bool
{
    return SDL_RenderClear(m_object);
}


auto
Renderer::present() -> bool
{
    return SDL_RenderPresent(m_object);
}


auto
Renderer::render_area(sdl::FRect &area, bool fill) -> bool
{
    if (fill)
        return SDL_RenderFillRect(m_object, &area);
    return SDL_RenderRect(m_object, &area);
}


auto
Renderer::get_text_engine() const -> const TextEngine &
{
    return m_engine;
}

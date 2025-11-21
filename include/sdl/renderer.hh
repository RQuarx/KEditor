#ifndef _KEDITOR_SDL_RENDERER_HH
#define _KEDITOR_SDL_RENDERER_HH
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_textengine.h>

#include "sdl/color.hh"
#include "sdl/rect.hh"
#include "sdl/resource.hh"
#include "sdl/window.hh"


namespace sdl
{
    class Renderer;


    class TextEngine
        : public Resource<TTF_TextEngine, TTF_DestroyRendererTextEngine>
    {
    public:
        using Resource::Resource;


        [[nodiscard]]
        static auto create(Renderer &render) -> std::optional<TextEngine>;
    };


    class Renderer : public Resource<SDL_Renderer, SDL_DestroyRenderer>
    {
    public:
        using Resource::Resource;


        [[nodiscard]]
        static auto create(Window &window, const std::string &device)
            -> std::optional<Renderer>;


        auto set_draw_color(Color color) -> bool;
        auto set_text_color(Color color) -> bool;


        auto clear() -> bool;


        auto present() -> bool;


        auto render_area(sdl::FRect &area, bool fill = true) -> bool;


        [[nodiscard]]
        auto get_text_engine() const -> const TextEngine &;

    private:
        TextEngine m_engine;
    };
}

#endif /* _KEDITOR_SDL_RENDERER_HH */

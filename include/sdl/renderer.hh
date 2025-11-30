#ifndef _KEDITOR_SDL_RENDERER_HH
#define _KEDITOR_SDL_RENDERER_HH
#include <memory>

#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_textengine.h>

#include "sdl/color.hh"
#include "sdl/rect.hh"
#include "sdl/resource.hh"
#include "sdl/signal.hh"
#include "sdl/window.hh"


namespace sdl
{
    enum class RenderReturnType : std::uint8_t
    {
        SUCCESS,
        FAILURE,
        RENDER_NEXT_FRAME
    };


    class Renderer;


    class TextEngine
        : public Resource<TTF_TextEngine, TTF_DestroyRendererTextEngine>
    {
    public:
        using Resource::Resource;


        [[nodiscard]]
        TextEngine(Renderer &render);
    };


    class Renderer : public Resource<SDL_Renderer, SDL_DestroyRenderer>
    {
    public:
        using SignalSignature = Signal<RenderReturnType, Renderer &>;
        using Resource::Resource;


        Renderer(Window &&window, const std::string &device);


        [[nodiscard]]
        auto get_render_nexts() -> Signal<RenderReturnType, sdl::Renderer &> &;


        [[nodiscard]]
        auto get_window() -> sdl::Window &;


        auto run_render_queue() -> RenderReturnType;


        void set_draw_color(Color color);
        void set_text_color(Color color);


        void clear();


        void present();


        void render_rect(sdl::FRect &area, bool fill = true);


        [[nodiscard]]
        auto get_text_engine() const -> std::shared_ptr<TextEngine>;

    private:
        sdl::Window m_window;

        std::shared_ptr<TextEngine> m_engine;

        Signal<RenderReturnType, Renderer &> m_render_nexts;
    };
}

#endif /* _KEDITOR_SDL_RENDERER_HH */

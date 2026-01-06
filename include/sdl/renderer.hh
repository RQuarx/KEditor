#pragma once
#include <memory>

#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_textengine.h>

#include "sdl/color.hh"
#include "sdl/rect.hh"
#include "sdl/resource.hh"
#include "sdl/window.hh"


namespace sdl
{
    using surface = resource<SDL_Surface, SDL_DestroySurface>;
    using texture = resource<SDL_Texture, SDL_DestroyTexture>;


    class renderer;


    class text_engine
        : public resource<TTF_TextEngine, TTF_DestroyRendererTextEngine>
    {
    public:
        using resource::resource;


        [[nodiscard]]
        text_engine(renderer &render);
    };


    class renderer : public resource<SDL_Renderer, SDL_DestroyRenderer>
    {
    public:
        using resource::resource;


        renderer(window &&window, const std::string &device);


        [[nodiscard]]
        auto get_window() -> sdl::window &;


        void set_draw_color(color color);
        void set_text_color(color color);

        void set_clip_rect(const frect &rect);
        void clear_clip_rect();


        void clear();


        void present();


        void render_rect(sdl::frect &area, bool fill = true);


        void render_texture(texture &texture, sdl::frect &dst);


        [[nodiscard]]
        auto get_text_engine() const -> std::shared_ptr<text_engine>;

    private:
        sdl::window m_window;

        std::shared_ptr<text_engine> m_engine;
    };
}

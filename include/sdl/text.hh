#pragma once
#include <SDL3_ttf/SDL_ttf.h>

#include "sdl/font.hh"
#include "sdl/rect.hh"
#include "sdl/renderer.hh"
#include "sdl/resource.hh"


namespace sdl
{
    class text : public resource<TTF_Text, TTF_DestroyText>
    {
    public:
        using resource::resource;


        text(renderer &render, font &font, std::string string);


        void set_color(color color);


        void render(fpoint position);


        [[nodiscard]]
        auto get_string() const -> std::string;
        void set_string(std::string string);

    private:
        std::string m_string;
    };
}

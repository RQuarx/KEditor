#ifndef _KEDITOR_SDL_TEXT_HH
#define _KEDITOR_SDL_TEXT_HH
#include <SDL3_ttf/SDL_ttf.h>

#include "sdl/font.hh"
#include "sdl/rect.hh"
#include "sdl/renderer.hh"
#include "sdl/resource.hh"


namespace sdl
{
    class Text : public Resource<TTF_Text, TTF_DestroyText>
    {
    public:
        using Resource::Resource;


        Text(Renderer &render, Font &font, std::string string);


        void set_color(Color color);


        void render(FPoint position);


        [[nodiscard]]
        auto get_string() const -> std::string;
        void set_string(std::string string);

    private:
        std::string m_string;
    };
}

#endif /* _KEDITOR_SDL_TEXT_HH */

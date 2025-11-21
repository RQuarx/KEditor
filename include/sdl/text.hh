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


        [[nodiscard]]
        static auto create(Renderer          &render,
                           Font              &font,
                           const std::string &string) -> std::optional<Text>;


        auto set_color(Color color) -> bool;


        auto render(FPoint position) -> bool;
    };
}

#endif /* _KEDITOR_SDL_TEXT_HH */

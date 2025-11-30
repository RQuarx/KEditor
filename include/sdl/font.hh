#ifndef _KEDITOR_SDL_FONT_HH
#define _KEDITOR_SDL_FONT_HH
#include <filesystem>

#include <SDL3_ttf/SDL_ttf.h>

#include "sdl/resource.hh"


namespace sdl
{
    class Font : public Resource<TTF_Font, TTF_CloseFont>
    {
    public:
        using Resource::Resource;

        Font(std::filesystem::path font_path, float ptsize = 12.0F);


    private:
        std::filesystem::path m_font_path;
    };
}

#endif /* _KEDITOR_SDL_FONT_HH */

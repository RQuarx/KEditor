#pragma once
#include <filesystem>

#include <SDL3_ttf/SDL_ttf.h>

#include "sdl/rect.hh"
#include "sdl/resource.hh"


namespace sdl
{
    class font : public resource<TTF_Font, TTF_CloseFont>
    {
    public:
        using resource::resource;

        font(std::filesystem::path font_path, float ptsize = 12.0F);


        [[nodiscard]]
        auto get_string_size(std::string_view string) -> sdl::fsize;


        [[nodiscard]] auto get_family_name() const -> std::string;
        [[nodiscard]] auto get_ptsize() const -> float;


    private:
        std::filesystem::path m_font_path;
    };
}

#pragma once
#include <filesystem>

#include "widgets/base.hh"


namespace widget
{
    class icon : public virtual base
    {
    public:
        icon(sdl::frect rect, std::filesystem::path path_to_icon);


        auto render(sdl::renderer &render) -> icon & override;


        auto set_icon_path(std::filesystem::path path_to_icon) -> icon &;
        [[nodiscard]] auto get_icon_path() const noexcept
            -> std::filesystem::path;

    protected:
        std::filesystem::path mp_path_to_icon;

    private:
        sdl::texture m_icon_texture;
        sdl::surface m_icon_surface;
    };
}

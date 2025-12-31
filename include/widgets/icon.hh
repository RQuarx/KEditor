#pragma once
#include <filesystem>

#include "widgets/base.hh"


namespace widget
{
    class icon : public base
    {
        using cache_type
            = std::unordered_map<std::filesystem::path, sdl::texture>;

    public:
        icon(sdl::frect rect, std::filesystem::path path_to_icon);


        auto render(sdl::renderer &render) -> icon & override;


        auto set_icon_path(std::filesystem::path path_to_icon) -> icon &;

        [[nodiscard]]
        auto get_icon_path() const noexcept -> std::filesystem::path;

    protected:
        std::filesystem::path mp_path_to_icon;

    private:
        static cache_type m_cache;
        std::mutex        m_cache_mtx;
    };
}

#pragma once
#include "config.hh"
#include "logger.hh"
#include "model/piece_table.hh"
#include "sdl/renderer.hh"
#include "sdl/texture.hh"
#include "widgets/box.hh"


/**
 * @brief A file buffer/editor class.
 */
class buffer : sdl::renderable<bool>
{
public:
    buffer(kei::config &config, kei::logger &logger, model::piece_table pt);


    auto open_file(std::filesystem::path path) -> bool;


    auto set_rect(sdl::frect rect) -> bool;

    [[nodiscard]]
    auto get_rect() const noexcept -> widget::box *;


    auto add_event_callbacks(sdl::event_handler &handler) -> bool override;
    auto render(sdl::renderer &render) -> bool override;

private:
    kei::config &m_config;
    kei::logger &m_logger;

    model::piece_table m_buffer { "" };

    std::filesystem::path m_file_path;

    bool m_focused;
    int  m_cursor_pos;

    std::unique_ptr<widget::box> m_box;

    std::vector<sdl::texture> m_texture_cache;


    buffer();


    auto mf_on_mouse_button_down(const sdl::event &event, sdl::renderer &render)
        -> sdl::event_return_type;
};

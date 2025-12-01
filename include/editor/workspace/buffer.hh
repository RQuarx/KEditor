#ifndef _KEDITOR_EDITOR_WORKSPACE_BUFFER_HH
#define _KEDITOR_EDITOR_WORKSPACE_BUFFER_HH
#include "config.hh"
#include "logger.hh"
#include "model/piece_table.hh"
#include "sdl/renderer.hh"
#include "sdl/texture.hh"
#include "widgets/box.hh"


/**
 * @brief A file buffer/editor class.
 */
class Buffer
{
public:
    Buffer(Config          *&config,
           Logger          *&logger,
           model::PieceTable pt);


    auto open_file(std::filesystem::path path) -> bool;


    auto set_rect(sdl::FRect rect) -> bool;

    [[nodiscard]]
    auto get_rect() const noexcept -> widget::Box *;


    auto add_event_callbacks(sdl::EventHandler &handler) -> bool;
    auto render(sdl::Renderer &render) -> bool;

private:
    Config *m_config;
    Logger *m_logger;

    model::PieceTable m_buffer { "" };

    std::filesystem::path m_file_path;

    bool m_focused;
    int  m_cursor_pos;

    std::unique_ptr<widget::Box> m_box;

    std::vector<sdl::Texture> m_texture_cache;


    Buffer();


    auto mf_on_mouse_button_down(const sdl::Event &event, sdl::Renderer &render)
        -> sdl::EventReturnType;
};

#endif /* _KEDITOR_EDITOR_WORKSPACE_BUFFER_HH */

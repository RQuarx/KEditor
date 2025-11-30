#ifndef _KEDITOR_EDITOR_EDITOR_HH
#define _KEDITOR_EDITOR_EDITOR_HH
#include "config.hh"
#include "logger.hh"
#include "model/piece_table.hh"
#include "sdl/renderer.hh"
#include "sdl/texture.hh"
#include "widgets/box.hh"


class Editor
{
public:
    [[nodiscard]]
    static auto create(Config          *&config,
                       Logger          *&logger,
                       model::PieceTable pt,
                       sdl::Renderer   *&render) -> std::optional<Editor>;


    auto open_file(std::string path) -> bool;


    auto set_rect(sdl::FRect rect) -> bool;


    auto add_event_callbacks(sdl::EventHandler &handler) -> bool;
    auto render() -> bool;

private:
    Config *m_config;
    Logger *m_logger;

    model::PieceTable m_buffer { "" };
    sdl::Renderer    *m_render { nullptr };

    std::string m_file_path;

    bool m_focused;
    int  m_cursor_pos;

    std::unique_ptr<widget::Box> m_box;

    std::vector<sdl::Texture> m_texture_cache;


    Editor();


    auto mf_on_mouse_button_down(const sdl::Event &event, sdl::Renderer &render)
        -> sdl::EventReturnType;
};

#endif /* _KEDITOR_EDITOR_EDITOR_HH */

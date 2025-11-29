#ifndef _KEDITOR_EDITOR_EDITOR_HH
#define _KEDITOR_EDITOR_EDITOR_HH
#include "logger.hh"
#include "model/piece_table.hh"
#include "sdl/renderer.hh"
#include "sdl/texture.hh"


class Editor
{
public:
    [[nodiscard]]
    static auto create(Logger          *&logger,
                       model::PieceTable pt,
                       sdl::Renderer    *render) -> std::optional<Editor>;


    void open_file(std::string path);

private:
    Logger           *m_logger;
    model::PieceTable m_buffer { "" };
    sdl::Renderer    *m_render { nullptr };

    std::string m_file_path;

    bool m_focussed;
    int  m_cursor_pos;

    std::vector<sdl::Texture> m_texture_cache;


    Editor() = default;
};

#endif /* _KEDITOR_EDITOR_EDITOR_HH */

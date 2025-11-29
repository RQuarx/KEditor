#include "editor/editor.hh"

using enum LogLevel;


auto
Editor::create(Logger *&logger, model::PieceTable pt, sdl::Renderer *render)
    -> std::optional<Editor>
{
    if (render == nullptr) return std::nullopt;

    Editor editor;

    editor.m_logger = logger;
    editor.m_buffer = std::move(pt);
    editor.m_render = render;

    return editor;
}


void
Editor::open_file(std::string path)
{
    m_logger->log<INFO>("Opening a new file {}", path);

    m_file_path = std::move(path);

    m_texture_cache.clear();
    m_buffer = model::PieceTable::from_file(path);
}

#include "editor/editor.hh"


auto
Editor::create(model::PieceTable pt, sdl::TextEngine *engine)
    -> std::optional<Editor>
{
    if (engine == nullptr) return std::nullopt;

    Editor editor;

    editor.m_buffer = std::move(pt);
    editor.m_engine = engine;

    return editor;
}

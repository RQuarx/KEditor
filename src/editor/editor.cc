#include "editor/editor.hh"


auto
Editor::create(model::PieceTable pt, sdl::Renderer *render)
    -> std::optional<Editor>
{
    if (render == nullptr) return std::nullopt;

    Editor editor;

    editor.m_buffer = std::move(pt);
    editor.m_render = render;

    return editor;
}




#ifndef _KEDITOR_EDITOR_EDITOR_HH
#define _KEDITOR_EDITOR_EDITOR_HH
#include "model/piece_table.hh"
#include "sdl/renderer.hh"


class Editor
{
public:
    [[nodiscard]]
    static auto create(model::PieceTable pt, sdl::Renderer *render)
        -> std::optional<Editor>;

private:
    model::PieceTable m_buffer { "" };
    sdl::Renderer    *m_render { nullptr };


    Editor() = default;
};

#endif /* _KEDITOR_EDITOR_EDITOR_HH */

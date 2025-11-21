#ifndef _KEDITOR_EDITOR_EDITOR_HH
#define _KEDITOR_EDITOR_EDITOR_HH
#include "model/piece_table.hh"
#include "sdl/text_engine.hh"


class Editor
{
public:
    [[nodiscard]]
    static auto create(model::PieceTable pt, sdl::TextEngine *engine)
        -> std::optional<Editor>;

private:
    model::PieceTable m_buffer;
    sdl::TextEngine  *m_engine;


    Editor();
};

#endif /* _KEDITOR_EDITOR_EDITOR_HH */

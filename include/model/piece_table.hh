#ifndef _KEDITOR_MODEL_PIECE_TABLE_HH
#define _KEDITOR_MODEL_PIECE_TABLE_HH
#include <cstdint>
#include <string>
#include <vector>


namespace model
{
    enum class BufferType : std::uint8_t
    {
        ADD,
        ORIGINAL
    };


    struct Piece
    {
        BufferType type;

        std::size_t offset;
        std::size_t length;
    };


    struct EditNode
    {
        enum class Type : std::uint8_t
        {
            INSERT,
            DELETE
        } type;

        std::size_t position;
        std::string text;

        EditNode *parent { nullptr };

        std::vector<EditNode> children;
    };


    class PieceTable
    {
    public:
        PieceTable(std::string text);

        void insert(std::size_t position, std::string text);
        void erase(std::size_t position, std::size_t length);

        void undo();
        void redo(std::size_t branch_index);

        [[nodiscard]]
        auto get_text(std::size_t position,
                      std::size_t length = std::string::npos) const
            -> std::string;

    private:
        std::string m_original;
        std::string m_add;

        std::vector<Piece> m_pieces;

        /* undo tree shit */
        std::vector<EditNode> m_root_edits;
        EditNode             *m_current_edit { nullptr };


        void mf_do_insert(std::size_t position, const std::string &text);
        void mf_do_erase(std::size_t position, std::size_t length);

        [[nodiscard]]
        auto mf_get_total_length() const -> std::size_t;
    };
}

#endif /* _KEDITOR_MODEL_PIECE_TABLE_HH */

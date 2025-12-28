#pragma once
#include <cstdint>
#include <string>
#include <vector>


namespace model
{
    enum class buffer_type : std::uint8_t
    {
        ADD,
        ORIGINAL
    };


    struct Piece
    {
        buffer_type type;

        std::size_t offset;
        std::size_t length;
    };


    struct edit_node
    {
        enum class Type : std::uint8_t
        {
            INSERT,
            DELETE
        } type;

        std::size_t position;
        std::string text;

        edit_node *parent { nullptr };

        std::vector<edit_node> children;
    };


    class piece_table
    {
    public:
        [[nodiscard]]
        static auto from_file(const std::string &path) -> piece_table;


        piece_table(std::string text);

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
        std::vector<edit_node> m_root_edits;
        edit_node             *m_current_edit { nullptr };


        void mf_do_insert(std::size_t position, const std::string &text);
        void mf_do_erase(std::size_t position, std::size_t length);

        [[nodiscard]]
        auto mf_get_total_length() const -> std::size_t;
    };
}

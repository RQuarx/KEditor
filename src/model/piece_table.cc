#include <fstream>
#include <sstream>

#include "exceptions.hh"
#include "model/piece_table.hh"

using model::PieceTable;


auto
PieceTable::from_file(const std::string &path) -> PieceTable
{
    std::ifstream file { path };
    if (!file) throw kei::FilesystemError { "Failed to open file {}", path };

    std::ostringstream oss;
    oss << file.rdbuf();

    return PieceTable { oss.str() };
}


PieceTable::PieceTable(std::string text) : m_original(std::move(text))
{
    m_pieces.emplace_back(BufferType::ORIGINAL, 0, m_original.length());
}


void
PieceTable::insert(std::size_t position, std::string text)
{
    EditNode node;

    node.type     = EditNode::Type::INSERT;
    node.position = position;
    node.text     = std::move(text);
    node.parent   = m_current_edit;

    if (m_current_edit != nullptr)
    {
        m_current_edit->children.emplace_back(std::move(node));
        m_current_edit = &m_current_edit->children.back();
    }
    else
    {
        m_root_edits.emplace_back(std::move(node));
        m_current_edit = &m_root_edits.back();
    }

    mf_do_insert(position, m_current_edit->text);
}


void
PieceTable::erase(std::size_t position, std::size_t length)
{
    std::string deleted_text { get_text(position, length) };

    EditNode node;

    node.type     = EditNode::Type::DELETE;
    node.position = position;
    node.text     = std::move(deleted_text);
    node.parent   = m_current_edit;

    if (m_current_edit != nullptr)
    {
        m_current_edit->children.emplace_back(std::move(node));
        m_current_edit = &m_current_edit->children.back();
    }
    else
    {
        m_root_edits.emplace_back(std::move(node));
        m_current_edit = &m_root_edits.back();
    }

    mf_do_erase(position, length);
}


void
PieceTable::undo()
{
    if (m_current_edit == nullptr) return;

    if (m_current_edit->type == EditNode::Type::INSERT)
        mf_do_erase(m_current_edit->position, m_current_edit->text.length());
    else
        mf_do_insert(m_current_edit->position, m_current_edit->text);

    m_current_edit = m_current_edit->parent;
}


void
PieceTable::redo(std::size_t branch_index)
{
    if (m_current_edit == nullptr)
    {
        if (branch_index < m_root_edits.size())
        {
            m_current_edit = &m_root_edits[branch_index];

            if (m_current_edit->type == EditNode::Type::INSERT)
                mf_do_insert(m_current_edit->position, m_current_edit->text);
            else
                mf_do_erase(m_current_edit->position,
                            m_current_edit->text.length());
        }
    }
    else if (branch_index < m_current_edit->children.size())
    {
        m_current_edit = &m_current_edit->children[branch_index];

        if (m_current_edit->type == EditNode::Type::INSERT)
            mf_do_insert(m_current_edit->position, m_current_edit->text);
        else
            mf_do_erase(m_current_edit->position,
                        m_current_edit->text.length());
    }
}


auto
PieceTable::get_text(std::size_t position, std::size_t length) const
    -> std::string
{
    std::size_t total_length { mf_get_total_length() };

    if (position >= total_length) return "";
    if (length == std::string::npos)
        length = total_length - position;
    else
        length = std::min(length, total_length - position);

    std::string result;
    std::size_t current_pos { 0 };

    for (const auto &piece : m_pieces)
    {
        if (current_pos + piece.length <= position)
        {
            current_pos += piece.length;
            continue;
        }

        std::size_t start_in_piece { position > current_pos
                                         ? position - current_pos
                                         : 0 };
        std::size_t len_in_piece { std::min(piece.length - start_in_piece,
                                            length - result.length()) };

        const std::string &buffer { (piece.type == BufferType::ORIGINAL)
                                        ? m_original
                                        : m_add };

        result += buffer.substr(piece.offset + start_in_piece, len_in_piece);

        if (result.length() >= length) break;
        current_pos += piece.length;
    }

    return result;
}


void
PieceTable::mf_do_insert(std::size_t position, const std::string &text)
{
    std::size_t add_offset { m_add.size() };
    std::size_t current_pos { 0 };
    m_add += text;

    for (auto it { m_pieces.begin() }; it != m_pieces.end(); it++)
    {
        if (current_pos + it->length <= position)
        {
            current_pos += it->length;
            continue;
        }

        std::size_t offset_in_piece { position - current_pos };
        if (offset_in_piece == 0)
        {
            m_pieces.insert(it, Piece { .type   = BufferType::ADD,
                                        .offset = add_offset,
                                        .length = text.length() });
            return;
        }

        if (offset_in_piece < it->length)
        {
            Piece after { *it };

            after.offset += offset_in_piece;
            after.length -= offset_in_piece;
            it->length    = offset_in_piece;

            it++;
            m_pieces.insert(it, Piece { .type   = BufferType::ADD,
                                        .offset = add_offset,
                                        .length = text.length() });
            m_pieces.insert(it, after);
            return;
        }

        current_pos += it->length;
    }

    m_pieces.emplace_back(BufferType::ADD, add_offset, text.length());
}


void
PieceTable::mf_do_erase(std::size_t position, std::size_t length)
{
    std::size_t current_pos { 0 };
    auto        it { m_pieces.begin() };

    while (it != m_pieces.end() && length > 0)
    {
        if (current_pos + it->length <= position)
        {
            current_pos += it->length;
            it++;
            continue;
        }

        std::size_t start_in_piece { (position > current_pos)
                                         ? position - current_pos
                                         : 0 };
        std::size_t len_to_erase { std::min(it->length - start_in_piece,
                                            length) };

        if (start_in_piece == 0 && len_to_erase == it->length)
        {
            /* erase entire piece */
            it = m_pieces.erase(it);
        }
        else if (start_in_piece == 0)
        {
            /* erase from start */

            it->offset += len_to_erase;
            it->length -= len_to_erase;

            it++;
        }
        else if (start_in_piece + len_to_erase == it->length)
        {
            /* erase to end */
            it->length = start_in_piece;

            it++;
        }
        else
        {
            Piece after { *it };
            after.offset += start_in_piece + len_to_erase;
            after.length -= start_in_piece + len_to_erase;
            it->length    = start_in_piece;

            it++;
            m_pieces.insert(it, after);
            it++;
        }
        length      -= len_to_erase;
        current_pos += it->length;
    }
}


auto
PieceTable::mf_get_total_length() const -> std::size_t
{
    std::size_t total { 0 };
    for (const auto &piece : m_pieces) total += piece.length;
    return total;
}

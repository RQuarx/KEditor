#include <algorithm>

#include "editor/content.hh"

using editor::content;
using kei::log_level;


content::content(kei::logger &logger, std::filesystem::path file_path)
    : m_file_path { std::move(file_path) }, m_file { m_file_path },
      m_logger { logger }
{
    m_domain
        = std::format("editor::content({})", m_file_path.filename().string());

    if (!m_file.empty())
        m_string_buffer.assign(m_file.data().data(), /* NOLINT */
                               m_file.size());

    m_logger[log_level::info, m_domain]("Opened path \"{}\"",
                                        m_file_path.string());
}


content::~content() { save(); }


void
content::add(std::size_t index, std::string &&content)
{
    index = std::min(index, m_string_buffer.size());

    while (!m_redo_stack.empty()) m_redo_stack.pop();

    m_string_buffer.insert(index, content);
    m_undo_stack.emplace(mutation::type::add, std::move(content), index);
}


void
content::remove(std::size_t index, std::size_t length)
{
    if (index >= m_string_buffer.size()) return;

    length = std::min(index + length, m_string_buffer.size() - index);

    std::string removed_content { m_string_buffer.substr(index, length) };
    m_string_buffer.erase(index, length);

    while (!m_redo_stack.empty()) m_redo_stack.pop();

    m_undo_stack.emplace(mutation::type::remove, std::move(removed_content),
                         index);
}


void
content::undo()
{
    if (m_undo_stack.empty()) return;

    mutation m { std::move(m_undo_stack.top()) };
    m_undo_stack.pop();

    m_logger[log_level::debug, m_domain](
        "Undo operation triggered ({})",
        m.type == mutation::type::add ? "add" : "remove");

    switch (m.type)
    {
    case mutation::type::add:
        m_string_buffer.erase(m.index, m.content.size());
        m_redo_stack.emplace(mutation::type::add, std::move(m.content),
                             m.index);
        break;

    case mutation::type::remove:
        m_string_buffer.insert(m.index, m.content);
        m_redo_stack.emplace(mutation::type::remove, std::move(m.content),
                             m.index);
        break;
    }
}


void
content::redo()
{
    if (m_redo_stack.empty()) return;

    mutation m { std::move(m_redo_stack.top()) };
    m_redo_stack.pop();

    m_logger[log_level::debug, m_domain](
        "Redo operation triggered ({})",
        m.type == mutation::type::add ? "add" : "remove");

    switch (m.type)
    {
    case mutation::type::add:
        m_string_buffer.insert(m.index, m.content);
        m_undo_stack.emplace(mutation::type::add, std::move(m.content),
                             m.index);
        break;

    case mutation::type::remove:
        m_string_buffer.erase(m.index, m.content.size());
        m_undo_stack.emplace(mutation::type::remove, std::move(m.content),
                             m.index);
        break;
    }
}


auto
content::get_string() const noexcept -> std::string
{
    return m_string_buffer;
}


auto
content::get_data() const noexcept -> std::string_view
{
    return std::string_view { m_string_buffer.data(), m_string_buffer.size() };
}


auto
content::save() -> bool
{
    try
    {
        std::ofstream out { m_file_path, std::ios::binary | std::ios::trunc };

        if (!out)
        {
            m_logger[log_level::error, m_domain](
                "Failed to open file for writing");
            return false;
        }

        out.write(m_string_buffer.data(), m_string_buffer.size());

        if (!out)
        {
            m_logger[log_level::error, m_domain]("Failed to write to file");
            return false;
        }

        out.flush();
    }
    catch (const std::exception &e)
    {
        m_logger[log_level::error, m_domain]("Failed to save file: {}",
                                             e.what());
        return false;
    }
    catch (...)
    {
        m_logger[log_level::error, m_domain]("Unknown error while saving file");
        return false;
    }

    return true;
}

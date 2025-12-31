#pragma once
#include <filesystem>
#include <stack>

#include "logger.hh"
#include "platform/mapped_file.hh"


namespace editor
{
    struct mutation
    {
        enum class type : std::uint8_t
        {
            add,
            remove
        } type;

        std::string content;
        std::size_t index;
    };


    class content
    {
    public:
        content(kei::logger &logger, std::filesystem::path file_path);
        ~content();


        void add(std::size_t index, std::string &&content);
        void remove(std::size_t index, std::size_t length);

        void undo();
        void redo();

        auto save() -> bool;

        [[nodiscard]] auto get_string() const noexcept -> std::string;
        [[nodiscard]] auto get_data() const noexcept -> std::string_view;

    private:
        std::filesystem::path m_file_path;
        platform::mapped_file m_file;

        std::string m_domain;

        kei::logger &m_logger;

        std::stack<mutation> m_undo_stack;
        std::stack<mutation> m_redo_stack;

        /*
        we store the changed string so we dont have to reallocate everytime
        */
        std::string m_string_buffer;
    };
}

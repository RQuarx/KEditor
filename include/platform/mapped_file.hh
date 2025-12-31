#pragma once
#include <filesystem>


namespace platform
{
    class mapped_file
    {
    public:
        mapped_file() = default;
        explicit mapped_file(const std::filesystem::path &file_path);
        ~mapped_file();

        mapped_file(const mapped_file &)                     = delete;
        auto operator=(const mapped_file &) -> mapped_file & = delete;

        mapped_file(mapped_file &&) noexcept;
        auto operator=(mapped_file &&) noexcept -> mapped_file &;


        [[nodiscard]]
        auto
        data() const noexcept -> std::string_view
        {
            return m_data;
        }


        [[nodiscard]]
        auto
        size() const noexcept -> std::size_t
        {
            return m_size;
        }


        [[nodiscard]]
        auto
        empty() const noexcept -> bool
        {
            return m_size == 0;
        }

    private:
        const char *m_data { nullptr };
        std::size_t m_size { 0UZ };

#ifdef _WIN32
        void *m_file { nullptr };
        void *m_mapping { nullptr };
#else
        int m_fd { -1 };
#endif


        void mf_open(const std::filesystem::path &file_path);
        void mf_close();
    };
}

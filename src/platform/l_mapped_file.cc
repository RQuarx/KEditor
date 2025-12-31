#include <cstring>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "exceptions.hh"
#include "platform/mapped_file.hh"

using platform::mapped_file;


mapped_file::mapped_file(const std::filesystem::path &file_path)
{
    mf_open(file_path);
}


mapped_file::~mapped_file() { mf_close(); }


mapped_file::mapped_file(mapped_file &&other) noexcept
{
    *this = std::move(other);
}


auto
mapped_file::operator=(mapped_file &&other) noexcept -> mapped_file &
{
    if (this != &other)
    {
        mf_close();

        m_data = other.m_data;
        m_size = other.m_size;
        m_fd   = other.m_fd;

        other.m_data = nullptr;
        other.m_size = 0;
        other.m_fd   = -1;
    }
    return *this;
}


void
mapped_file::mf_open(const std::filesystem::path &file_path)
{
    m_fd = ::open(file_path.c_str(), O_RDONLY);
    if (m_fd < 0)
        throw kei::filesystem_error { "::open(): {}", std::strerror(errno) };

    struct stat st {};
    if (fstat(m_fd, &st) < 0)
        throw kei::filesystem_error { "fstat(): {}", std::strerror(errno) };

    if (st.st_size == 0) return;

    m_size = static_cast<size_t>(st.st_size);

    void *view { mmap(nullptr, m_size, PROT_READ, MAP_PRIVATE, m_fd, 0) };
    if (view == MAP_FAILED)
        throw kei::filesystem_error { "mmap(): {}", std::strerror(errno) };

    m_data = static_cast<const char *>(view);
}


void
mapped_file::mf_close()
{
    if (m_data != nullptr) munmap(const_cast<char *>(m_data), m_size);
    if (m_fd != -1) ::close(m_fd);

    m_data = nullptr;
    m_size = 0;
    m_fd   = -1;
}

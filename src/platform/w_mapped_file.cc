#include <windows.h>

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

        m_data    = other.m_data;
        m_size    = other.m_size;
        m_file    = other.m_file;
        m_mapping = other.m_mapping;

        other.m_data    = nullptr;
        other.m_size    = 0;
        other.m_file    = nullptr;
        other.m_mapping = nullptr;
    }
    return *this;
}


void
mapped_file::mf_open(const std::filesystem::path &path)
{
    HANDLE file { CreateFileW(path.wstring().c_str(), GENERIC_READ,
                              FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL, nullptr) };

    if (file == INVALID_HANDLE_VALUE)
        throw kei::filesystem_error { "CreateFileW" };

    LARGE_INTEGER size;
    if (!GetFileSizeEx(file, &size))
        throw kei::filesystem_error { "GetFileSizeEx failed" };

    if (size.QuadPart == 0)
    {
        m_file = file;
        return;
    }

    HANDLE mapping { CreateFileMappingW(file, nullptr, PAGE_READONLY, 0, 0,
                                        nullptr) };

    if (!mapping) throw kei::filesystem_error { "CreateFileMapping failed" };

    void *view { MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0) };
    if (!view) throw kei::filesystem_error { "MapViewOfFile failed" };

    m_file    = file;
    m_mapping = mapping;
    m_data    = static_cast<const char *>(view);
    m_size    = static_cast<size_t>(size.QuadPart);
}

void
mapped_file::mf_close()
{
    if (m_data != nullptr) UnmapViewOfFile(m_data);
    if (m_mapping != nullptr) CloseHandle(m_mapping);
    if (m_file != nullptr) CloseHandle(m_file);

    m_data    = nullptr;
    m_size    = 0;
    m_file    = nullptr;
    m_mapping = nullptr;
}

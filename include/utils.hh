#ifndef _KEDITOR_UTILS_HH
#define _KEDITOR_UTILS_HH
#include <cstring>
#include <fstream>
#include <string>

#include "exceptions.hh"


namespace kei
{
    [[nodiscard]]
    auto getenv(const std::string &name, const std::string &other = "")
        -> std::string;


    template <typename T_FStream>
    [[nodiscard]]
    auto
    open_file(const std::string &path) -> T_FStream
        requires(std::is_same_v<T_FStream, std::fstream>
                 || std::is_same_v<T_FStream, std::ifstream>
                 || std::is_same_v<T_FStream, std::ofstream>)
    {
        T_FStream stream { path };

        if (!stream.is_open())
            throw kei::FilesystemError { "I/O Error" };

        return stream;
    }
}

#endif /* _KEDITOR_UTILS_HH */

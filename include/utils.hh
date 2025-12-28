#pragma once
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

        if (!stream.is_open()) throw kei::filesystem_error { "I/O Error" };

        return stream;
    }


    template <typename T_IntType>
    [[nodiscard]]
    auto
    to_int(std::string_view string) noexcept -> std::optional<T_IntType>
    {
        T_IntType val;
        return std::from_chars(string.begin(), string.end(), val).ec
                    == std::errc {}
                 ? std::optional { val }
                 : std::nullopt;
    }
}

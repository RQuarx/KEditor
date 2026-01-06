#include <filesystem>

#include "utils.hh"

namespace fs = std::filesystem;


auto
kei::getenv(const std::string &name, const std::string &other) -> std::string
{
    const char *value { std::getenv(name.c_str()) };
    return value == nullptr ? other : value;
}


auto
kei::get_asset(std::string_view name, const std::string &fallback) noexcept
    -> std::string
{
    static std::array<fs::path, 2> candidates { fs::current_path(),
                                                fs::path { "/etc/keditor" } };

    for (const auto &base : candidates)
        if (fs::exists(base / name)) return { base / name };

    return fallback;
}


auto
kei::get_asset(std::string_view name) noexcept -> std::optional<std::string>
{
    std::string result { get_asset(name, {}) };
    return result.empty() ? std::nullopt : std::optional { result };
}

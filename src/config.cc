#include <fstream>

#include <json/json.h>

#include "config.hh"

namespace fs = std::filesystem;


namespace
{
    [[nodiscard]]
    auto
    get_config_directory() -> std::string
    {
#ifdef _WIN32
        const char *APPDATA { std::getenv("APPDATA") };
        return APPDATA == nullptr ? "" : APPDATA;

#elifdef __APPLE__
        const char *HOME { std::getenv("HOME") };
        if (HOME == nullptr) return "";
        return std::format("{}/Library/Application Support", HOME);

#else
        const char *XDG_CONFIG_HOME { std::getenv("XDG_CONFIG_HOME") };
        if (XDG_CONFIG_HOME != nullptr) return XDG_CONFIG_HOME;

        const char *HOME { std::getenv("HOME") };
        if (HOME == nullptr) return "";
        return std::format("{}/.config", HOME);
#endif
    }
}


Config::Config(const std::span<char *> &arg_values)
{
    parse_config_file();
    parse_args(arg_values);
}


void
Config::parse_config_file()
{
    std::string config_path { get_config_directory() };
    if (config_path.empty())
        throw kei::FilesystemError { "No valid config path found." };

    m_config_path = fs::path { config_path } / "keditor" / "config.json";

    if (!fs::exists(m_config_path))
        m_config_path = fs::current_path() / "keditor" / "config.json";

    std::ifstream file { m_config_path };

    Json::CharReaderBuilder builder;
    std::string             err;

    if (!Json::parseFromStream(builder, file, &m_config, &err))
        throw kei::ParsingError { "{}", err };
}


void
Config::parse_args(const std::span<char *> &arg_values)
{
    m_arg_values["locale"] = std::locale {}.name();

    for (std::string_view arg : arg_values) {}
}

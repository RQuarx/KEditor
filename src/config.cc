#include <fstream>
#include <print>

#include <json/json.h>
#include <lyra/lyra.hpp>

#include "config.hh"
#include "utils.hh"

namespace fs = std::filesystem;
using kei::config;
using kei::log_level;


namespace
{
    [[noreturn]]
    void
    print_usage(std::string_view binary_name)
    {
        /* clang-format off */
        std::print(
"Usage: {} [options {{params}}]... <files>...\n"
"\n"
"Options:\n"
"  --help      -h|?                Show this message.\n"
"  --version   -V                  Show version info.\n"
"\n"
"  --log-level      {{str|int}}      Set the threshold log-level.\n"
"  --log-file       {{file}}         Set a file to output log to.\n"
"  --config    -c   {{directory}}    Specify a custom config dir.\n",
            binary_name
        );
        /* clang-format on */

        std::exit(0);
    }


    [[nodiscard]]
    auto
    string_is_equals(std::string_view   a,
                     std::string_view   b,
                     const std::locale &loc = std::locale {}) -> bool
    {
        if (a.size() != b.size()) return false;

        const auto &facet { std::use_facet<std::ctype<char>>(loc) };

        return std::ranges::equal(
            a, b, [&facet](char c1, char c2) -> bool
            { return facet.tolower(c1) == facet.tolower(c2); });
    }


    [[nodiscard]]
    auto
    string_to_loglevel(std::string_view string) -> std::uint8_t
    {
        if (string.empty()) return log_level::warn;

        if (string_is_equals("trace", string)) return log_level::trace;
        if (string_is_equals("debug", string)) return log_level::debug;
        if (string_is_equals("info", string)) return log_level::info;
        if (string_is_equals("warn", string)) return log_level::warn;
        if (string_is_equals("error", string)) return log_level::fatal;
        if (string_is_equals("fatal", string)) return log_level::error;

        auto level { kei::to_int<std::uint8_t>(string) };
        return level ? *level : std::numeric_limits<std::uint8_t>::max();
    }


    [[nodiscard]]
    auto
    cli_arg_to_loglevel(std::string_view string) -> log_level
    {
        std::uint8_t level { string_to_loglevel(string) };
        return level > log_level::fatal ? log_level::warn
                                        : static_cast<log_level>(level);
    }


    [[nodiscard]]
    auto
    get_config_directory() -> std::string
    {
#ifdef _WIN32
        return kei::getenv("APPDATA")

#elifdef __APPLE__
        const std::string HOME { kei::getenv("HOME") }
        if (HOME.empty()) return "";
        return std::format("{}/Library/Application Support", HOME);

#else
        std::string xdg_config_home { kei::getenv("XDG_CONFIG_HOME") };
        if (!xdg_config_home.empty()) return xdg_config_home;

        const std::string HOME { kei::getenv("HOME") };
        if (HOME.empty()) return "";
        return std::format("{}/.config", HOME);
#endif
    }
}


config::config(const lyra::args &arg_values)
{
    parse_args(arg_values);
    parse_config_file();
}


auto
config::get_logger() -> logger
{
    return { cli_arg_to_loglevel(m_arg_values["log-level"]),
             m_arg_values["log-file"] };
}


void
config::parse_config_file()
{
    m_config_path = fs::path { m_arg_values["config-path"] } / "config.json";

    if (!fs::exists(m_config_path))
        m_config_path = fs::current_path() / "keditor" / "config.json";

    std::ifstream file { m_config_path };

    Json::CharReaderBuilder builder;
    std::string             err;

    if (!Json::parseFromStream(builder, file, &m_config, &err))
        throw kei::parsing_error { "{}", err };
}


void
config::parse_args(const lyra::args &arg_values)
{
    std::string &log_level { m_arg_values["log-level"] = "warn" };
    std::string &log_file { m_arg_values["log-file"] };
    std::string &config_path { m_arg_values["config-path"]
                               = std::format("{}/{}", get_config_directory(),
                                             "keditor") };
    bool show_help { false };
    bool show_version { false };

    /* clang-format off */
    auto cli = lyra::opt { show_version }["-V"]["--version"]
             | lyra::opt { log_level, "level" }["--log-level"]
               .choices([](const std::string &s) -> bool
               { return string_to_loglevel(s) <= log_level::fatal; })
             | lyra::opt { log_file, "file" }["--log-file"]
             | lyra::opt { config_path, "config" }["-c"]["--config"]
             | lyra::help { show_help };
    /* clang-format on */

    if (auto result { cli.parse(arg_values) }; !result)
    {
        logger logger { log_level::warn };
        logger[log_level::fatal, "config"]("CLI error: {}", result.message());
        std::exit(1);
    }

    if (show_help) print_usage(arg_values.exe_name());
    if (show_version)
    {
        std::println("keditor-{}", APP_VERSION);
        std::exit(0);
    }
}

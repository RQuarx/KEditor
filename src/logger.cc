#include <chrono>
#include <cstring>
#include <iostream>

#include "exceptions.hh"
#include "logger.hh"


namespace
{
    template <typename T> using tpair = std::pair<T, T>;


    auto
    string_to_loglevel(std::string_view str) -> LogLevel
    {
        if (str.contains("debug")) return LogLevel::DEBUG;
        if (str.contains("info")) return LogLevel::INFO;
        if (str.contains("warn")) return LogLevel::WARN;
        if (str.contains("error")) return LogLevel::ERROR;
        return LogLevel::MAX;
    }


    /* clang-format off */
     constexpr std::array<tpair<std::string_view>, 4> LABELS {{
         { "\033[1;36mdebug\033[0;0;0m", "debug" },
         { "\033[1;32minfo\033[0;0;0m ", "info " },
         { "\033[1;33mwarn\033[0;0;0m ", "warn " },
         { "\033[1;31merror\033[0;0;0m", "error" },
    }};
    /* clang-format on */
}


Logger::Logger(std::string_view log_level, const std::string &log_file)
{
    set_log_level(log_level);
    set_log_file(log_file);
}


auto
Logger::set_log_level(std::string_view log_level) -> Logger &
{
    if (log_level.empty()) goto err;

    try
    {
        auto level { static_cast<std::uint32_t>(LogLevel::WARN) };

        if (std::from_chars(log_level.begin(), log_level.end(), level).ec
            != std::errc {})
            throw kei::InvalidArgument { "" };

        if (static_cast<LogLevel>(level) >= LogLevel::MAX)
        {
            log<LogLevel::WARN>("Log level too large, using default level");
            goto err;
        }

        m_threshold_level = static_cast<LogLevel>(level);
    }
    catch (...)
    {
        LogLevel level { string_to_loglevel(log_level) };

        if (level == LogLevel::MAX)
        {
            log<LogLevel::WARN>("Invalid log level {}, using default level",
                                log_level);
            goto err;
        }

        m_threshold_level = level;
    }

    return *this;

err:
    m_threshold_level = LogLevel::WARN;
    return *this;
}


auto
Logger::set_log_file(const std::string &log_file) -> Logger &
{
    if (log_file.empty()) return *this;

    m_log_file.open(log_file, std::ios_base::app);

    if (m_log_file.fail() && !m_log_file.eof())
    {
        log<LogLevel::ERROR>("Failed to open {}: {}", log_file,
                             std::strerror(errno));

        throw kei::FilesystemError { "Failed to open log file {}", log_file };
    }

    return *this;
}


void
Logger::write(LogLevel level, std::string_view domain, const std::string &msg)
{
    tpair<std::string_view> labels { LABELS[static_cast<size_t>(level)] };

    std::string label { std::format("{} {} at \033[38;2;70;172;173m{}\033[0;0m",
                                    get_time(), labels.first, domain) };

    if (m_log_file.is_open())
    {
        std::string file_label { std::format("{} at {}", get_time(),
                                             labels.second, domain) };

        m_log_file << std::format("[{}]: {}", file_label, msg) << '\n';
        m_log_file.flush();
    }

    if (level < m_threshold_level) return;

    std::size_t label_len { label.length() };
    m_longest_label = std::max(m_longest_label, label_len);

    std::println(std::cerr, "[{}]: {}\033[1m{}\033[0m", label,
                 std::string(m_longest_label - label_len, ' '), msg);
}


auto
Logger::get_time() -> std::string
{
    using std::chrono::duration;
    using ms = std::chrono::milliseconds;
    using m  = std::chrono::minutes;
    using s  = std::chrono::seconds;

    duration now { std::chrono::system_clock::now().time_since_epoch() };
    ms       millis { std::chrono::duration_cast<ms>(now) % 1000 };
    m        minutes { std::chrono::duration_cast<m>(now) % 60 };
    s        seconds { std::chrono::duration_cast<s>(now) % 60 };

    return std::format("{:02}:{:02}.{:03}", minutes.count(), seconds.count(),
                       millis.count());
}

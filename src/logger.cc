#include <iostream>
#include <thread>
#include <utility>

#include "logger.hh"


using kei::log_entry;
using kei::log_level;
using kei::logger;


namespace
{
    template <typename T> using tpair = std::pair<T, T>;


    enum class log_type : std::uint8_t
    {
        file,
        console,
    };


    [[nodiscard]]
    auto
    get_time(const std::chrono::time_point<std::chrono::system_clock> &now)
        -> std::string
    {
        using std::chrono::duration;
        using ms = std::chrono::milliseconds;
        using m  = std::chrono::minutes;
        using s  = std::chrono::seconds;

        auto dura { now.time_since_epoch() };
        ms   millis { std::chrono::duration_cast<ms>(dura) % 1000 };
        m    minutes { std::chrono::duration_cast<m>(dura) % 60 };
        s    seconds { std::chrono::duration_cast<s>(dura) % 60 };

        return std::format("{:02}:{:02}.{:03}", minutes.count(),
                           seconds.count(), millis.count());
    }


    /* clang-format off */
    constexpr std::array<tpair<std::string_view>, 6> LABELS {{
         { "\033[38;2;156;163;175mtrace\033[0m", "trace" },
         { "\033[38;2;59;130;246mdebug\033[0m",  "debug" },
         { "\033[38;2;34;211;238minfo\033[0m ",  "info " },
         { "\033[38;2;250;204;21mwarn\033[0m ",  "warn " },
         { "\033[38;2;239;68;68merror\033[0m",   "error" },
         { "\033[38;2;192;38;211mfatal\033[0m",  "fatal" }
    }};
    /* clang-format on */


    [[nodiscard]]
    auto
    format_log(log_type          type,
               log_level        level,
               std::string_view time,
               std::string_view domain,
               std::string_view message,
               std::string_view file_name,
               std::uint32_t    line) -> std::string
    {
        const std::string_view &label_colored { LABELS[level].first };
        const std::string_view &label_raw { LABELS[level].second };

        if (level >= log_level::error)
        {
            if (type == log_type::file)
                return std::format("{} [{} at {}:{}]: {}", time, label_raw,
                                   file_name, line, message);

            return std::format("{} [{} at \033[38;2;70;172;173m{}:{}\033[0m]: "
                               "\033[1m{}\033[0m",
                               time, label_colored, file_name, line, message);
        }

        if (type == log_type::file)
            return std::format("{} [{} at {}]: {}", time, label_raw, domain,
                               message);

        return std::format("{} [{} at \033[38;2;70;172;173m{}\033[0m]: "
                           "\033[1m{}\033[0m",
                           time, label_colored, domain, message);
    }
}


logger::logger(log_level threshold_level, const std::filesystem::path &log_file)
    : m_threshold { threshold_level }
{
    if (!log_file.empty()) m_log_file = { log_file, std::ios::app };
    m_worker = std::jthread { [this](std::stop_token st) -> void
                              { process_queue(std::move(st)); } };
}


logger::~logger()
{
    m_worker.request_stop();
    m_cv.notify_one();
}


auto
logger::operator[](log_level            level,
                   std::string_view     domain,
                   std::source_location source) noexcept -> log_entry
{
    return log_entry { level < m_threshold ? nullptr : this, level, domain,
                       source };
}


void
logger::process_queue(std::stop_token stop_token)
{
    while (!stop_token.stop_requested() || !m_log_queue.empty())
    {
        std::unique_lock lock { m_queue_mtx };

        m_cv.wait(
            lock, [&] -> bool
            { return stop_token.stop_requested() || !m_log_queue.empty(); });

        while (!m_log_queue.empty())
        {
            const auto obj { std::move(m_log_queue.front()) };
            m_log_queue.pop();

            lock.unlock();

            const std::string time_str { get_time(obj.time) };

            std::string formatted_console { format_log(
                log_type::console, obj.level, time_str, obj.domain, obj.message,
                obj.source.file_name(), obj.source.line()) };

            std::println(std::clog, "{}", formatted_console);

            if (!m_log_file.is_open())
            {
                lock.lock();
                continue;
            }

            std::string formatted_file { format_log(
                log_type::file, obj.level, time_str, obj.domain, obj.message,
                obj.source.file_name(), obj.source.line()) };

            std::println(m_log_file, "{}", formatted_file);
            m_log_file.flush();

            lock.lock();
        }
    }
}


log_entry::log_entry(logger              *parent,
                     log_level            level,
                     std::string_view     domain,
                     std::source_location source)
    : m_parent { parent }, m_obj { .time    = std::chrono::system_clock::now(),
                                   .level   = level,
                                   .domain  = domain,
                                   .source  = source,
                                   .message = "" }
{
}

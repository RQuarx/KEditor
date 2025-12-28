#pragma once
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <queue>
#include <source_location>
#include <thread>


namespace kei
{
    enum log_level : std::uint8_t
    {
        trace,
        debug,
        info,
        warn,
        error,
        fatal,
    };


    class log_entry;
    class logger
    {
        friend log_entry;

        struct log_object
        {
            std::chrono::time_point<std::chrono::system_clock> time;
            log_level                                          level;
            std::string_view                                   domain;
            std::source_location                               source;
            std::string                                        message;
        };

    public:
        logger(log_level                    threshold_level,
               const std::filesystem::path &log_file = "");
        ~logger();


        [[nodiscard]]
        auto operator[](log_level            level,
                        std::string_view     domain,
                        std::source_location source
                        = std::source_location::current()) noexcept
            -> log_entry;

    private:
        log_level     m_threshold;
        std::ofstream m_log_file;

        std::queue<log_object> m_log_queue;
        std::mutex             m_queue_mtx;

        std::condition_variable m_cv;
        std::jthread            m_worker;


        void process_queue(std::stop_token stop_token);


        template <typename... T_Args>
        void
        push_log(log_object &&object)
        {
            {
                std::scoped_lock lock { m_queue_mtx };
                m_log_queue.push(std::move(object));
            }
            m_cv.notify_one();
        }
    };


    class log_entry
    {
        friend logger;

    public:
        log_entry(const log_entry &)                     = delete;
        auto operator=(const log_entry &) -> log_entry & = delete;

        log_entry(log_entry &&)                     = default;
        auto operator=(log_entry &&) -> log_entry & = default;


        template <typename... T_Args>
        void
        operator()(std::format_string<T_Args...> fmt, T_Args &&...args)
        {
            if (m_parent == nullptr) return;

            m_obj.message = std::format(fmt, std::forward<T_Args>(args)...);
            m_parent->push_log(std::move(m_obj));
        }

    private:
        logger            *m_parent;
        logger::log_object m_obj;


        log_entry(logger              *parent,
                  log_level            level,
                  std::string_view     domain,
                  std::source_location source);
    };
}

#ifndef _KEDITOR_LOG_HH
#define _KEDITOR_LOG_HH
#include <format>
#include <fstream>
#include <source_location>


enum class LogLevel : unsigned char
{
    DEBUG = 0,
    INFO  = 1,
    WARN  = 2,
    ERROR = 3,
    MAX   = 4
};


class Logger
{
public:
    /**
     * Helper struct to store a format string and the source function name.
     *
     * [note]--------------------------------------------------------------
     *
     * Used internally for type-safe logging macros.
     */
    template <typename... T_Args> struct StringSource
    {
        std::format_string<T_Args...> fmt;
        std::string_view              func;

        constexpr StringSource(const char                 *fmt,
                               const std::source_location &source
                               = std::source_location::current())
            : fmt(fmt), func(source.function_name())
        {
        }
    };

    template <typename... T_Args>
    using StringSource_t = std::type_identity_t<StringSource<T_Args...>>;


    /**
     * Set the logger threshold level.
     *
     * [note]-------------------------
     *
     * Messages below the passed level are not printed to console.
     *
     * [params]-----------------------
     *
     * `log_level`:
     *   Either numeric or string log level ("debug", "info", etc.)
     */
    auto set_log_level(std::string_view log_level) -> Logger &;


    /**
     * Set the log file path.
     *
     * [note]----------------
     *
     * Throws kei::FilesystemError if the file cannot be opened.
     *
     * [params]--------------
     *
     * `log_file`:
     *   Path to the log file.
     */
    auto set_log_file(const std::string &log_file) -> Logger &;


    template <LogLevel T_Level, typename... T_Args>
    void
    log(StringSource_t<T_Args...> fmt, T_Args &&...args)
    {
        std::string_view func { fmt.func.substr(0, fmt.func.find('(')) };

        for (char delim : " > ")
            if (auto pos { func.find(delim) }; pos != std::string_view::npos)
                func = func.substr(pos + 1);

        write(T_Level, func,
              std::format(fmt.fmt, std::forward<T_Args>(args)...));
    }

private:
    std::ofstream m_log_file;
    LogLevel      m_threshold_level { LogLevel::WARN };
    std::size_t   m_longest_label { 0 };


    void write(LogLevel level, std::string_view domain, const std::string &msg);


    [[nodiscard]]
    static auto get_time() -> std::string;
};


inline Logger logger;

#endif /* _KEDITOR_LOG_HH */

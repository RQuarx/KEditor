#pragma once
#include <format>
#include <string>


namespace kei
{
    class exception : public std::exception
    {
    public:
        template <typename... T_Args>
        exception(std::format_string<T_Args...> fmt, T_Args &&...args)
            : m_string { std::format(fmt, std::forward<T_Args>(args)...) }
        {
        }


        exception() : m_string { "No message" } {}


        ~exception() noexcept override = default;


        [[nodiscard]]
        auto
        what() const noexcept -> const char * override
        {
            return m_string.c_str();
        }

    private:
        std::string m_string;
    };

#define EXCEPTION_OBJ(name)                     \
class name : public kei::exception /* NOLINT */ \
{                                               \
public:                                         \
    using kei::exception::exception;            \
    const std::string_view m_name { #name };    \
}

    EXCEPTION_OBJ(conversion_error);
    EXCEPTION_OBJ(parsing_error);
    EXCEPTION_OBJ(invalid_argument);
    EXCEPTION_OBJ(filesystem_error);
    EXCEPTION_OBJ(init_error);
}


#undef EXCEPTION_OBJ

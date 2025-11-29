#ifndef _KEDITOR_EXCEPTION_HH
#define _KEDITOR_EXCEPTION_HH
#include <format>
#include <string>


namespace kei
{
    class Exception : public std::exception
    {
    public:
        template <typename... T_Args>
        constexpr Exception(std::format_string<T_Args...> fmt, T_Args &&...args)
            : m_string { std::format(fmt, std::forward<T_Args>(args)...) }
        {
        }


        constexpr Exception() : m_string("No message") {}


        ~Exception() noexcept override = default;


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
class name : public kei::Exception /* NOLINT */ \
{                                               \
public:                                         \
    using kei::Exception::Exception;            \
}

    EXCEPTION_OBJ(ConversionError);
    EXCEPTION_OBJ(ParsingError);
    EXCEPTION_OBJ(InvalidArgument);
    EXCEPTION_OBJ(FilesystemError);
    EXCEPTION_OBJ(InitError);
}

namespace sdl { EXCEPTION_OBJ(Exception); }


#undef EXCEPTION_OBJ

#endif /* _KEDITOR_EXCEPTION_HH */

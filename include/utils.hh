#ifndef _KEDITOR_UTILS_HH
#define _KEDITOR_UTILS_HH
#include <string>


namespace kei
{
    [[nodiscard]] auto getenv(const std::string &name,
                              const std::string &other = "") -> std::string;
}

#endif /* _KEDITOR_UTILS_HH */

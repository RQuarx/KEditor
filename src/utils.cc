#include "utils.hh"


auto
kei::getenv(const std::string &name, const std::string &other) -> std::string
{
    const char *value { std::getenv(name.c_str()) };
    return value == nullptr ? other : value;
}
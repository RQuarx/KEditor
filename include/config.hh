#ifndef _KEDITOR_CONFIG_HH
#define _KEDITOR_CONFIG_HH
#include <filesystem>
#include <string>

#include <json/value.h>

#include "exceptions.hh"


class Config
{
public:
    Config();


    template <typename Tp>
    [[nodiscard]]
    auto
    get(const std::string &key) const -> Tp
    {
        const Json::Value *node { &m_config };

        size_t start { 0 };
        while (true)
        {
            std::size_t dot { key.find('.', start) };
            std::string part { (dot == std::string::npos)
                                   ? key.substr(start)
                                   : key.substr(start, dot - start) };

            if (!node->isObject() || !node->isMember(part))
                throw kei::InvalidArgument {
                    R"(Config: missing key "{}" in path "{}")", part, key
                };

            node = &(*node)[part];

            if (dot == std::string::npos) break;
            start = dot + 1;
        }

        return convert<Tp>(*node, key);
    }

private:
    std::filesystem::path m_config_path;

    Json::Value m_config;


    template <typename Tp>
    [[nodiscard]]
    static auto convert(const Json::Value &, const std::string &) -> Tp;
};


template <>
inline auto
Config::convert<int>(const Json::Value &v, const std::string &key) -> int
{
    if (v.isInt()) return v.asInt();

    throw kei::ConversionError {
        "Config: key \"{}\" cannot be converted to int", key
    };
}


template <>
inline auto
Config::convert<double>(const Json::Value &v, const std::string &key) -> double
{
    if (v.isDouble() || v.isInt()) return v.asDouble();

    throw kei::ConversionError {
        "Config: key \"{}\" cannot be converted to double", key
    };
}


template <>
inline auto
Config::convert<bool>(const Json::Value &v, const std::string &key) -> bool
{
    if (v.isBool()) return v.asBool();

    throw kei::ConversionError {
        "Config: key \"{}\" cannot be converted to bool", key
    };
}


template <>
inline auto
Config::convert<std::string>(const Json::Value &v, const std::string &key)
    -> std::string
{
    if (v.isString()) return v.asString();

    throw kei::ConversionError {
        "Config: key \"{}\" cannot be converted to string", key
    };
}

#endif /* _KEDITOR_CONFIG_HH */

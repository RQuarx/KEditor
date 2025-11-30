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
    get(const std::string &key, const std::string &other = "") const -> Tp
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

        try
        {
            return convert<Tp>(*node, key);
        }
        catch (const kei::ConversionError &e)
        {
            if (other.empty())
                throw e;
            return other;
        }
    }

private:
    std::filesystem::path m_config_path;

    Json::Value m_config;


    template <typename Tp>
    [[nodiscard]]
    static auto
    convert(const Json::Value &val, const std::string &key) -> Tp
    {
        if (val.is<Tp>()) return val.as<Tp>();
        throw kei::ConversionError { "{} is not of type {}", key,
                                     typeid(Tp).name() };
    }
};

#endif /* _KEDITOR_CONFIG_HH */

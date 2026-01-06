#pragma once
#include <cstdint>
#include <functional>


namespace widget
{
    using id_t = std::size_t;


    enum class orientation : std::uint8_t
    {
        vertical   = 0,
        horizontal = 0,
        v          = vertical,
        h          = horizontal
    };


    template<typename T>
    [[nodiscard]]
    constexpr auto
    make_id(T id) -> id_t
    {
        return std::hash<T> {}(id);
    }
}

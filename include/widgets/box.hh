#pragma once
#include "sdl/rect.hh"
#include "widgets/base.hh"


namespace widget
{
    class box : public base
    {
    public:
        box(sdl::frect rect) noexcept;

        auto render(sdl::renderer &render) -> box & override;
    };
}


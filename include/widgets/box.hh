#pragma once
#include "sdl/context.hh"
#include "widgets/widget.hh"


namespace widget
{
    namespace config
    {
        inline struct box_config
        {
            sdl::color background { 0x2A4A7B8A_rgba };
            sdl::color border     { 0x6D6D8080_rgba };
        } box;
    }


    void box(sdl::context &ctx, sdl::frect rect, float border = 0);


    class scoped_box
    {
    public:
        scoped_box(sdl::context &ctx, sdl::frect rect, float border = 0);
        ~scoped_box();

        scoped_box(const scoped_box &)                     = delete;
        auto operator=(const scoped_box &) -> scoped_box & = delete;
        scoped_box(scoped_box &&)                          = delete;
        auto operator=(scoped_box &&) -> scoped_box &      = delete;

    private:
        sdl::context &m_ctx;
    };


    class box_factory
    {
    public:
        box_factory(sdl::context &ctx,
                    sdl::frect    region      = {},
                    orientation   orientation = orientation::horizontal);


        void box(float extent, float border = 0);


        [[nodiscard]]
        auto scoped_box(float extent, float border = 0) -> scoped_box;


        [[nodiscard]] auto remaining() const noexcept -> float;
        [[nodiscard]] auto split(float       ratio,
                                 orientation orientation
                                 = orientation::horizontal) noexcept
            -> box_factory;


        float padding { 0.0F };
        float spacing { 0.0F };

    private:
        sdl::context &m_ctx;
        sdl::frect    m_region;
        sdl::fpoint   m_cursor;
        orientation   m_orientation;


        auto mf_next_rect(float extent) noexcept -> sdl::frect;
    };
}

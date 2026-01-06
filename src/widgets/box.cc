#include "widgets/box.hh"


namespace widget
{
    namespace
    {
        [[nodiscard]]
        auto
        inset_rect(sdl::frect r, float inset) noexcept -> sdl::frect
        {
            auto d { inset * 2.0F };

            r.x += inset;
            r.y += inset;
            r.w  = std::max(0.0F, r.w - d);
            r.h  = std::max(0.0F, r.h - d);

            return r;
        }


        void
        render_box(sdl::render_context &render, sdl::frect rect, float border)
        {

            if (border > 0.0F)
            {
                render.renderer.set_draw_color(config::box.border);
                render.renderer.render_rect(rect, true);

                rect = inset_rect(rect, border);
            }

            render.renderer.set_draw_color(config::box.background);
            render.renderer.render_rect(rect, true);
        }
    }


    void
    box(sdl::context &ctx, sdl::frect rect, float border)
    {
        sdl::frect final { ctx.resolve_rect(rect) };
        render_box(ctx.render, final, border);
    }


    scoped_box::scoped_box(sdl::context &ctx, sdl::frect rect, float border)
        : m_ctx { ctx }
    {
        sdl::frect final { ctx.resolve_rect(rect) };

        m_ctx.push_clip(final);
        m_ctx.push_origin({ final.x, final.y });

        render_box(ctx.render, final, border);
    }


    scoped_box::~scoped_box()
    {
        m_ctx.pop_origin();
        m_ctx.pop_clip();
    }


    box_factory::box_factory(sdl::context &ctx,
                             sdl::frect    region,
                             orientation   orientation)
        : m_ctx { ctx }, m_orientation { orientation }
    {
        if (region.w <= 0.0F || region.h <= 0.0F)
        {
            auto ws { ctx.render.renderer.get_window().get_size() };

            region.w = static_cast<float>(ws->w);
            region.h = static_cast<float>(ws->h);
        }

        m_region = region;
        m_cursor = { .x = region.x, .y = region.y };
    }


    void
    box_factory::box(float extent, float border)
    {
        widget::box(m_ctx, mf_next_rect(extent), border);
    }


    auto
    box_factory::scoped_box(float extent, float border) -> class scoped_box
    {
        return widget::scoped_box { m_ctx, mf_next_rect(extent), border };
    }


    auto
    box_factory::mf_next_rect(float extent) noexcept -> sdl::frect
    {
        sdl::frect r {};

        if (m_orientation == orientation::vertical)
        {
            r = { .x = m_region.x + padding,
                  .y = m_cursor.y + padding,
                  .w = m_region.w - (padding * 2.0F),
                  .h = extent };

            m_cursor.y += extent + spacing;
        }
        else
        {
            r = { .x = m_cursor.x + padding,
                  .y = m_region.y + padding,
                  .w = extent,
                  .h = m_region.h - (padding * 2.0F) };

            m_cursor.x += extent + spacing;
        }

        return r;
    }


    auto
    box_factory::split(float ratio, orientation orientation) noexcept
        -> box_factory
    {
        ratio = std::clamp(ratio, 0.0F, 1.0F);

        sdl::frect child { m_region };

        if (m_orientation == orientation::vertical)
        {
            float h { m_region.h * ratio };

            child.y = m_cursor.y;
            child.h = h;

            m_cursor.y += h + spacing;
        }
        else
        {
            float w { m_region.w * ratio };

            child.x = m_cursor.x;
            child.w = w;

            m_cursor.x += w + spacing;
        }

        box_factory factory { m_ctx, child, orientation };
        factory.padding = padding;
        factory.spacing = spacing;

        return factory;
    }
}

#include "sdl/text.hh"
#include "utils.hh"
#include "widgets/label.hh"


namespace widget
{
    namespace
    {
        [[nodiscard]]
        auto
        get_text(config::label_resource &resource,
                 sdl::render_context    &render,
                 id_t                    id,
                 const std::string      &text) -> sdl::text &
        {
            if (!resource.font)
                resource.font
                    = { kei::get_asset("fonts/FiraMono-Regular.ttf", {}),
                        24.0F };

            std::string key { std::format("{}|{}|{}",
                                          resource.font.get_family_name(),
                                          resource.font.get_ptsize(), id) };


            auto it { resource.text_cache.find(key) };
            if (it == resource.text_cache.end())
            {
                return resource.text_cache
                    .emplace(key,
                             sdl::text { render.renderer, resource.font, text })
                    .first->second;
            }

            if (it->second.get_string() != text) it->second.set_string(text);

            return it->second;
        }


        void
        render_label(sdl::context         &ctx,
                     id_t                  id,
                     const std::string    &text,
                     sdl::fpoint           position,
                     sdl::fsize            size,
                     config::label_config &config)
        {
            auto &resource { ctx.get_widget_data<config::label_resource>(id) };

            sdl::text &text_obj { get_text(resource, ctx.render, id, text) };

            if (size.w == 0 || size.h == 0)
                size = resource.font.get_string_size(text);

            sdl::render_context &render { ctx.render };

            sdl::frect rect { ctx.resolve_rect(
                { position.x, position.y, size.w, size.h }) };

            if (config.background.a > 0)
            {
                render.renderer.set_draw_color(config.background);
                render.renderer.render_rect(rect);
            }

            text_obj.set_color(config.foreground);
            text_obj.render({ rect.x, rect.y });
        }
    }


    auto
    get_label_font(sdl::context &context, id_t id) -> sdl::font *
    {
        auto &resource { context.get_widget_data<config::label_resource>(id) };
        return !resource.font ? nullptr : &resource.font;
    }


    void
    label(sdl::context      &ctx,
          id_t               id,
          const std::string &text,
          sdl::fpoint        position,
          sdl::fsize         size)
    {
        render_label(ctx, id, text, position, size, config::label);
    }


    void
    label_disabled(sdl::context      &ctx,
                   id_t               id,
                   const std::string &text,
                   sdl::fpoint        position,
                   sdl::fsize         size)
    {
        render_label(ctx, id, text, position, size, config::label_disabled);
    }
}

#pragma once
#include "sdl/context.hh"
#include "sdl/font.hh"
#include "sdl/text.hh"


namespace widget
{
    namespace config
    {
        inline struct label_config
        {
            sdl::color background { 0x00000000_rgba };
            sdl::color foreground { 0xFFFFFF_rgb };
        } label;


        inline label_config label_disabled {
            .background = 0x00000000_rgba,
            .foreground = 0x7F7F7F_rgb,
        };


        struct label_resource : sdl::widget_context
        {
            sdl::font                                  font;
            std::unordered_map<std::string, sdl::text> text_cache;

            ~label_resource() override = default;
        };
    }


    template <typename... T_Args>
    void
    set_label_font(sdl::context &context, id_t id, T_Args &&...params)
    {
        auto &resource { context.get_widget_data<config::label_resource>(id) };
        resource.font
            = std::make_unique<sdl::font>(std::forward<T_Args>(params)...);
    }


    [[nodiscard]]
    auto get_label_font(sdl::context &context, id_t id) -> sdl::font *;


    /**
     * A widget which draws a piece of text @param text at location
     * @param position. @param id is used to differenciate labels with different
     * fonts, use @e widget::set_label_font(...) to set the font for each ids.
     *
     * If @param size is specified, the size of the label will be forced to be
     * the specified size instead of the size of the string.
     */
    void label(sdl::context      &ctx,
               id_t               id,
               const std::string &text,
               sdl::fpoint        position,
               sdl::fsize         size = {});


    void label_disabled(sdl::context      &ctx,
                        id_t               id,
                        const std::string &text,
                        sdl::fpoint        position,
                        sdl::fsize         size = {});

}

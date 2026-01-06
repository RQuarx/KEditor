#include <lyra/args.hpp>

#include "config.hh"
#include "sdl/context.hh"
#include "sdl/event.hh"
#include "sdl/renderer.hh"
#include "widgets/button.hh"
#include "widgets/label.hh"
#include "widgets/widget.hh"

using kei::log_level;


namespace
{
    [[nodiscard]]
    auto
    create_renderer() -> sdl::renderer
    {
        sdl::window   window { APP_NAME, SDL_WINDOW_HIGH_PIXEL_DENSITY
                                           | SDL_WINDOW_RESIZABLE };
        sdl::renderer render { std::move(window), "" };

        return render;
    }
}


auto
main(int argc, char **argv) -> int
try
{
    kei::config config {
        lyra::args { argc, argv }
    };

    kei::logger logger { config.get_logger() };

    sdl::event_handler handler;
    sdl::context       ctx { handler };

    ctx.render.renderer = create_renderer();

    while (true)
    {
        handler.poll(ctx.render.renderer);
        if (handler.should_exit()) break;

        ctx.begin_frame();

        auto *size_buf { widget::get_label_font(ctx, 0) };

        sdl::fsize size;
        if (size_buf == nullptr)
            size = { .w = 100, .h = 50 };
        else
            size = size_buf->get_string_size("Hello, World!");

        auto res { widget::button(ctx, widget::make_id("0"),
                                  { 10, 10, size.w, size.h }) };

        logger[log_level::debug, "main"](
            "{}", res == widget::button_result::hover     ? "hover"
                  : res == widget::button_result::clicked ? "clicked"
                                                          : "none");

        widget::label(ctx, 0, "Hello, World!", { 10, 10 });

        ctx.end_frame();
    }

    return 0;
}
catch (const kei::exception &e)
{
    kei::logger logger { log_level::warn };
    logger[log_level::fatal, "main"]("{}", e.what());
    return 1;
}

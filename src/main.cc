#include <lyra/args.hpp>

#include "config.hh"
#include "editor/content.hh"
#include "logger.hh"
#include "sdl/event.hh"
#include "sdl/instance.hh"
#include "sdl/renderer.hh"
#include "sdl/window.hh"

using kei::log_level;


namespace
{
    [[nodiscard]]
    auto
    create_window_and_renderer() -> sdl::renderer
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

    logger[log_level::info, "main"]("Initializing SDL3");
    sdl::instance SDL { SDL_INIT_VIDEO };

    logger[log_level::info, "main"]("Creating window and renderer");
    sdl::renderer      render { create_window_and_renderer() };
    sdl::event_handler event_handler;

    editor::content c { logger, "test_file" };

    c.add(0, "Hello, World!");
    c.remove(5, 8);
    c.save();

    while (true)
    {
        auto res { event_handler.poll(render) };
        if (res == sdl::event_return_type::SUCCESS) break;
        if (res == sdl::event_return_type::FAILURE) return 1;

        render.set_draw_color(0x000000_rgb);
        render.clear();


        render.present();
    }

    return 0;
}
catch (const kei::exception &e)
{
    kei::logger logger { log_level::warn };
    logger[log_level::error, "main"]("{}", e.what());
    return 1;
}

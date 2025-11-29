#include "config.hh"
#include "logger.hh"
#include "sdl/event.hh"
#include "sdl/instance.hh"
#include "sdl/renderer.hh"
#include "sdl/window.hh"
#include "utils.hh"

using enum LogLevel;


namespace
{
    [[nodiscard]]
    auto
    create_window_and_renderer() -> sdl::Renderer
    {
        auto window { sdl::Window::create(
            APP_NAME, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE) };
        if (!window)
            throw sdl::Exception { "Failed to create a window: {}",
                                   sdl::get_error() };

        auto render { sdl::Renderer::create(std::move(*window), "") };
        if (!render)
            throw sdl::Exception { "Failed to create a renderer: {}",
                                   sdl::get_error() };

        return std::move(*render);
    }


    Logger logger { kei::getenv("LOG_LEVEL", "warn"),
                    kei::getenv("LOG_FILE", "") };
}


auto
main(int argc, char **argv) -> int
try
{
    sdl::Instance SDL { SDL_INIT_VIDEO };
    Config        config;

    sdl::Renderer     render { create_window_and_renderer() };
    sdl::EventHandler event_handler;

    while (true)
    {
        auto res { event_handler.poll(render) };
        if (res == sdl::EventReturnType::SUCCESS) break;
        if (res == sdl::EventReturnType::FAILURE) return 1;

        render.set_draw_color("0E0E0E"_rgb);
        render.clear();
        render.present();
    }

    return 0;
}
catch (const kei::Exception &e)
{
    logger.log<ERROR>("Exception caught: {}", e.what());
    return 1;
}

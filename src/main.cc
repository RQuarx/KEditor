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
        sdl::Window window { APP_NAME, SDL_WINDOW_HIGH_PIXEL_DENSITY
                                           | SDL_WINDOW_RESIZABLE };
        sdl::Renderer render { std::move(window), "" };

        return render;
    }


    Logger logger { kei::getenv("LOG_LEVEL", "warn"),
                    kei::getenv("LOG_FILE", "") };
}


auto
main(int argc, char **argv) -> int
try
{
    sdl::Instance     SDL { SDL_INIT_VIDEO };
    sdl::Renderer     render { create_window_and_renderer() };
    sdl::EventHandler event_handler;

    Config config;

    while (true)
    {
        auto res { event_handler.poll(render) };
        if (res == sdl::EventReturnType::SUCCESS) break;
        if (res == sdl::EventReturnType::FAILURE) return 1;

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

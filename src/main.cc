#include "config.hh"
#include "log.hh"
#include "model/piece_table.hh"
#include "sdl/event.hh"
#include "sdl/font.hh"
#include "sdl/instance.hh"
#include "sdl/renderer.hh"
#include "sdl/window.hh"
#include "widgets/hoverable.hh"
#include "widgets/label.hh"

#define VALUE_OR_OTHER(val, other) (val) != nullptr ? (val) : (other)

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
        {
            logger.log<ERROR>("Failed to create a window: {}",
                              sdl::get_error());
            std::exit(1);
        }

        auto render { sdl::Renderer::create(std::move(*window), "") };
        if (!render)
        {
            logger.log<ERROR>("Failed to create a renderer: {}",
                              sdl::get_error());
            std::exit(1);
        }

        return std::move(*render);
    }
}


auto
main(int argc, char **argv) -> int
{
    logger.set_log_file(VALUE_OR_OTHER(std::getenv("LOG_FILE"), ""))
        .set_log_level(VALUE_OR_OTHER(std::getenv("LOG_LEVEL"), "warn"));

    Config config;
    sdl::Instance SDL { SDL_INIT_VIDEO };

    sdl::Renderer     render { create_window_and_renderer() };
    sdl::EventHandler event_handler;

    while (true)
    {
        auto res { event_handler.poll(render) };
        if (res == sdl::EventReturnType::SUCCESS) std::exit(0);
        if (res == sdl::EventReturnType::FAILURE) std::exit(1);

        render.set_draw_color("0E0E0E"_rgb);
        render.clear();
        render.present();
    }
}

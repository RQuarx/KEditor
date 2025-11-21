#include "log.hh"
#include "sdl/event.hh"
#include "sdl/font.hh"
#include "sdl/instance.hh"
#include "sdl/renderer.hh"
#include "sdl/window.hh"
#include "widgets/box.hh"

#define VALUE_OR_OTHER(val, other) (val) != nullptr ? (val) : (other)

using enum LogLevel;


namespace
{
    [[nodiscard]]
    auto
    create_window_and_renderer() -> std::pair<sdl::Window, sdl::Renderer>
    {
        auto window { sdl::Window::create(
            APP_NAME, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE) };
        if (!window)
        {
            logger.log<ERROR>("Failed to create a window: {}",
                              sdl::get_error());
            std::exit(1);
        }

        auto render { sdl::Renderer::create(*window, "") };
        if (!render)
        {
            logger.log<ERROR>("Failed to create a renderer: {}",
                              sdl::get_error());
            std::exit(1);
        }

        return { std::move(*window), std::move(*render) };
    }
}


auto
main(int argc, char **argv) -> int
{
    logger.set_log_file(VALUE_OR_OTHER(std::getenv("LOG_FILE"), ""))
        .set_log_level(VALUE_OR_OTHER(std::getenv("LOG_LEVEL"), "warn"));

    sdl::Instance SDL { SDL_INIT_VIDEO };

    auto [window, render] { create_window_and_renderer() };
    sdl::EventHandler event_handler;

    event_handler.connect(SDL_EVENT_QUIT,
                          [](const SDL_Event &) -> sdl::EventReturnType
                          { return sdl::EventReturnType::SUCCESS; });

    auto text_engine { sdl::TextEngine::create(render) };
    auto font { sdl::Font::create(
        "/usr/share/fonts/MapleMono-NF/MapleMono-NF-Regular.ttf", 26) };

    if (!font)
    {
        logger.log<LogLevel::ERROR>("Failed to open a font: {}",
                                    sdl::get_error());
        return 1;
    }

    widget::Box base { { 50, 50, 400, 400 }, 0x444444_rgb, 0xFFFFFF_rgb };

    while (true)
    {
        auto res { event_handler.poll() };
        if (res == sdl::EventReturnType::SUCCESS) std::exit(0);
        if (res == sdl::EventReturnType::FAILURE) std::exit(1);

        render.set_draw_color("0E0E0E"_rgb);
        render.clear();

        base.render(render);

        render.present();
    }
}

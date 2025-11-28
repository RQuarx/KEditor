#include "log.hh"
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

    sdl::Instance SDL { SDL_INIT_VIDEO };

    sdl::Renderer     render { create_window_and_renderer() };
    sdl::EventHandler event_handler;

    auto text_engine { sdl::TextEngine::create(render) };
    auto font_buf { sdl::Font::create(
        "/usr/share/fonts/MapleMono-NF/MapleMono-NF-Regular.ttf", 25) };

    if (!font_buf)
    {
        logger.log<LogLevel::ERROR>("Failed to open a font: {}",
                                    sdl::get_error());
        return 1;
    }

    auto font { std::make_shared<sdl::Font>(std::move(*font_buf)) };

    widget::Label label {
        { 50, 50 },
        "Hello, World!", 0x444444_rgb, 0xFFFFFF_rgb, font
    };

    widget::Hoverable hover {
        { 50, 100, 100, 100 },
        0x444444_rgb
    };

    hover.add_event_callbacks(event_handler, render);

    while (true)
    {

        auto res { event_handler.poll(render) };
        if (res == sdl::EventReturnType::SUCCESS) std::exit(0);
        if (res == sdl::EventReturnType::FAILURE) std::exit(1);

        render.set_draw_color("0E0E0E"_rgb);
        render.clear();

        // render.run_render_queue();
        if (!label.render(render)) logger.log<ERROR>("{}", sdl::get_error());
        if (!hover.render(render)) logger.log<ERROR>("{}", sdl::get_error());

        render.present();
    }
}

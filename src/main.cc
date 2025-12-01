#include "config.hh"
#include "logger.hh"
#include "model/piece_table.hh"
#include "sdl/event.hh"
#include "sdl/instance.hh"
#include "sdl/renderer.hh"
#include "sdl/window.hh"
#include "utils.hh"

using enum LogLevel;


namespace
{
    constexpr auto LOREM_IPSUM {
        R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas eget purus ut eros scelerisque euismod. Vivamus tempus luctus erat, et volutpat ex lacinia faucibus. Proin nec ullamcorper erat. Morbi rutrum, dolor non bibendum rhoncus, ligula dui ultrices ligula, vitae feugiat nisl nunc non lorem. Aenean dictum pulvinar ligula et pulvinar. Maecenas at nunc venenatis, blandit quam nec, laoreet velit. Maecenas lacinia mauris orci, eu rutrum nisl pellentesque sed. Integer metus nulla, euismod non vestibulum sed, vulputate ac nisl.

Nullam semper condimentum neque quis gravida. Aliquam vehicula lorem non quam consequat consequat. Vestibulum eu arcu tempus, ornare nibh a, maximus lorem. Vestibulum a consequat nulla. Vivamus consectetur, ante eget finibus pretium, augue velit elementum dui, sit amet euismod velit velit eget neque. Ut pretium venenatis pharetra. Phasellus non convallis nisl, eu commodo nisl. Quisque consectetur ex eu nibh mattis, vel elementum tortor accumsan.

Curabitur finibus ante velit, suscipit sollicitudin lacus porttitor eget. Praesent ipsum quam, varius at nibh id, faucibus volutpat nisl. Sed ac felis dignissim risus porttitor pretium vitae eu arcu. Ut luctus ultrices quam volutpat vulputate. Cras faucibus tellus in arcu finibus, ut pulvinar lacus semper. Fusce ut nunc ultricies, tristique ante feugiat, convallis velit. Nulla quis urna id enim dapibus faucibus eget vitae ante. Pellentesque urna est, elementum rutrum ultricies id, pellentesque id elit.

Maecenas semper ex non sapien euismod condimentum. Praesent arcu lorem, vulputate at mauris ac, feugiat mollis neque. Quisque sit amet tortor non lorem sodales viverra. Nulla venenatis diam nec urna ultricies vehicula. Integer viverra erat non nisi viverra, nec dignissim massa scelerisque. Mauris molestie pulvinar diam, blandit euismod est finibus eget. Fusce tincidunt eros id nisl faucibus, id pharetra libero lacinia. Pellentesque ac ullamcorper massa. Proin viverra magna nunc, eget fermentum dolor condimentum non. Maecenas a justo eget turpis mollis rutrum a quis quam. Aenean nec ornare ipsum, ut faucibus leo. Cras ac egestas dolor, et malesuada nisl. Fusce lobortis felis sed risus convallis laoreet. Sed id dolor vitae orci lacinia interdum. Vivamus dictum mauris dui, eu viverra odio maximus sed.

Morbi semper, nisl vel venenatis hendrerit, lorem tortor convallis nulla, id posuere felis ante a eros. Etiam commodo elit enim, a vehicula arcu dignissim at. In pulvinar semper massa, aliquam dapibus turpis tempus sit amet. Nam sed ante mattis, bibendum ipsum sed, egestas nisi. Donec venenatis dictum turpis, non dapibus ante eleifend eget. Fusce consectetur varius lacinia. Quisque pretium condimentum felis, gravida porta leo malesuada in. .)"
    };


    [[nodiscard]]
    auto
    create_window_and_renderer() -> sdl::Renderer
    {
        sdl::Window   window { APP_NAME, SDL_WINDOW_HIGH_PIXEL_DENSITY
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

    Config config {
        { argv, argv + argc }
    };

    model::PieceTable pt { LOREM_IPSUM };

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
    logger.log<ERROR>("{}", e.what());
    return 1;
}

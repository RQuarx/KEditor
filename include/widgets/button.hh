#pragma once
#include "sdl/context.hh"


namespace widget
{
    namespace config
    {
        inline struct button_config
        {
            sdl::color base_color { 0x2A4A7B8A_rgba };
            sdl::color hover_color { 0x4396FA66_rgba };
            sdl::color clicked_color { 0x4396FAB0_rgba };

            sdl::color border_color { 0x6D6D8080_rgba };
            float      border_width { 1 };

            std::uint64_t fadein_time_ms { 100 };
            std::uint64_t fadeout_time_ms { 150 };
        } button;
    }


    enum class button_result : std::uint8_t
    {
        none    = 0,
        hover   = 1 << 0,
        clicked = 1 << 1,
    };


    enum class fade_state : std::uint8_t
    {
        none,
        to_hover,
        to_normal
    };


    struct button_state
    {
        fade_state fade_state;

        sdl::color current_color;
        sdl::color fade_start_color;
        sdl::color fade_target_color;

        std::uint64_t previous_frame_time;
        float         fade_elapsed;

        bool was_hot { false };
    };


    auto button(sdl::context &ctx, id_t id, sdl::frect rect) -> button_result;
}

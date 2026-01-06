#include <print>
#include "widgets/button.hh"


namespace widget
{
    namespace
    {
        std::unordered_map<id_t, button_state> g_button_animation;


        void
        render_fade(sdl::render_context &render,
                    button_state        &state,
                    sdl::frect           rect)
        {
            std::uint64_t now { SDL_GetTicks() };

            auto delta_time { static_cast<float>(now
                                                 - state.previous_frame_time) };

            state.fade_elapsed += delta_time;

            std::uint64_t duration { state.fade_state == fade_state::to_hover
                                         ? config::button.fadein_time_ms
                                         : config::button.fadeout_time_ms };

            duration = std::max<std::uint64_t>(duration, 1);

            auto t { std::min(state.fade_elapsed / duration, 1.0F) };

            state.current_color = sdl::color::lerp(state.fade_start_color,
                                                   state.fade_target_color, t);

            render.renderer.set_draw_color(state.current_color);
            render.renderer.render_rect(rect);

            state.previous_frame_time = now;
            if (t >= 1.0F) state.fade_state = fade_state::none;
        }


        void
        update_fade(button_state &state,
                    sdl::color    base,
                    sdl::color    hover,
                    bool          is_hot)
        {
            if (is_hot && !state.was_hot)
            {
                state.fade_state          = fade_state::to_hover;
                state.fade_elapsed        = 0.0F;
                state.fade_start_color    = state.current_color;
                state.fade_target_color   = hover;
                state.previous_frame_time = SDL_GetTicks();
            }
            else if (!is_hot && state.was_hot)
            {
                state.fade_state          = fade_state::to_normal;
                state.fade_elapsed        = 0.0F;
                state.fade_start_color    = state.current_color;
                state.fade_target_color   = base;
                state.previous_frame_time = SDL_GetTicks();
            }

            state.was_hot = is_hot;
        }
    }


    auto
    button(sdl::context &ctx, id_t id, sdl::frect rect) -> button_result
    {
        sdl::render_context &render { ctx.render };
        sdl::input_context  &input { ctx.input };

        rect = ctx.resolve_rect(rect);

        bool hovered { sdl::is_point_in_rect(rect, input.cursor_position) };
        if (hovered) ctx.set_hot(id);

        auto [it, inserted] { g_button_animation.try_emplace(id) };
        button_state &state { it->second };

        if (inserted) state.current_color = config::button.base_color;

        update_fade(state, config::button.base_color,
                    config::button.hover_color, hovered);

        bool clicked { false };

        if (hovered && ctx.mouse_pressed(sdl::mouse_button::left))
            ctx.set_active(id);

        if (ctx.is_active(id) && ctx.mouse_released(sdl::mouse_button::left))
        {
            std::println("eee");
            if (hovered) clicked = true;

            ctx.clear_active();
        }

        if (ctx.is_active(id))
        {
            render.renderer.set_draw_color(config::button.clicked_color);
            render.renderer.render_rect(rect);
        }
        else if (state.fade_state != fade_state::none)
        {
            render_fade(render, state, rect);
        }
        else
        {
            render.renderer.set_draw_color(state.current_color);
            render.renderer.render_rect(rect);
        }

        if (clicked) return button_result::clicked;
        if (ctx.is_hot(id)) return button_result::hover;

        return button_result::none;
    }
}

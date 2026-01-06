#include "sdl/context.hh"

using sdl::context;


context::context(event_handler &handler) : m_instance { SDL_INIT_VIDEO }
{
    render.origin_stack.push_back({ 0.0F, 0.0F });

    auto bind { [&handler, this](SDL_EventType type) -> void
                {
                    m_connections.push_back(handler.connect(
                        type, [this](const sdl::event &e) -> void
                        { handle_event(e); }));
                } };

    bind(SDL_EVENT_MOUSE_MOTION);
    bind(SDL_EVENT_MOUSE_BUTTON_DOWN);
    bind(SDL_EVENT_MOUSE_BUTTON_UP);
    bind(SDL_EVENT_MOUSE_WHEEL);
    bind(SDL_EVENT_KEY_DOWN);
    bind(SDL_EVENT_KEY_UP);
    bind(SDL_EVENT_TEXT_INPUT);
    bind(SDL_EVENT_WINDOW_RESIZED);
    bind(SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED);
}


void
context::begin_frame()
{
    input.cursor_delta = { .x = 0.0F, .y = 0.0F };
    input.scroll       = { .x = 0.0F, .y = 0.0F };

    input.text_input.clear();

    input.hot_id = 0;

    input.keys_pressed.reset();
    input.keys_released.reset();

    for (auto &b : input.mouse_buttons)
    {
        b.pressed  = false;
        b.released = false;
    }

    render.renderer.set_draw_color(0x0F0F0FF0_rgba);
    render.renderer.clear();
}


void
context::end_frame()
{
    render.clip_stack.clear();
    render.origin_stack.clear();
    render.renderer.present();
}


auto
context::key_down(SDL_Scancode key) const -> bool
{
    return input.keys_down.test(key);
}


auto
context::key_pressed(SDL_Scancode key) const -> bool
{
    return input.keys_pressed.test(key);
}


auto
context::key_released(SDL_Scancode key) const -> bool
{
    return input.keys_released.test(key);
}


auto
context::mouse_down(mouse_button b) const -> bool
{
    return input.mouse_buttons[std::to_underlying(b)].down;
}


auto
context::mouse_pressed(mouse_button b) const -> bool
{
    return input.mouse_buttons[std::to_underlying(b)].pressed;
}


auto
context::mouse_released(mouse_button b) const -> bool
{
    return input.mouse_buttons[std::to_underlying(b)].released;
}


auto
context::is_hot(std::uint64_t id) const -> bool
{
    return input.hot_id == id;
}

auto
context::is_active(std::uint64_t id) const -> bool
{
    return input.active_id == id;
}

void
context::set_hot(std::uint64_t id)
{
    input.hot_id = id;
}

void
context::set_active(std::uint64_t id)
{
    input.active_id = id;
}

void
context::clear_active()
{
    input.active_id = 0;
}


void
context::push_clip(const frect &clip)
{
    render.clip_stack.push_back(clip);
    render.renderer.set_clip_rect(clip);
}

void
context::pop_clip()
{
    if (!render.clip_stack.empty()) render.clip_stack.pop_back();

    if (render.clip_stack.empty())
        render.renderer.clear_clip_rect();
    else
        render.renderer.set_clip_rect(render.clip_stack.back());
}


void
context::push_origin(const fpoint &origin)
{
    render.origin_stack.push_back(origin);
}


void
context::pop_origin()
{
    if (!render.origin_stack.empty()) render.origin_stack.pop_back();
}


auto
context::resolve_rect(sdl::frect rect) const noexcept -> sdl::frect
{
    sdl::frect resolved { rect };
    if (!render.origin_stack.empty())
    {
        auto origin { render.origin_stack.back() };
        resolved.x += origin.x;
        resolved.y += origin.y;
    }

    return resolved;
}


void
context::handle_event(const sdl::event &e)
{
    switch (e.type)
    {
    case SDL_EVENT_MOUSE_MOTION:
        {
            sdl::fpoint new_pos { static_cast<float>(e.motion.x),
                                  static_cast<float>(e.motion.y) };

            input.cursor_delta.x += new_pos.x - input.cursor_position.x;
            input.cursor_delta.y += new_pos.y - input.cursor_position.y;
            input.cursor_position = new_pos;
            break;
        }

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        {
            const std::uint8_t button { e.button.button };

            if (button > 5) break;

            auto      &state { input.mouse_buttons[button - 1] };
            const bool down { (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) };

            if (down && !state.down) state.pressed = true;
            if (!down && state.down) state.released = true;

            state.down = down;
            break;
        }

    case SDL_EVENT_MOUSE_WHEEL:
        {
            input.scroll.x += static_cast<float>(e.wheel.x);
            input.scroll.y += static_cast<float>(e.wheel.y);
            break;
        }

    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        {
            const SDL_Scancode sc { e.key.scancode };
            const bool         down { (e.type == SDL_EVENT_KEY_DOWN) };

            if (down && !input.keys_down[sc]) input.keys_pressed.set(sc);
            if (!down && input.keys_down[sc]) input.keys_released.set(sc);

            input.keys_down.set(sc, down);
            break;
        }

    case SDL_EVENT_TEXT_INPUT: input.text_input += e.text.text; break;
    case SDL_EVENT_WINDOW_RESIZED:
        render.window_size = { .w = static_cast<float>(e.window.data1),
                               .h = static_cast<float>(e.window.data2) };
        break;

    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        render.framebuffer_size = { .w = static_cast<float>(e.window.data1),
                                    .h = static_cast<float>(e.window.data2) };
        break;

    default: break;
    }
}

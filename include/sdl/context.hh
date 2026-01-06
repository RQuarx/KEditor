#pragma once
#include <bitset>
#include <typeindex>
#include <vector>

#include "sdl/event.hh"
#include "sdl/instance.hh"
#include "sdl/renderer.hh"


namespace sdl
{
    enum class mouse_button : std::uint8_t
    {
        left       = 0,
        right      = 1,
        middle     = 2,
        side_front = 3,
        side_back  = 4
    };


    struct mouse_button_state
    {
        bool down { false };
        bool pressed { false };
        bool released { false };
    };


    struct input_context /* managed by `context` */
    {
        sdl::fpoint cursor_position;
        sdl::fpoint cursor_delta;

        std::array<mouse_button_state, 5> mouse_buttons;

        sdl::fpoint scroll;

        std::bitset<SDL_SCANCODE_COUNT> keys_down;
        std::bitset<SDL_SCANCODE_COUNT> keys_pressed;
        std::bitset<SDL_SCANCODE_COUNT> keys_released;

        std::string text_input;

        std::size_t hot_id { 0 };
        std::size_t active_id { 0 };
    };


    struct render_context /* managed by `context` */
    {
        renderer renderer;

        float      delta_time;
        sdl::fsize window_size;
        sdl::fsize framebuffer_size;

        std::vector<frect>  clip_stack;
        std::vector<fpoint> origin_stack;


        render_context() = default;


        render_context(const render_context &)                     = delete;
        auto operator=(const render_context &) -> render_context & = delete;
    };


    struct widget_context
    {
        virtual ~widget_context() = default;
    };


    struct context
    {
        render_context render;
        input_context  input;


        context(event_handler &handler);

        context(const context &)                     = delete;
        auto operator=(const context &) -> context & = delete;


        void begin_frame();
        void end_frame();

        [[nodiscard]] auto key_down(SDL_Scancode key) const -> bool;
        [[nodiscard]] auto key_pressed(SDL_Scancode key) const -> bool;
        [[nodiscard]] auto key_released(SDL_Scancode key) const -> bool;

        [[nodiscard]] auto mouse_down(mouse_button b) const -> bool;
        [[nodiscard]] auto mouse_pressed(mouse_button b) const -> bool;
        [[nodiscard]] auto mouse_released(mouse_button b) const -> bool;

        [[nodiscard]] auto is_hot(std::size_t id) const -> bool;
        [[nodiscard]] auto is_active(std::size_t id) const -> bool;

        void set_hot(std::size_t id);
        void set_active(std::size_t id);
        void clear_active();

        void push_clip(const frect &clip);
        void pop_clip();

        void push_origin(const fpoint &origin);
        void pop_origin();


        [[nodiscard]] auto resolve_rect(sdl::frect rect) const noexcept
            -> sdl::frect;


        template <typename T>
        [[nodiscard]]
        auto
        get_widget_data(std::size_t id) -> T &
        {
            static_assert(std::is_base_of_v<widget_context, T>,
                          "T must derive from widget_context");

            auto &slot { m_widget[typeid(T)] };
            auto  it { slot.find(id) };

            if (it == slot.end())
            {
                auto ptr { std::make_unique<T>() };
                it = slot.emplace(id, std::move(ptr)).first;
            }

            return *static_cast<T *>(it->second.get());
        }


    private:
        instance                m_instance;
        std::vector<connection> m_connections;

        std::unordered_map<
            std::type_index,
            std::unordered_map<std::size_t, std::unique_ptr<widget_context>>>
            m_widget;


        void handle_event(const sdl::event &e);
    };
}

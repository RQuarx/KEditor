#pragma once
#include <cstdint>
#include <functional>
#include <utility>

#include <SDL3/SDL_events.h>

#include "sdl/renderer.hh"
#include "sdl/signal.hh"


namespace sdl
{
    using event           = SDL_Event;
    using event_signature = std::function<void(const event &)>;

    class event_handler
    {
    public:
        using signal_t = signal<void, const event &>;

        event_handler()  = default;
        ~event_handler() = default;


        template <typename... T_Args>
        auto
        connect(SDL_EventType type, T_Args &&...args) -> connection
        {
            return m_signals[std::to_underlying(type)].connect(
                std::forward<T_Args>(args)...);
        }


        void
        handle_event(const event &e)
        {
            auto it { m_signals.find(e.type) };
            if (it == m_signals.end()) return;

            auto &sig { it->second };

            for (auto &slot : sig.get_slots()) slot.slot(e);
        }


        void
        poll(sdl::renderer &render)
        {
            event e;
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_EVENT_QUIT)
                {
                    m_should_exit = true;
                    return;
                }

                SDL_ConvertEventToRenderCoordinates(render, &e);
                handle_event(e);
            }
        }


        void
        clear()
        {
            m_signals.clear();
        }


        [[nodiscard]]
        auto
        should_exit() const noexcept -> bool
        {
            return m_should_exit;
        }

    private:
        std::unordered_map<std::uint32_t, signal_t> m_signals;
        bool                                        m_should_exit { false };
    };
}

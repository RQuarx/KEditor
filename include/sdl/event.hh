#pragma once
#include <cstdint>
#include <functional>
#include <mutex>
#include <utility>

#include <SDL3/SDL_events.h>

#include "sdl/renderer.hh"
#include "sdl/signal.hh"


namespace sdl
{
    enum class event_return_type : std::uint8_t
    {
        CONTINUE,
        SKIP,
        FAILURE,
        SUCCESS,
    };

    using event = SDL_Event;
    using event_signature
        = std::function<event_return_type(const event &, renderer &)>;

    class event_handler
    {
    public:
        using signal_t = signal<event_return_type, const event &, renderer &>;

        event_handler()  = default;
        ~event_handler() = default;


        template <typename... T_Args>
        auto
        connect(SDL_EventType type, T_Args &&...args) -> connection
        {
            std::scoped_lock lock { m_mutex };

            return m_signals[std::to_underlying(type)].connect(
                std::forward<T_Args>(args)...);
        }


        auto
        handle_event(const event &e, sdl::renderer &render) -> event_return_type
        {
            std::scoped_lock lock { m_mutex };

            if (e.type == SDL_EVENT_QUIT) return event_return_type::SUCCESS;

            auto it { m_signals.find(e.type) };
            if (it == m_signals.end()) return event_return_type::CONTINUE;

            auto &sig { it->second };

            for (auto &slot : sig.get_slots())
            {
                event_return_type r { slot.slot(e, render) };

                if (r == event_return_type::FAILURE)
                    return event_return_type::FAILURE;

                if (r == event_return_type::SUCCESS)
                    return event_return_type::SUCCESS;

                if (r == event_return_type::SKIP)
                    return event_return_type::CONTINUE;
            }

            return event_return_type::CONTINUE;
        }


        auto
        poll(sdl::renderer &render) -> event_return_type
        {
            event e;
            while (SDL_PollEvent(&e))
            {
                SDL_ConvertEventToRenderCoordinates(render.raw(), &e);

                event_return_type res { handle_event(e, render) };

                switch (res)
                {
                case event_return_type::SKIP:     [[fallthrough]];
                case event_return_type::CONTINUE: continue; ;
                case event_return_type::FAILURE:
                    return event_return_type::FAILURE;
                case event_return_type::SUCCESS:
                    return event_return_type::SUCCESS;
                };
            }

            return event_return_type::CONTINUE;
        }


        void
        clear()
        {
            std::scoped_lock lock { m_mutex };
            m_signals.clear();
        }

    private:
        std::mutex                                  m_mutex;
        std::unordered_map<std::uint32_t, signal_t> m_signals;

        std::vector<signal_t::slot_type *> m_run_next_slots;
    };
}

#ifndef _KEDITOR_SDL_EVENT_HH
#define _KEDITOR_SDL_EVENT_HH
#include <cstdint>
#include <functional>
#include <mutex>
#include <utility>

#include <SDL3/SDL_events.h>

#include "log.hh"
#include "sdl/renderer.hh"
#include "sdl/signal.hh"


namespace sdl
{
    enum class EventReturnType : std::uint8_t
    {
        CONTINUE,
        SKIP,
        FAILURE,
        SUCCESS,
    };

    using Event = SDL_Event;
    using event_signature
        = std::function<EventReturnType(const Event &, Renderer &)>;

    class EventHandler
    {
    public:
        using SignalType = Signal<EventReturnType, const Event &, Renderer &>;

        EventHandler()  = default;
        ~EventHandler() = default;


        template <typename... T_Args>
        auto
        connect(SDL_EventType type, T_Args &&...args) -> Connection
        {
            std::scoped_lock lock { m_mutex };
            return m_signals[type].connect(std::forward<T_Args>(args)...);
        }


        auto
        handle_event(const Event &e, sdl::Renderer &render) -> EventReturnType
        {
            std::scoped_lock lock { m_mutex };

            if (e.type == SDL_EVENT_QUIT) return EventReturnType::SUCCESS;

            auto it { m_signals.find(e.type) };
            if (it == m_signals.end()) return EventReturnType::CONTINUE;

            auto &sig { it->second };

            for (auto &slot : sig.get_slots())
            {
                EventReturnType r { slot.slot(e, render) };

                if (r == EventReturnType::FAILURE)
                    return EventReturnType::FAILURE;

                if (r == EventReturnType::SUCCESS)
                    return EventReturnType::SUCCESS;

                if (r == EventReturnType::SKIP)
                    return EventReturnType::CONTINUE;
            }

            return EventReturnType::CONTINUE;
        }


        auto
        poll(sdl::Renderer &render) -> EventReturnType
        {
            Event e;
            while (SDL_PollEvent(&e))
            {
                SDL_ConvertEventToRenderCoordinates(render.raw(), &e);

                EventReturnType res { handle_event(e, render) };

                switch (res)
                {
                case EventReturnType::SKIP:     [[fallthrough]];
                case EventReturnType::CONTINUE: continue; ;
                case EventReturnType::FAILURE:  return EventReturnType::FAILURE;
                case EventReturnType::SUCCESS:  return EventReturnType::SUCCESS;
                };
            }

            return EventReturnType::CONTINUE;
        }


        void
        clear()
        {
            std::scoped_lock lock(m_mutex);
            m_signals.clear();
        }

    private:
        std::mutex                                    m_mutex;
        std::unordered_map<std::uint32_t, SignalType> m_signals;

        std::vector<SignalType::slot_type *> m_run_next_slots;
    };
}

#endif /* _KEDITOR_SDL_EVENT_HH */

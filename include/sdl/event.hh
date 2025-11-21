#ifndef _KEDITOR_SDL_EVENT_HH
#define _KEDITOR_SDL_EVENT_HH
#include <cstdint>
#include <functional>
#include <list>
#include <mutex>
#include <optional>

#include <SDL3/SDL_events.h>


namespace sdl
{
    enum class EventReturnType : std::uint8_t
    {
        CONTINUE,
        RUN_NEXT, /* Force the next iteration of this callback to be ran again */
        SKIP,
        FAILURE,
        SUCCESS,
    };


    template <typename T_Ret, typename... T_Params> class Signal;

    class Connection
    {
    public:
        using signature = std::function<void()>;

        Connection()                       = default;
        Connection(const Connection &)     = delete;
        Connection(Connection &&) noexcept = default;

        auto operator=(const Connection &) -> Connection &     = delete;
        auto operator=(Connection &&) noexcept -> Connection & = default;

        explicit Connection(signature disconnect_function);
        ~Connection();


        void disconnect();

    private:
        signature m_disconnect_fn;
    };


    template <typename T_Ret, typename... T_Params> class Signal
    {
    public:
        using slot_type = std::function<T_Ret(T_Params...)>;


        Signal() = default;
        ~Signal() { clear(); }


        auto
        connect(slot_type slot) -> Connection
        {
            std::scoped_lock lock { m_mutex };

            m_slots.emplace_back(std::move(slot));
            auto it { m_slots.end() };

            return Connection(
                [this, it] -> auto
                {
                    std::scoped_lock lock(m_mutex);
                    if (it != m_slots.end()) m_slots.erase(it);
                });
        }


        auto
        emit(T_Params &&...params) -> std::vector<T_Ret>
        {
            std::scoped_lock lock(m_mutex);

            if (!m_enabled) return {};

            std::vector<T_Ret> results;
            results.reserve(m_slots.size());

            for (auto &slot : m_slots)
            {
                results.emplace_back(slot(params...));
            }
            return results;
        }


        auto
        operator()(T_Params &&...params) -> std::vector<T_Ret>
        {
            return emit(std::forward<T_Params>(params)...);
        }


        template <typename T_Pred>
        auto
        emit_until(T_Pred pred, T_Params... params) -> std::optional<T_Ret>
        {
            std::scoped_lock lock(m_mutex);

            if (!m_enabled) return std::nullopt;

            for (auto &slot : m_slots)
            {
                T_Ret r { slot(params...) };
                if (pred(r)) return r;
            }

            return std::nullopt;
        }


        void
        clear()
        {
            std::scoped_lock lock(m_mutex);
            m_slots.clear();
        }


        void
        set_enabled(bool v)
        {
            m_enabled = v;
        }


        auto
        is_enabled() const -> bool
        {
            return m_enabled;
        }


        [[nodiscard]]
        auto
        get_slots() const -> std::list<slot_type>
        {
            return m_slots;
        }


    private:
        mutable std::mutex   m_mutex;
        std::list<slot_type> m_slots;
        bool                 m_enabled { true };
    };


    using event_signature = std::function<EventReturnType(const SDL_Event &)>;


    class EventHandler
    {
    public:
        using SignalType = Signal<EventReturnType, const SDL_Event &>;

        EventHandler()  = default;
        ~EventHandler() = default;


        auto
        connect(SDL_EventType type, event_signature cb) -> Connection
        {
            std::scoped_lock lock { m_mutex };
            return m_signals[type].connect(std::move(cb));
        }


        template <typename T_Class>
        auto
        connect(SDL_EventType type,
                T_Class      *instance,
                EventReturnType (T_Class::*method)(const SDL_Event &))
            -> Connection
        {
            event_signature cb { [instance,
                                  method](const SDL_Event &e) -> EventReturnType
                                 { return (instance->*method)(e); } };

            return connect(type, std::move(cb));
        }


        template <typename T_Func, typename... T_Params>
        auto
        connect(SDL_EventType type, T_Func &&func, T_Params &&...params)
            -> Connection
        {
            event_signature cb {
                [f { std::forward<T_Func>(func) },
                 &...bound { std::forward<T_Params>(params) }](
                    const SDL_Event &e) mutable -> EventReturnType
                { return f(e, bound...); }
            };

            return connect(type, std::move(cb));
        }


        template <typename T_Class, typename T_Method, typename... T_Param>
        auto
        connect(SDL_EventType type,
                T_Class      *instance,
                T_Method    &&method,
                T_Param &&...params) -> Connection
        {
            static_assert(std::is_member_function_pointer_v<T_Method>,
                          "method must be a member function pointer");

            event_signature cb {
                [instance, method, &...bound { std::forward<T_Param>(params) }](
                    const SDL_Event &e) mutable -> EventReturnType
                { return (instance->*method)(e, bound...); }
            };

            return connect(type, std::move(cb));
        }


        auto
        handle_event(const SDL_Event &e) -> EventReturnType
        {
            std::scoped_lock lock { m_mutex };

            auto it { m_signals.find(e.type) };
            if (it == m_signals.end()) return EventReturnType::CONTINUE;

            auto &sig { it->second };

            for (auto &slot : sig.get_slots())
            {
                EventReturnType r { slot(e) };

                if (r == EventReturnType::FAILURE)
                    return EventReturnType::FAILURE;

                if (r == EventReturnType::SUCCESS)
                    return EventReturnType::SUCCESS;

                if (r == EventReturnType::SKIP)
                    return EventReturnType::CONTINUE;

                if (r == EventReturnType::RUN_NEXT)
                    m_run_next_slots.push_back(&slot);
            }

            return EventReturnType::CONTINUE;
        }


        auto
        poll() -> EventReturnType
        {
            auto match_result { [](EventReturnType type) -> int
                                {
                                    switch (type)
                                    {
                                    case EventReturnType::RUN_NEXT:
                                        [[fallthrough]];
                                    case EventReturnType::SKIP:     [[fallthrough]];
                                    case EventReturnType::CONTINUE: return 0;
                                    case EventReturnType::FAILURE:  return 1;
                                    case EventReturnType::SUCCESS:  return 2;
                                    };
                                } };

            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                int res { match_result(handle_event(e)) };

                for (auto *f : m_run_next_slots)
                {
                    int res { match_result((*f)(e)) };

                    if (res == 1) return EventReturnType::FAILURE;
                    if (res == 2) return EventReturnType::SUCCESS;
                }

                if (res == 1) return EventReturnType::FAILURE;
                if (res == 2) return EventReturnType::SUCCESS;
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

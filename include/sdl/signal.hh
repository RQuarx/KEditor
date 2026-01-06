#pragma once
#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <optional>


namespace sdl
{
    template <typename T_Ret, typename... T_Params> class signal;

    class connection
    {
    public:
        using signature = std::function<void()>;

        connection()                       = default;
        connection(const connection &)     = delete;
        connection(connection &&) noexcept = default;

        auto operator=(const connection &) -> connection &     = delete;
        auto operator=(connection &&) noexcept -> connection & = default;

        explicit connection(signature disconnect_function);
        ~connection();

        void disconnect();

    private:
        signature m_disconnect_fn;
    };


    template <typename T_Ret, typename... T_Params> class signal
    {
        struct slot_wrapper
        {
            std::function<T_Ret(T_Params...)> slot;
            std::size_t                       id;
        };

    public:
        using slot_type   = std::function<T_Ret(T_Params...)>;
        using emit_result = std::conditional_t<std::is_same_v<T_Ret, void>,
                                               void,
                                               std::vector<T_Ret>>;


        signal() = default;
        ~signal() { clear(); }

        signal(const signal &)                     = delete;
        auto operator=(const signal &) -> signal & = delete;


        signal(signal &&other) noexcept
        {
            std::scoped_lock lock(other.m_mutex);
            m_slots   = std::move(other.m_slots);
            m_enabled = other.m_enabled;
        }


        auto
        operator=(signal &&other) noexcept -> signal &
        {
            if (this == &other) return *this;

            std::scoped_lock lock(m_mutex, other.m_mutex);
            m_slots   = std::move(other.m_slots);
            m_enabled = other.m_enabled;
            return *this;
        }


        auto
        connect(slot_type slot) -> connection
        {
            std::scoped_lock lock(m_mutex);

            size_t id { m_next_id++ };
            m_slots.emplace_back(slot, id);

            return connection(
                [this, id] -> auto
                {
                    std::scoped_lock lock { m_mutex };
                    m_slots.remove_if([id](auto &s) -> auto
                                      { return s.id == id; });
                });
        }


        template <typename T_Func, typename... T_FuncParams>
        auto
        connect(T_Func &&f, T_FuncParams &&...extra) -> connection
        {
            auto bound_slot { [f = std::forward<T_Func>(f),
                               extra...](T_Params... params) mutable -> T_Ret
                              { return f(params..., extra...); } };

            return connect(slot_type { bound_slot });
        }


        template <typename T_Instance,
                  typename T_Method,
                  typename... T_MethodParams>
        auto
        connect(T_Instance *instance,
                T_Method  &&mem_func,
                T_MethodParams &&...extra) -> connection
        {
            auto bound_slot {
                [instance, mem_func = std::forward<T_Method>(mem_func),
                 extra...](T_Params... params) mutable -> T_Ret
                { return (instance->*mem_func)(params..., extra...); }
            };
            return connect(slot_type { bound_slot });
        }


        template <typename T_Instance,
                  typename T_Method,
                  typename... T_MethodParams>
        auto
        connect(const T_Instance *instance,
                T_Method        &&mem_func,
                T_MethodParams &&...extra) -> connection
        {
            auto bound_slot {
                [instance, mem_func = std::forward<T_Method>(mem_func),
                 extra...](T_Params... params) mutable -> T_Ret
                { return (instance->*mem_func)(params..., extra...); }
            };

            return connect(slot_type { bound_slot });
        }


        auto
        emit(T_Params &&...params) -> emit_result
        {
            std::scoped_lock lock(m_mutex);

            if (!m_enabled)
            {
                if constexpr (!std::is_same_v<T_Ret, void>)
                    return {};
                else
                    return;
            }

            if constexpr (std::is_same_v<T_Ret, void>)
            {
                for (auto &slot : m_slots)
                    slot.slot(std::forward<T_Params>(params)...);
            }
            else
            {
                std::vector<T_Ret> results;
                results.reserve(m_slots.size());

                for (auto &slot : m_slots)
                    results.emplace_back(
                        slot.slot(std::forward<T_Params>(params)...));

                return results;
            }
        }


        auto
        operator()(T_Params &&...params) -> emit_result
        {
            return emit(std::forward<T_Params>(params)...);
        }


        template <typename T_Pred>
        auto
        emit_until(T_Pred pred, T_Params... params) -> std::optional<T_Ret>
            requires(!std::is_same_v<T_Ret, void>)
        {
            std::scoped_lock lock(m_mutex);

            if (!m_enabled) return std::nullopt;

            for (auto &slot : m_slots)
            {
                T_Ret r { slot.slot(params...) };
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
        get_slots() const -> std::list<slot_wrapper>
        {
            return m_slots;
        }


    private:
        mutable std::mutex       m_mutex;
        std::list<slot_wrapper>  m_slots;
        bool                     m_enabled { true };
        std::atomic<std::size_t> m_next_id { 0 };
    };
}

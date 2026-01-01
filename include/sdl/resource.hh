#pragma once


namespace sdl
{
    template <typename T_SDLObject, void (*T_Destructor)(T_SDLObject *)>
    class resource
    {
    public:
        constexpr resource() : m_object { nullptr } {}

        explicit resource(T_SDLObject *object) : m_object(object) {}

        ~resource()
        {
            if (m_object) T_Destructor(m_object);
        }

        resource(const resource &)                     = delete;
        auto operator=(const resource &) -> resource & = delete;


        resource(resource &&other) noexcept : m_object(other.m_object)
        {
            other.m_object = nullptr;
        }


        auto
        operator=(resource &&other) noexcept -> resource &
        {
            if (this != &other)
            {
                if (m_object) T_Destructor(m_object);
                m_object       = other.m_object;
                other.m_object = nullptr;
            }
            return *this;
        }


        [[nodiscard]]
        auto
        get() -> T_SDLObject *
        {
            return m_object;
        }


        [[nodiscard]]
        auto
        get() const -> T_SDLObject *
        {
            return m_object;
        }


        explicit
        operator bool() const noexcept
        {
            return m_object != nullptr;
        }


        explicit
        operator T_SDLObject *() const noexcept
        {
            return m_object;
        }

    protected:
        T_SDLObject *m_object;
    };
}

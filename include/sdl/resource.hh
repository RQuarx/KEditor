#pragma once


namespace sdl
{
    /**
     * @brief A lifetime manager for SDL objects.
     *
     * @tparam T_SDLObject  The SDL object to manage.
     * @tparam T_Destructor The `SDL_Destroy*` function for the object.
     */
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
        raw() -> T_SDLObject *
        {
            return m_object;
        }


        [[nodiscard]]
        auto
        raw() const -> T_SDLObject *
        {
            return m_object;
        }


        [[nodiscard]]
        auto
        operator!() const -> bool
        {
            return m_object == nullptr;
        }

    protected:
        T_SDLObject *m_object;
    };
}

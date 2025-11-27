#ifndef _KEDITOR_SDL_RESOURCE_HH
#define _KEDITOR_SDL_RESOURCE_HH


namespace sdl
{
    template <typename T_SDLObject, void (*T_Destructor)(T_SDLObject *)>
    class Resource
    {
    public:
        Resource() : m_object(nullptr) {}

        explicit Resource(T_SDLObject *object) : m_object(object) {}

        ~Resource()
        {
            if (m_object) T_Destructor(m_object);
        }

        Resource(const Resource &)                     = delete;
        auto operator=(const Resource &) -> Resource & = delete;


        Resource(Resource &&other) noexcept : m_object(other.m_object)
        {
            other.m_object = nullptr;
        }


        auto
        operator=(Resource &&other) noexcept -> Resource &
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

#endif /* _KEDITOR_SDL_RESOURCE_HH */

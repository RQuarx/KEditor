#ifndef _KEDITOR_WIDGETS_HOVERABLE_HH
#define _KEDITOR_WIDGETS_HOVERABLE_HH
#include "widgets/box.hh"


namespace widget
{
    class Hoverable : public Box
    {
    public:
        static constexpr std::uint8_t BRIGHTNESS_FACTOR { 50 };
        static constexpr std::int64_t FADEOUT_TIME_MS { 500 };


        Hoverable(sdl::FRect                area,
                  sdl::Color                color,
                  std::optional<sdl::Color> border_color = std::nullopt);


        void add_event_callbacks(sdl::EventHandler &handler,
                                 sdl::Renderer     &render) override;


        void render(sdl::Renderer &render) override;

    private:
        sdl::Color m_hovered_color;
        sdl::Color m_current_color;

        std::uint64_t m_prev_frame_time { 0 };

        bool m_hovered;
        bool m_fading_out;

        float m_fade_elapsed;

    protected:
        auto mf_on_mouse_motion(const sdl::Event &event, sdl::Renderer &render)
            -> sdl::EventReturnType;


        void mf_render_fade(sdl::Renderer &render);
    };
}

#endif /* _KEDITOR_WIDGETS_HOVERABLE_HH */

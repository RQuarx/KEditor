#ifndef _KEDITOR_WIDGETS_LABEL_HH
#define _KEDITOR_WIDGETS_LABEL_HH
#include "sdl/text.hh"
#include "widgets/box.hh"


namespace widget
{
    class Label : public Box
    {
    public:
        Label(sdl::FPoint                position,
              std::string                string,
              sdl::Color                 background_color,
              sdl::Color                 text_color,
              std::shared_ptr<sdl::Font> font);


        auto render(sdl::Renderer &render) -> bool override;

    private:
        sdl::Color m_text_color;

        std::string m_string;
        sdl::Text   m_text;

        sdl::FSize m_string_size;

        std::shared_ptr<sdl::Font> m_font;
    };
}

#endif /* _KEDITOR_WIDGETS_LABEL_HH */

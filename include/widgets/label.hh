#pragma once
#include "sdl/text.hh"
#include "widgets/base.hh"


namespace widget
{
    class label : public base
    {
    public:
        label(sdl::fpoint                       position,
              const std::shared_ptr<sdl::font> &font,
              std::string                       string);

        auto render(sdl::renderer &render) -> label & override;


        auto set_font(const std::shared_ptr<sdl::font> &font) noexcept
            -> label &;
        auto set_text_color(sdl::color color) noexcept -> label &;
        auto set_string(std::string string) noexcept -> label &;


        [[nodiscard]] auto get_font() const noexcept
            -> std::shared_ptr<sdl::font>;
        [[nodiscard]] auto get_text_color() const noexcept -> sdl::color;
        [[nodiscard]] auto get_string() const noexcept -> std::string;

    protected:
        sdl::color  mp_text_color;
        std::string mp_string;
        sdl::text   mp_text;

        std::shared_ptr<sdl::font> mp_font;

    private:
        using base::set_rect;
        using base::set_size;
    };
}

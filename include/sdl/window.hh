#pragma once
#include <cstdint>
#include <optional>
#include <string>

#include <SDL3/SDL_video.h>

#include "sdl/resource.hh"


namespace sdl
{
    struct window_size
    {
        int w;
        int h;
    };


    class window : public resource<SDL_Window, SDL_DestroyWindow>
    {
    public:
        using resource::resource;


        [[nodiscard]]
        window(const std::string &title, std::uint64_t flags);


        [[nodiscard]]
        auto get_size() -> std::optional<window_size>;


        /**
         * @brief Sets the state of the text input.
         *
         * @param state The state to set text input state to.
         *              True to start, and false to stop.
         */
        void set_text_input_state(bool state);


        [[nodiscard]]
        auto get_text_input_state() const noexcept -> bool;
    };

}

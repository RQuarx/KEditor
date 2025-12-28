#include "editor/workspace/buffer.hh"

#define DOMAIN "editor::workspace::buffer"

using kei::log_level;


buffer::buffer(kei::config &config, kei::logger &logger, model::piece_table pt)
    : m_config { config }, m_logger { logger }, m_buffer { std::move(pt) }
{
}


auto
buffer::open_file(std::filesystem::path path) -> bool
{
    m_logger[log_level::info, DOMAIN]("Opening a new file {}", path.string());

    if (!std::filesystem::exists(path))
    {
        m_logger[log_level::error, DOMAIN](
            "Tried opening a non-existent path {}", path.string());
        return false;
    }

    if (!std::filesystem::is_regular_file(path))
    {
        m_logger[log_level::error, DOMAIN]("Tried opening a non-regular file");
        return false;
    }

    m_file_path = std::move(path);

    m_texture_cache.clear();

    try
    {
        m_buffer = model::piece_table::from_file(path);
    }
    catch (const kei::exception &e)
    {
        m_logger[log_level::error, DOMAIN]("Failed to open file {}: {}",
                                           m_file_path.string(), e.what());
        return false;
    }

    return true;
}


auto
buffer::set_rect(sdl::frect rect) -> bool
{
    sdl::color color;

    try
    {
        color = sdl::color { m_config.get<std::string>("editor.color.bg") };
    }
    catch (const kei::exception &e)
    {
        m_logger[log_level::error, DOMAIN](
            "Invalid color on \"editor.color.bg\"");
        return false;
    }

    m_box = std::make_unique<widget::box>(rect, color);
    return true;
}


auto
buffer::get_rect() const noexcept -> widget::box *
{
    return m_box.get();
}


auto
buffer::add_event_callbacks(sdl::event_handler &handler) -> bool
{
    handler.connect(SDL_EVENT_MOUSE_BUTTON_DOWN, this,
                    &buffer::mf_on_mouse_button_down);
    return true;
}


auto
buffer::render(sdl::renderer &render) -> bool
{
    return true;
}


auto
buffer::mf_on_mouse_button_down(const sdl::event &event, sdl::renderer &render)
    -> sdl::event_return_type
{
    sdl::fpoint click_position { event.button.x, event.button.y };

    m_focused = sdl::is_point_in_rect(m_box->get_area(false), click_position);

    try
    {
        render.get_window().set_text_input_state(m_focused);
    }
    catch (const kei::exception &e)
    {
        m_logger[log_level::error, DOMAIN]("Failed to set text input state: {}",
                                           e.what());
        return sdl::event_return_type::FAILURE;
    }

    return sdl::event_return_type::CONTINUE;
}

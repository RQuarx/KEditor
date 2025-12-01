#include "editor/workspace/buffer.hh"

using enum LogLevel;


Buffer::Buffer(Config *&config, Logger *&logger, model::PieceTable pt)
    : m_config { config }, m_logger { logger }, m_buffer { std::move(pt) }
{
}


auto
Buffer::open_file(std::filesystem::path path) -> bool
{
    m_logger->log<INFO>("Opening a new file {}", path.string());

    if (!std::filesystem::exists(path))
    {
        m_logger->log<ERROR>("Tried opening a non-existent path {}",
                             path.string());
        return false;
    }

    if (!std::filesystem::is_regular_file(path))
    {
        m_logger->log<ERROR>("Tried opening a non-regular file");
        return false;
    }

    m_file_path = std::move(path);

    m_texture_cache.clear();

    try
    {
        m_buffer = model::PieceTable::from_file(path);
    }
    catch (const kei::Exception &e)
    {
        m_logger->log<ERROR>("Failed to open file {}: {}", m_file_path.string(),
                             e.what());

        return false;
    }

    return true;
}


auto
Buffer::set_rect(sdl::FRect rect) -> bool
{
    sdl::Color color;

    try
    {
        color = sdl::Color { m_config->get<std::string>("editor.color.bg") };
    }
    catch (const kei::Exception &e)
    {
        m_logger->log<ERROR>("Invalid color on \"editor.color.bg\"");
        return false;
    }

    m_box = std::make_unique<widget::Box>(rect, color);
    return true;
}


auto
Buffer::get_rect() const noexcept -> widget::Box *
{
    return m_box.get();
}


auto
Buffer::add_event_callbacks(sdl::EventHandler &handler) -> bool
{
    handler.connect(SDL_EVENT_MOUSE_BUTTON_DOWN, this,
                    &Buffer::mf_on_mouse_button_down);
    return true;
}


auto
Buffer::render(sdl::Renderer &render) -> bool
{
    return true;
}


auto
Buffer::mf_on_mouse_button_down(const sdl::Event &event, sdl::Renderer &render)
    -> sdl::EventReturnType
{
    sdl::FPoint click_position { event.button.x, event.button.y };

    m_focused = sdl::is_point_in_rect(m_box->get_area(false), click_position);

    try
    {
        render.get_window().set_text_input_state(m_focused);
    }
    catch (const kei::Exception &e)
    {
        m_logger->log<ERROR>("Failed to set text input state: {}", e.what());
        return sdl::EventReturnType::FAILURE;
    }

    return sdl::EventReturnType::CONTINUE;
}

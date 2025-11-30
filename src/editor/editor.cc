#include "editor/editor.hh"

using enum LogLevel;


auto
Editor::create(Config          *&config,
               Logger          *&logger,
               model::PieceTable pt,
               sdl::Renderer   *&render) -> std::optional<Editor>
{
    if (render == nullptr) return std::nullopt;

    Editor editor;

    editor.m_config = config;
    editor.m_logger = logger;
    editor.m_buffer = std::move(pt);
    editor.m_render = render;

    return editor;
}


auto
Editor::open_file(std::string path) -> bool
{
    m_logger->log<INFO>("Opening a new file {}", path);

    m_file_path = std::move(path);

    m_texture_cache.clear();

    try
    {
        m_buffer = model::PieceTable::from_file(path);
    }
    catch (const kei::Exception &e)
    {
        m_logger->log<ERROR>("Failed to open file {}: {}", m_file_path,
                             e.what());

        return false;
    }

    return true;
}


auto
Editor::set_rect(sdl::FRect rect) -> bool
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
Editor::add_event_callbacks(sdl::EventHandler &handler) -> bool
{
    handler.connect(SDL_EVENT_MOUSE_BUTTON_DOWN, this,
                    &Editor::mf_on_mouse_button_down);
    return true;
}


auto
Editor::render() -> bool
{
    return true;
}


auto
Editor::mf_on_mouse_button_down(const sdl::Event &event, sdl::Renderer &render)
    -> sdl::EventReturnType
{
    sdl::FPoint click_position { event.button.x, event.button.y };

    m_focused = sdl::is_point_in_rect(m_box->get_area(false), click_position);

    if (!m_focused) return sdl::EventReturnType::CONTINUE;
}

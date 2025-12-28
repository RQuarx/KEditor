#pragma once
#include "config.hh"
#include "logger.hh"
#include "sdl/event.hh"
#include "sdl/renderable.hh"
#include "sdl/renderer.hh"


class workspace : sdl::renderable<bool>
{
public:
    workspace(kei::config &config, kei::logger &logger);


    auto open_path(std::filesystem::path path) -> bool;


    auto add_event_callbacks(sdl::event_handler &handler) -> bool override;
    auto render(sdl::renderer &render) -> bool override;


private:
    kei::config &m_config;
    kei::logger &m_logger;

    std::vector<std::filesystem::path> m_paths;
};

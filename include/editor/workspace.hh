#ifndef _KEDITOR_EDITOR_WORKSPACE_H
#define _KEDITOR_EDITOR_WORKSPACE_H
#include "config.hh"
#include "logger.hh"
#include "sdl/event.hh"
#include "sdl/renderer.hh"


class Workspace
{
public:
    Workspace(Config *&config, Logger *&logger);


    auto open_path(std::filesystem::path path) -> bool;


    auto add_event_callbacks(sdl::EventHandler &handler) -> bool;
    auto render(sdl::Renderer &render) -> bool;


private:
    Config *m_config;
    Logger *m_logger;

    std::vector<std::filesystem::path> m_paths;
};


#endif /* _KEDITOR_EDITOR_WORKSPACE_H */

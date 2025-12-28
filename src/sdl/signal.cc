#include "sdl/signal.hh"

using sdl::connection;


connection::connection(connection::signature disconnect_function)
    : m_disconnect_fn(std::move(disconnect_function))
{
}


connection::~connection() { disconnect(); }


void
connection::disconnect()
{
    if (m_disconnect_fn != nullptr)
    {
        m_disconnect_fn();
        m_disconnect_fn = nullptr;
    }
}

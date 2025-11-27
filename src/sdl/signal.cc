#include "sdl/signal.hh"

using sdl::Connection;
using sdl::Signal;


Connection::Connection(Connection::signature disconnect_function)
    : m_disconnect_fn(std::move(disconnect_function))
{
}


Connection::~Connection() { disconnect(); }


void
Connection::disconnect()
{
    if (m_disconnect_fn != nullptr)
    {
        m_disconnect_fn();
        m_disconnect_fn = nullptr;
    }
}

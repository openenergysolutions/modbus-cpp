#include "server/channel/AsioServer.h"

#include <algorithm>

using namespace std::placeholders;

namespace modbus
{

AsioServer::AsioServer(std::shared_ptr<Logger> logger,
                       std::shared_ptr<exe4cpp::StrandExecutor> executor,
                       const Ipv4Endpoint& endpoint,
                       unsigned int max_connections)
    : m_logger{logger},
      m_executor{executor},
      m_max_connections{max_connections},
      m_tcp_acceptor{*m_executor->get_context(),
        asio::ip::tcp::endpoint{asio::ip::address::from_string(endpoint.get_hostname()), static_cast<unsigned short>(endpoint.get_port())}},
      m_is_shutdown{false},
      m_num_connections{0}
{
}

void AsioServer::start(std::shared_ptr<IServerConnectionListenerBuilder> connection_listener_builder)
{
    m_connection_listener_builder = connection_listener_builder;
    start_accept();
}

void AsioServer::shutdown()
{
    m_executor->post([this, self=shared_from_this()] {
        m_is_shutdown = true;

        m_tcp_acceptor.close();

        auto it = m_connections.begin();
        while(it != m_connections.end())
        {
            (*it)->close();
            ++it;
        }
        m_connections.clear();
    });
}

void AsioServer::remove_connection(std::shared_ptr<AsioServerTcpConnection> connection)
{
    auto it = std::find(m_connections.begin(), m_connections.end(), connection);
    if(it != m_connections.end())
    {
        (*it)->close();
        m_connections.erase(it);
        --m_num_connections;
    }
}

size_t AsioServer::get_num_connections() const
{
    return m_num_connections;
}

void AsioServer::start_accept()
{
    if(m_is_shutdown) return;

    m_executor->post([=, self=shared_from_this()] {
        if(m_is_shutdown) return;

        auto connection = std::make_shared<AsioServerTcpConnection>(m_logger, m_executor, std::dynamic_pointer_cast<AsioServer>(shared_from_this()));
        m_connections.push_back(connection);
        m_tcp_acceptor.async_accept(connection->get_socket(),
                                    m_executor->wrap(
                                        std::bind(&AsioServer::handle_accept,
                                        std::dynamic_pointer_cast<AsioServer>(shared_from_this()),
                                        connection, _1)
                                    ));
    });
}

void AsioServer::handle_accept(std::shared_ptr<AsioServerTcpConnection> connection, const asio::error_code& ec)
{
    if(m_is_shutdown) return;

    if(ec)
    {
        remove_connection(connection);
        return;
    }

    // Kill an old connection if there are two many open connections
    ++m_num_connections;
    if(m_num_connections > m_max_connections)
    {
        remove_connection(m_connections.front());
    }

    connection->set_listener(std::shared_ptr<IConnectionListener>(std::move(m_connection_listener_builder->build(connection))));
    connection->start();

    // Loop to continue accepting connections
    start_accept();
}

} // namespace modbus

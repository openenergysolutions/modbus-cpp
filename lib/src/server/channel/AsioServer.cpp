#include "server/channel/AsioServer.h"

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
      m_tcp_acceptor{*m_executor->get_service(),
        asio::ip::tcp::endpoint{asio::ip::address::from_string(endpoint.get_hostname()), static_cast<unsigned short>(endpoint.get_port())}}
{
}

void AsioServer::start(std::shared_ptr<IServerConnectionListenerBuilder> connection_listener_builder)
{
    m_connection_listener_builder = connection_listener_builder;
    start_accept();
}

void AsioServer::shutdown()
{

}

void AsioServer::start_accept()
{
    if(m_connections.size() < m_max_connections)
    {
        auto connection = std::make_shared<AsioServerTcpConnection>(m_logger, m_executor);
        m_connections.push_back(connection);
        m_tcp_acceptor.async_accept(connection->get_socket(),
                                    std::bind(&AsioServer::handle_accept,
                                    std::dynamic_pointer_cast<AsioServer>(shared_from_this()),
                                    connection, _1));
    }
}

void AsioServer::handle_accept(std::shared_ptr<AsioServerTcpConnection> connection, const asio::error_code& ec)
{
    if(ec)
    {
        // TODO: do something!
        return;
    }

    connection->set_listener(std::shared_ptr<IConnectionListener>(std::move(m_connection_listener_builder->build(connection))));
    connection->start();

    // Loop to continue accepting connections
    start_accept();
}

} // namespace modbus

#include "channel/AsioTcpConnection.h"

#include <functional>
#include "channel/IConnectionListener.h"

using namespace std::placeholders;

namespace modbus
{

AsioTcpConnection::AsioTcpConnection(std::shared_ptr<Logger> logger,
                                     std::shared_ptr<asio::io_service> io_service,
                                     asio::strand strand,
                                     const Ipv4Endpoint& endpoint)
        : m_logger{logger},
          m_ip_endpoint{endpoint},
          m_strand{strand},
          m_resolver{*io_service},
          m_tcp_socket{*io_service},
          m_current_connection_status{ConnectionStatus::NotConnected}
{

}

void AsioTcpConnection::set_listener(std::weak_ptr<IConnectionListener> listener)
{
    m_connection_listener = listener;
}

void AsioTcpConnection::send(const loopser::rseq_t& data)
{
    m_write_buffer = std::make_unique<loopser::Buffer>(data);

    if(m_current_connection_status == ConnectionStatus::NotConnected)
    {
        m_logger->info("Establishing connection to {}:{}", m_ip_endpoint.get_hostname(), m_ip_endpoint.get_port());

        m_current_connection_status = ConnectionStatus::Connecting;
        asio::ip::tcp::resolver::query query{m_ip_endpoint.get_hostname(), std::to_string(m_ip_endpoint.get_port())};
        m_resolver.async_resolve(query,
                                 m_strand.wrap(std::bind(&AsioTcpConnection::resolve_handler,
                                                         std::dynamic_pointer_cast<AsioTcpConnection>(shared_from_this()),
                                                         _1, _2)));
    }

    if(m_current_connection_status == ConnectionStatus::Connected)
    {
        send_buffer();
    }
}

void AsioTcpConnection::close()
{
    m_strand.dispatch([=, self = shared_from_this()] () {
        m_current_connection_status = ConnectionStatus::NotConnected;

        if(m_tcp_socket.is_open())
        {
            m_logger->info("Closing connection");

            std::error_code ec;
            m_tcp_socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
            m_tcp_socket.close(ec);
        }
    });
}

void AsioTcpConnection::resolve_handler(const asio::error_code& ec, asio::ip::tcp::resolver::iterator endpoints)
{
    if(ec)
    {
        m_logger->error("IP resolver error: {}", ec.message());
        m_current_connection_status = ConnectionStatus::NotConnected;
        send_error();
        return;
    }

    asio::async_connect(m_tcp_socket,
                        endpoints,
                        m_strand.wrap(std::bind(&AsioTcpConnection::connect_handler,
                                                std::dynamic_pointer_cast<AsioTcpConnection>(shared_from_this()),
                                                _1)));
}

void AsioTcpConnection::connect_handler(const asio::error_code& ec)
{
    if(ec)
    {
        m_logger->error("Connection error: {}", ec.message());
        close();
        send_error();
        return;
    }

    m_logger->info("Connection established");
    m_current_connection_status = ConnectionStatus::Connected;
    send_buffer();

    begin_read();
}

void AsioTcpConnection::read_handler(const asio::error_code& ec, std::size_t bytes_transferred)
{
    if(ec)
    {
        m_logger->error("Read error: {}", ec.message());
        close();
        send_error();
        return;
    }

    m_logger->info("Received {} bytes", bytes_transferred);

    auto connection_listener = m_connection_listener.lock();
    if(connection_listener)
    {
        connection_listener->on_receive(loopser::rseq_t{m_read_buffer.data(), (unsigned int) bytes_transferred});
    }

    begin_read();
}

void AsioTcpConnection::write_handler(const std::error_code& ec, std::size_t bytes_transferred)
{
    if(ec)
    {
        m_logger->error("Write error: {}", ec.message());
        close();
        send_error();
        return;
    }

    m_logger->info("Sent {} bytes", bytes_transferred);

    m_write_buffer = nullptr;

    auto connection_listener = m_connection_listener.lock();
    if(connection_listener)
    {
        connection_listener->on_write_done();
    }
}

void AsioTcpConnection::begin_read()
{
    m_tcp_socket.async_read_some(asio::buffer(m_read_buffer, m_read_buffer.size()),
                                 m_strand.wrap(std::bind(&AsioTcpConnection::read_handler,
                                                         std::dynamic_pointer_cast<AsioTcpConnection>(shared_from_this()),
                                                         _1, _2)));
}

void AsioTcpConnection::send_buffer()
{
    if(m_write_buffer)
    {
        m_tcp_socket.async_send(asio::buffer(m_write_buffer->as_rslice(), m_write_buffer->length()),
                                m_strand.wrap(std::bind(&AsioTcpConnection::write_handler,
                                                        std::dynamic_pointer_cast<AsioTcpConnection>(shared_from_this()),
                                                        _1, _2)));
    }
}

void AsioTcpConnection::send_error()
{
    auto connection_listener = m_connection_listener.lock();
    if(connection_listener)
    {
        connection_listener->on_error();
    }
}

} // namespace modbus

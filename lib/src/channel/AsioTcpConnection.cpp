#include "channel/AsioTcpConnection.h"

#include "channel/ConnectionListener.h"

namespace modbus
{

AsioTcpConnection::AsioTcpConnection(std::shared_ptr<asio::io_service> io_service, asio::strand strand, const Ipv4Endpoint& endpoint)
: m_ip_endpoint{endpoint},
  m_strand{strand},
  m_resolver{*io_service},
  m_tcp_socket{*io_service},
  m_current_connection_status{ConnectionStatus::NotConnected},
  m_connection_listener{nullptr}
{

}

void AsioTcpConnection::set_listener(ConnectionListener* listener)
{
    m_connection_listener = listener;
}

void AsioTcpConnection::send(const openpal::rseq_t& data)
{
    m_write_buffer = std::make_unique<openpal::Buffer>(data);

    if(m_current_connection_status == ConnectionStatus::NotConnected)
    {
        m_current_connection_status = ConnectionStatus::Connecting;
        asio::ip::tcp::resolver::query q{m_ip_endpoint.get_hostname(), std::to_string(m_ip_endpoint.get_port())};

        auto cb = [=, self = shared_from_this()](const std::error_code& ec, asio::ip::tcp::resolver::iterator it)
        {
            resolve_handler(ec, it);
        };
        m_resolver.async_resolve(q, m_strand.wrap(cb));
    }

    if(m_current_connection_status == ConnectionStatus::Connected)
    {
        send_buffer();
    }
}

void AsioTcpConnection::close()
{
    //if(m_tcp_socket.is_open())
    //{
        m_current_connection_status = ConnectionStatus::NotConnected;
        //m_tcp_socket.shutdown(asio::ip::tcp::socket::shutdown_both);
        std::error_code ec;
        m_tcp_socket.close(ec);
    //}
}

void AsioTcpConnection::resolve_handler(const std::error_code& ec, asio::ip::tcp::resolver::iterator it)
{
    if(ec)
    {
        m_current_connection_status = ConnectionStatus::NotConnected;
        send_error();
        return;
    }

    auto cb = [=, self = shared_from_this()](const std::error_code& ec)
    {
        connect_handler(ec);
    };
    m_tcp_socket.async_connect(it->endpoint(), m_strand.wrap(cb));
}

void AsioTcpConnection::connect_handler(const std::error_code& ec)
{
    if(ec)
    {
        m_current_connection_status = ConnectionStatus::NotConnected;
        send_error();
        return;
    }

    m_current_connection_status = ConnectionStatus::Connected;
    send_buffer();

    auto cb = [=, self = shared_from_this()](const std::error_code& ec, std::size_t bytes_t)
    {
        read_handler(ec, bytes_t);
    };
    m_tcp_socket.async_read_some(asio::buffer(m_read_buffer, m_read_buffer.size()), m_strand.wrap(cb));
}

void AsioTcpConnection::read_handler(const std::error_code& ec, std::size_t bytes_transferred)
{
    if(ec)
    {
        m_current_connection_status = ConnectionStatus::NotConnected;
        send_error();
        return;
    }

    if(m_connection_listener)
    {
        m_connection_listener->on_receive(openpal::rseq_t{m_read_buffer.data(), (unsigned int)bytes_transferred});
    }

    auto cb = [=, self = shared_from_this()](const std::error_code& ec, std::size_t bytes_transferred)
    {
        read_handler(ec, bytes_transferred);
    };
    m_tcp_socket.async_read_some(asio::buffer(m_read_buffer, m_read_buffer.size()), m_strand.wrap(cb));
}

void AsioTcpConnection::write_handler(const std::error_code& ec, std::size_t bytes_transferred)
{
    if(ec)
    {
        m_current_connection_status = ConnectionStatus::NotConnected;
        send_error();
        return;
    }

    m_write_buffer = nullptr;
}

void AsioTcpConnection::send_buffer()
{
    if(m_write_buffer)
    {
        auto cb = [=, self = shared_from_this()](const std::error_code& ec, std::size_t bytes_transferred)
        {
            write_handler(ec, bytes_transferred);
        };
        m_tcp_socket.async_send(asio::buffer(m_write_buffer->as_rslice(), m_write_buffer->length()), m_strand.wrap(cb));
    }
}

void AsioTcpConnection::send_error()
{
    if(m_tcp_socket.is_open())
    {
        m_tcp_socket.close();
    }

    if(m_connection_listener)
    {
        m_connection_listener->on_error();
    }
}

} // namespace modbus

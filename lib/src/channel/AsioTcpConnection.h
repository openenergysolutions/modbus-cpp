#ifndef MODBUS_ASIOTCPCONNECTION_H
#define MODBUS_ASIOTCPCONNECTION_H

#include <openpal/container/Buffer.h>
#include "channel/ITcpConnection.h"
#include "modbus/Ipv4Endpoint.h"

#include "asio.hpp"

namespace modbus
{

class AsioTcpConnection : public ITcpConnection
{
public:
    AsioTcpConnection(std::shared_ptr<asio::io_service> io_service, asio::strand strand, const Ipv4Endpoint& endpoint);

    void set_listener(ConnectionListener* listener) override;
    void send(const openpal::rseq_t& data) override;
    void close() override;

private:
    enum class ConnectionStatus
    {
        NotConnected,
        Connecting,
        Connected
    };

    void resolve_handler(const std::error_code& ec, asio::ip::tcp::resolver::iterator it);
    void connect_handler(const std::error_code& ec);
    void read_handler(const std::error_code& ec, std::size_t bytes_transferred);
    void write_handler(const std::error_code& ec, std::size_t bytes_transferred);

    void send_buffer();
    void send_error();

    Ipv4Endpoint m_ip_endpoint;
    asio::strand m_strand;
    asio::ip::tcp::resolver m_resolver;
    asio::ip::tcp::socket m_tcp_socket;

    ConnectionStatus m_current_connection_status;
    std::array<uint8_t, 4096> m_read_buffer;
    std::unique_ptr<openpal::Buffer> m_write_buffer;

    ConnectionListener* m_connection_listener;
};

} // namespace modbus

#endif //MODBUS_ASIOTCPCONNECTION_H

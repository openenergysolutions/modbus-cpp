#ifndef MODBUS_ASIOTCPCONNECTION_H
#define MODBUS_ASIOTCPCONNECTION_H

#include <array>
#include "exe4cpp/asio/StrandExecutor.h"
#include "ser4cpp/container/Buffer.h"

#include "modbus/channel/Ipv4Endpoint.h"
#include "logging/Logger.h"
#include "channel/ITcpConnection.h"

namespace modbus
{

class AsioTcpConnection : public ITcpConnection
{
public:
    AsioTcpConnection(std::shared_ptr<Logger> logger,
                      std::shared_ptr<exe4cpp::StrandExecutor> executor,
                      const Ipv4Endpoint& endpoint);

    void set_listener(std::weak_ptr<IConnectionListener> listener) override;
    void send(const ser4cpp::rseq_t& data) override;
    void close() override;

private:
    enum class ConnectionStatus
    {
        NotConnected,
        Connecting,
        Connected
    };

    void resolve_handler(const asio::error_code& ec, asio::ip::tcp::resolver::iterator endpoints);
    void connect_handler(const asio::error_code& ec);
    void read_handler(const asio::error_code& ec, std::size_t bytes_transferred);
    void write_handler(const asio::error_code& ec, std::size_t bytes_transferred);

    void begin_read();
    void send_buffer();
    void send_error(const std::string& message);

    std::shared_ptr<Logger> m_logger;
    Ipv4Endpoint m_ip_endpoint;
    std::shared_ptr<exe4cpp::StrandExecutor> m_executor;
    asio::ip::tcp::resolver m_resolver;
    asio::ip::tcp::socket m_tcp_socket;

    ConnectionStatus m_current_connection_status;
    std::array<uint8_t, 4096> m_read_buffer;
    std::unique_ptr<ser4cpp::Buffer> m_write_buffer;

    std::weak_ptr<IConnectionListener> m_connection_listener;
};

} // namespace modbus

#endif //MODBUS_ASIOTCPCONNECTION_H

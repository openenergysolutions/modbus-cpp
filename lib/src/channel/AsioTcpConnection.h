/*
 * Copyright 2017-2018 Duke Energy Corporation and Open Energy Solutions, Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MODBUS_ASIOTCPCONNECTION_H
#define MODBUS_ASIOTCPCONNECTION_H

#include <array>
#include <string>
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
                      const Ipv4Endpoint& endpoint,
                      const std::string& adapter = "");

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
    std::string m_adapter;
    asio::ip::tcp::resolver m_resolver;
    asio::ip::tcp::socket m_tcp_socket;

    ConnectionStatus m_current_connection_status;
    std::array<uint8_t, 4096> m_read_buffer;
    std::unique_ptr<ser4cpp::Buffer> m_write_buffer;

    std::weak_ptr<IConnectionListener> m_connection_listener;
};

} // namespace modbus

#endif //MODBUS_ASIOTCPCONNECTION_H

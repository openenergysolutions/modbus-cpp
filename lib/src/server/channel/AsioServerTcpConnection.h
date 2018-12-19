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
#ifndef MODBUS_ASIOSERVERTCPCONNECTION_H
#define MODBUS_ASIOSERVERTCPCONNECTION_H

#include <array>
#include "exe4cpp/asio/StrandExecutor.h"
#include "ser4cpp/container/Buffer.h"

#include "modbus/Ipv4Endpoint.h"
#include "ITcpConnection.h"
#include "logging/Logger.h"

namespace modbus
{

class AsioServerTcpConnection : public ITcpConnection
{
public:
    AsioServerTcpConnection(std::shared_ptr<Logger> logger,
                            std::shared_ptr<exe4cpp::StrandExecutor> executor);

    void set_listener(std::weak_ptr<IConnectionListener> listener) override;
    void send(const ser4cpp::rseq_t& data) override;
    void close() override;

    void start();
    asio::ip::tcp::socket& get_socket();

private:
    void connect_handler(const asio::error_code& ec);
    void read_handler(const asio::error_code& ec, std::size_t bytes_transferred);
    void write_handler(const asio::error_code& ec, std::size_t bytes_transferred);

    void begin_read();
    void send_buffer();
    void send_error(const std::string& message);

    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<exe4cpp::StrandExecutor> m_executor;
    asio::ip::tcp::socket m_tcp_socket;

    bool m_is_connected;
    std::array<uint8_t, 4096> m_read_buffer;
    std::unique_ptr<ser4cpp::Buffer> m_write_buffer;

    std::weak_ptr<IConnectionListener> m_connection_listener;
};

} // namespace modbus

#endif //MODBUS_ASIOSERVERTCPCONNECTION_H

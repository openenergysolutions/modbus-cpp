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
#include "client/channel/AsioClientTcpConnection.h"

#include <functional>
#include "IConnectionListener.h"

using namespace std::placeholders;

namespace modbus
{

AsioClientTcpConnection::AsioClientTcpConnection(std::shared_ptr<Logger> logger,
                                                 std::shared_ptr<exe4cpp::StrandExecutor> executor,
                                                 const Ipv4Endpoint& endpoint)
        : m_logger{logger},
          m_ip_endpoint{endpoint},
          m_executor{executor},
          m_resolver{*executor->get_service()},
          m_tcp_socket{*executor->get_service()},
          m_current_connection_status{ConnectionStatus::NotConnected}
{

}

void AsioClientTcpConnection::set_listener(std::weak_ptr<IConnectionListener> listener)
{
    m_connection_listener = listener;
}

void AsioClientTcpConnection::send(const ser4cpp::rseq_t& data)
{
    m_write_buffer = std::make_unique<ser4cpp::Buffer>(data);

    if(m_current_connection_status == ConnectionStatus::NotConnected)
    {
        m_logger->info("Establishing connection to {}:{}", m_ip_endpoint.get_hostname(), m_ip_endpoint.get_port());

        m_current_connection_status = ConnectionStatus::Connecting;
        asio::ip::tcp::resolver::query query{m_ip_endpoint.get_hostname(), std::to_string(m_ip_endpoint.get_port())};
        m_resolver.async_resolve(query,
                                 m_executor->wrap(std::bind(&AsioClientTcpConnection::resolve_handler,
                                                         std::dynamic_pointer_cast<AsioClientTcpConnection>(shared_from_this()),
                                                         _1, _2)));
    }

    if(m_current_connection_status == ConnectionStatus::Connected)
    {
        send_buffer();
    }
}

void AsioClientTcpConnection::close()
{
    m_executor->post([=, self = shared_from_this()] () {
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

void AsioClientTcpConnection::resolve_handler(const asio::error_code& ec, asio::ip::tcp::resolver::iterator endpoints)
{
    if(ec)
    {
        m_logger->error("IP resolver error: {}", ec.message());
        m_current_connection_status = ConnectionStatus::NotConnected;
        send_error(ec.message());
        return;
    }

    asio::async_connect(m_tcp_socket,
                        endpoints,
                        m_executor->wrap(std::bind(&AsioClientTcpConnection::connect_handler,
                                                std::dynamic_pointer_cast<AsioClientTcpConnection>(shared_from_this()),
                                                _1)));
}

void AsioClientTcpConnection::connect_handler(const asio::error_code& ec)
{
    if(ec)
    {
        m_logger->error("Connection error: {}", ec.message());
        close();
        send_error(ec.message());
        return;
    }

    m_logger->info("Connection established");
    m_current_connection_status = ConnectionStatus::Connected;
    send_buffer();

    begin_read();
}

void AsioClientTcpConnection::read_handler(const asio::error_code& ec, std::size_t bytes_transferred)
{
    if(ec)
    {
        m_logger->error("Read error: {}", ec.message());
        close();
        send_error(ec.message());
        return;
    }

    m_logger->debug("Received {} bytes", bytes_transferred);

    auto connection_listener = m_connection_listener.lock();
    if(connection_listener)
    {
        connection_listener->on_receive(ser4cpp::rseq_t{m_read_buffer.data(), (unsigned int) bytes_transferred});
    }

    begin_read();
}

void AsioClientTcpConnection::write_handler(const std::error_code& ec, std::size_t bytes_transferred)
{
    if(ec)
    {
        m_logger->error("Write error: {}", ec.message());
        close();
        send_error(ec.message());
        return;
    }

    m_logger->debug("Sent {} bytes", bytes_transferred);

    m_write_buffer = nullptr;

    auto connection_listener = m_connection_listener.lock();
    if(connection_listener)
    {
        connection_listener->on_write_done();
    }
}

void AsioClientTcpConnection::begin_read()
{
    m_tcp_socket.async_read_some(asio::buffer(m_read_buffer, m_read_buffer.size()),
                                 m_executor->wrap(std::bind(&AsioClientTcpConnection::read_handler,
                                                         std::dynamic_pointer_cast<AsioClientTcpConnection>(shared_from_this()),
                                                         _1, _2)));
}

void AsioClientTcpConnection::send_buffer()
{
    if(m_write_buffer)
    {
        m_tcp_socket.async_send(asio::buffer(m_write_buffer->as_rslice(), m_write_buffer->length()),
                                m_executor->wrap(std::bind(&AsioClientTcpConnection::write_handler,
                                                        std::dynamic_pointer_cast<AsioClientTcpConnection>(shared_from_this()),
                                                        _1, _2)));
    }
}

void AsioClientTcpConnection::send_error(const std::string& message)
{
    auto connection_listener = m_connection_listener.lock();
    if(connection_listener)
    {
        connection_listener->on_error(message);
    }
}

} // namespace modbus

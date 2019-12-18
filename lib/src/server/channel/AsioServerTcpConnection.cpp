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
#include "server/channel/AsioServerTcpConnection.h"

#include <functional>
#include "IConnectionListener.h"
#include "server/channel/AsioServer.h"

using namespace std::placeholders;

namespace modbus
{

AsioServerTcpConnection::AsioServerTcpConnection(std::shared_ptr<Logger> logger,
                                                 std::shared_ptr<exe4cpp::StrandExecutor> executor,
                                                 std::weak_ptr<AsioServer> server)
        : m_logger{logger},
          m_executor{executor},
          m_server{server},
          m_tcp_socket{*executor->get_context()},
          m_is_shutdown{false},
          m_is_connected{false}
{

}

void AsioServerTcpConnection::set_listener(std::shared_ptr<IConnectionListener> listener)
{
    m_connection_listener = listener;
}

void AsioServerTcpConnection::send(const ser4cpp::rseq_t& data)
{
    if(m_is_shutdown) return;

    m_write_buffer = std::make_unique<ser4cpp::Buffer>(data);
    send_buffer();
}

void AsioServerTcpConnection::close()
{
    if(m_is_shutdown) return;

    m_executor->post([=, self=shared_from_this()]() {
        if(m_is_shutdown) return;

        m_is_connected = false;

        if(m_tcp_socket.is_open())
        {
            m_logger->info("Closing connection");

            std::error_code ec;
            m_tcp_socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
            m_tcp_socket.close(ec);
            notify_close();
        }
    });
}

void AsioServerTcpConnection::shutdown()
{
    m_executor->post([=, self = shared_from_this()] () {
        m_is_shutdown = true;
        
        notify_close();

        std::error_code ec;
        m_tcp_socket.cancel(ec);
    });
}

void AsioServerTcpConnection::start()
{
    if(m_is_shutdown) return;

    m_logger->info("New connection established");
    m_is_connected = true;
    send_buffer();

    begin_read();
}

asio::ip::tcp::socket& AsioServerTcpConnection::get_socket()
{
    return m_tcp_socket;
}

void AsioServerTcpConnection::read_handler(const asio::error_code& ec, std::size_t bytes_transferred)
{
    if(m_is_shutdown) return;

    if(ec)
    {
        m_logger->error("Read error: {}", ec.message());
        close();
        send_error(ec.message());
        return;
    }

    m_logger->debug("Received {} bytes", bytes_transferred);

    if(m_connection_listener)
    {
        m_connection_listener->on_receive(ser4cpp::rseq_t{m_read_buffer.data(), (unsigned int) bytes_transferred});
    }

    begin_read();
}

void AsioServerTcpConnection::write_handler(const std::error_code& ec, std::size_t bytes_transferred)
{
    if(m_is_shutdown) return;

    if(ec)
    {
        m_logger->error("Write error: {}", ec.message());
        close();
        send_error(ec.message());
        return;
    }

    m_logger->debug("Sent {} bytes", bytes_transferred);

    m_write_buffer = nullptr;

    if(m_connection_listener)
    {
        m_connection_listener->on_write_done();
    }
}

void AsioServerTcpConnection::begin_read()
{
    if(m_is_shutdown) return;

    if(m_is_connected)
    {
        m_tcp_socket.async_read_some(asio::buffer(m_read_buffer, m_read_buffer.size()),
                                    m_executor->wrap(std::bind(&AsioServerTcpConnection::read_handler,
                                                               std::dynamic_pointer_cast<AsioServerTcpConnection>(shared_from_this()),
                                                               _1, _2)));
    }
}

void AsioServerTcpConnection::send_buffer()
{
    if(m_is_shutdown) return;

    if(m_write_buffer && m_is_connected)
    {
        m_tcp_socket.async_send(asio::buffer(m_write_buffer->as_rslice(), m_write_buffer->length()),
                                m_executor->wrap(std::bind(&AsioServerTcpConnection::write_handler,
                                                        std::dynamic_pointer_cast<AsioServerTcpConnection>(shared_from_this()),
                                                        _1, _2)));
    }
}

void AsioServerTcpConnection::send_error(const std::string& message)
{
    if(m_is_shutdown) return;

    if(m_connection_listener)
    {
        m_connection_listener->on_error(message);
    }

    notify_close();
}

void AsioServerTcpConnection::notify_close()
{
    auto server = m_server.lock();
    if(server)
    {
        server->remove_connection(std::dynamic_pointer_cast<AsioServerTcpConnection>(shared_from_this()));
    }
}

} // namespace modbus

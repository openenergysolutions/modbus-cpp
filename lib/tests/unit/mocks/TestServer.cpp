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
#include "mocks/TestServer.h"

TestServer::TestServer(unsigned short port, exe4cpp::duration_t timeout)
        : m_timeout{timeout},
          m_endpoint{asio::ip::tcp::endpoint{asio::ip::tcp::v4(), port}},
          m_io_service{std::make_shared<asio::io_service>()},
          m_acceptor{std::make_shared<asio::ip::tcp::acceptor>(*m_io_service)},
          m_pending_start{false},
          m_pending_stop{false},
          m_pending_connection{false},
          m_num_connections{0},
          m_pending_close_connection{false},
          m_buffer_size{0},
          m_thread_pool{m_io_service, 1}
{

}

TestServer::~TestServer()
{
    stop();
}

void TestServer::start()
{
    m_pending_start = false;

    m_io_service->post([=, acceptor = m_acceptor]() {
        m_acceptor->open(m_endpoint.protocol());
        m_acceptor->set_option(asio::ip::tcp::acceptor::reuse_address(true));
        m_acceptor->bind(m_endpoint);
        m_acceptor->listen();

        begin_accept();

        // Advertise the operation is finished
        std::lock_guard<std::mutex> lock(m_start_lock);
        m_pending_start = true;
        m_start_cv.notify_all();
    });

    // Wait until operation is finished
    std::unique_lock<std::mutex> lock(m_start_lock);
    m_start_cv.wait(lock, [=]() { return m_pending_start; });
}

void TestServer::stop()
{
    m_pending_stop = false;

    m_io_service->post([=, socket = m_current_socket, acceptor = m_acceptor]() {
        close_socket();
        close_acceptor();

        // Advertise the operation is finished
        std::lock_guard<std::mutex> lock(m_stop_lock);
        m_pending_stop = true;
        m_stop_cv.notify_all();
    });

    // Wait until operation is finished
    std::unique_lock<std::mutex> lock(m_stop_lock);
    m_stop_cv.wait(lock, [=]() { return m_pending_stop; });
}

void TestServer::send(const ser4cpp::rseq_t& data)
{
    m_io_service->post([=, socket = m_current_socket]() {
        m_current_socket->write_some(asio::buffer(data, data.length()));
    });
}

bool TestServer::wait_for_connection()
{
    std::unique_lock<std::mutex> lock(m_connection_lock);

    auto result = m_connection_cv.wait_for(lock, m_timeout, [=]() { return m_pending_connection; });
    m_pending_connection = false;

    return result;
}

unsigned int TestServer::get_num_connections()
{
    std::lock_guard<std::mutex> lock(m_connection_lock);
    return m_num_connections;
}

bool TestServer::wait_for_close_connection()
{
    std::unique_lock<std::mutex> lock(m_close_connection_lock);

    auto result = m_close_connection_cv.wait_for(lock, m_timeout, [=]() { return m_pending_close_connection; });
    m_pending_close_connection = false;

    return result;
}

bool TestServer::wait_for_data()
{
    std::unique_lock<std::mutex> lock(m_data_lock);

    auto result = m_data_cv.wait_for(lock, m_timeout, [=]() { return m_buffer_size > 0; });
    m_buffer_size = 0;

    return result;
}

void TestServer::begin_accept()
{
    auto new_socket = std::make_shared<asio::ip::tcp::socket>(*m_io_service);
    m_acceptor->async_accept(*new_socket, [=, acceptor = m_acceptor](const asio::error_code& ec) { accept_handler(ec, new_socket); });
}

void TestServer::begin_read()
{
    m_current_socket->async_read_some(asio::buffer(m_data_buffer.data(), m_data_buffer.size()),
                                      [=, socket = m_current_socket](const asio::error_code& ec, std::size_t bytes_transferred) {
        read_handler(ec, bytes_transferred);
    });
}

void TestServer::close_acceptor()
{
    if(m_acceptor->is_open())
    {
        asio::error_code ec;
        m_acceptor->close(ec);
    }
}

void TestServer::close_socket()
{
    if(m_current_socket)
    {
        if(m_current_socket->is_open())
        {
            asio::error_code ec;
            m_current_socket->shutdown(asio::ip::tcp::socket::shutdown_both, ec);
            m_current_socket->close(ec);
        }
    }
}

void TestServer::accept_handler(const asio::error_code& ec, std::shared_ptr<asio::ip::tcp::socket> socket)
{
    if(ec)
    {
        close_acceptor();
        return;
    }

    // Close existing socket
    close_socket();

    // Start reading from new socket
    m_current_socket = socket;
    begin_read();

    // Continue accepting new connections
    begin_accept();

    // Advertise the new connection
    std::lock_guard<std::mutex> lock(m_connection_lock);
    ++m_num_connections;
    m_pending_connection = true;
    m_connection_cv.notify_all();
}

void TestServer::read_handler(const asio::error_code& ec, std::size_t bytes_transferred)
{
    if(ec)
    {
        close_socket();

        // Advertise connection lost
        std::lock_guard<std::mutex> lock(m_close_connection_lock);
        m_pending_close_connection = true;
        m_close_connection_cv.notify_all();

        return;
    }

    begin_read();

    // Advertise new data
    std::lock_guard<std::mutex> lock(m_data_lock);
    m_buffer_size += bytes_transferred;
    m_data_cv.notify_all();
}

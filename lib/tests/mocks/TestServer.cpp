#include "TestServer.h"

TestServer::TestServer(unsigned short port)
        : m_io_service{std::make_shared<asio::io_service>()},
          m_thread_pool{m_io_service, 1},
          m_acceptor{*m_io_service, asio::ip::tcp::endpoint{asio::ip::tcp::v4(), port}}
{

}

TestServer::~TestServer()
{
    m_io_service->stop();
}

void TestServer::start()
{
    m_current_socket = std::make_shared<asio::ip::tcp::socket>(*m_io_service);
    m_acceptor.async_accept(*m_current_socket, [=](const asio::error_code& ec) { handle_accept(ec); });
}

void TestServer::stop()
{
    if(m_current_socket)
    {
        m_current_socket->cancel();
    }
}

bool TestServer::wait_for_connection()
{
    std::unique_lock<std::mutex> lock(m_lock);

    auto result = m_connection_cv.wait_for(lock, std::chrono::seconds(5), [=]() { return m_connected; });
    return result;
}

void TestServer::handle_accept(const std::error_code& ec)
{
    if(!ec)
    {
        std::lock_guard<std::mutex> lock(m_lock);
        m_connected = true;
        m_connection_cv.notify_all();
    }
}

#include "mocks/TestServer.h"

TestServer::TestServer(unsigned short port)
        : m_io_service{std::make_shared<asio::io_service>()},
          m_thread_pool{m_io_service, 1},
          m_endpoint{asio::ip::tcp::endpoint{asio::ip::tcp::v4(), port}},
          m_acceptor{*m_io_service},
          m_pending_connection{false},
          m_num_connections{0}
{

}

TestServer::~TestServer()
{
    m_io_service->stop();
}

void TestServer::start()
{
    m_acceptor.open(m_endpoint.protocol());
    m_acceptor.bind(m_endpoint);
    m_acceptor.listen();
    m_current_socket = std::make_shared<asio::ip::tcp::socket>(*m_io_service);
    m_acceptor.async_accept(*m_current_socket, [=](const asio::error_code& ec) { handle_accept(ec); });
}

void TestServer::stop()
{
    if(m_current_socket)
    {
        asio::error_code ec;
        m_current_socket->close(ec);
    }
    m_acceptor.close();
}

void TestServer::send(const openpal::rseq_t& data)
{
    m_current_socket->write_some(asio::buffer(data, data.length()));
}

bool TestServer::wait_for_connection()
{
    std::unique_lock<std::mutex> lock(m_lock);

    auto result = m_connection_cv.wait_for(lock, std::chrono::seconds(5), [=]() { return m_pending_connection; });
    m_pending_connection = false;

    return result;
}

unsigned int TestServer::get_num_connections()
{
    std::lock_guard<std::mutex> lock(m_lock);
    return m_num_connections;
}

void TestServer::handle_accept(const std::error_code& ec)
{
    if(!ec)
    {
        {
            std::lock_guard<std::mutex> lock(m_lock);
            ++m_num_connections;
            m_pending_connection = true;
            m_connection_cv.notify_all();
        }

        m_acceptor.async_accept(*m_current_socket, [=](const asio::error_code& ec) { handle_accept(ec); });
    }
}

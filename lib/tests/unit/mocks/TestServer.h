#ifndef MODBUS_TESTSERVER_H
#define MODBUS_TESTSERVER_H

#include <condition_variable>
#include <memory>
#include <mutex>

#include "exe4cpp/asio/ThreadPool.h"
#include "exe4cpp/Typedefs.h"
#include "ser4cpp/container/SequenceTypes.h"
#include "ser4cpp/container/StaticBuffer.h"

class TestServer
{
public:
    TestServer(unsigned short port, exe4cpp::duration_t timeout);
    ~TestServer();

    void start();
    void stop();

    void send(const ser4cpp::rseq_t& data);

    bool wait_for_connection();
    unsigned int get_num_connections();

    bool wait_for_close_connection();

    bool wait_for_data();

private:
    void begin_accept();
    void begin_read();
    void close_acceptor();
    void close_socket();

    void accept_handler(const asio::error_code& ec, std::shared_ptr<asio::ip::tcp::socket> socket);
    void read_handler(const asio::error_code& ec, std::size_t bytes_transferred);

    exe4cpp::duration_t m_timeout;
    asio::ip::tcp::endpoint m_endpoint;

    std::shared_ptr<asio::io_service> m_io_service;
    std::shared_ptr<asio::ip::tcp::acceptor> m_acceptor;
    std::shared_ptr<asio::ip::tcp::socket> m_current_socket;

    std::mutex m_start_lock;
    std::condition_variable m_start_cv;
    bool m_pending_start;

    std::mutex m_stop_lock;
    std::condition_variable m_stop_cv;
    bool m_pending_stop;

    std::mutex m_connection_lock;
    std::condition_variable m_connection_cv;
    bool m_pending_connection;
    unsigned int m_num_connections;

    std::mutex m_close_connection_lock;
    std::condition_variable m_close_connection_cv;
    bool m_pending_close_connection;

    std::mutex m_data_lock;
    std::condition_variable m_data_cv;
    std::array<uint8_t, 4096> m_data_buffer;
    std::size_t m_buffer_size;

    exe4cpp::ThreadPool m_thread_pool;
};

#endif //MODBUS_TESTSERVER_H

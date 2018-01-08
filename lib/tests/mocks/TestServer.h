#ifndef MODBUS_TESTSERVER_H
#define MODBUS_TESTSERVER_H

#include <condition_variable>
#include <memory>
#include <mutex>
#include "asio.hpp"
#include "asiopal/ThreadPool.h"
#include "openpal/container/SequenceTypes.h"

class TestServer
{
public:
    TestServer(unsigned short port);
    ~TestServer();

    void start();
    void stop();

    void send(const openpal::rseq_t& data);

    bool wait_for_connection();
    unsigned int get_num_connections();

private:
    void handle_accept(const asio::error_code& ec);

    std::shared_ptr<asio::io_service> m_io_service;
    openpal::ThreadPool m_thread_pool;
    asio::ip::tcp::endpoint m_endpoint;
    asio::ip::tcp::acceptor m_acceptor;
    std::shared_ptr<asio::ip::tcp::socket> m_current_socket;

    std::mutex m_lock;
    std::condition_variable m_connection_cv;
    bool m_pending_connection;
    unsigned int m_num_connections;
};

#endif //MODBUS_TESTSERVER_H

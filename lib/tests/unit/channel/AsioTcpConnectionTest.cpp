#include "catch.hpp"

#include <chrono>
#include <memory>

#include "loopser/asio/ThreadPool.h"
#include "loopser/container/StaticBuffer.h"
#include "loopser/executor/Typedefs.h"

#include "modbus/Ipv4Endpoint.h"
#include "modbus/logging/LoggerFactory.h"
#include "channel/AsioTcpConnection.h"
#include "mocks/AsioTcpConnectionWrapper.h"
#include "mocks/ConnectionListenerMock.h"
#include "mocks/TestServer.h"

using namespace modbus;

void send_test_data(asio::strand& strand, AsioTcpConnectionWrapper& connection, const loopser::rseq_t& data)
{
    auto shared_connection = connection.get();
    strand.post([=] () {
        shared_connection->send(data);
    });
}

void close_connection(asio::strand& strand, AsioTcpConnectionWrapper& connection)
{
    auto shared_connection = connection.get();
    strand.post([=] () {
        shared_connection->close();
    });
}

TEST_CASE("AsioTcpConnection")
{
    constexpr unsigned short test_port = 8888;
    constexpr loopser::duration_t timeout = std::chrono::seconds(5);
    loopser::StaticBuffer<unsigned int, 6> test_data;
    {
        auto dest = test_data.as_wseq();
        dest[0] = 'h';
        dest[1] = 'e';
        dest[2] = 'l';
        dest[3] = 'l';
        dest[4] = 'o';
        dest[5] = '\n';
    }

    auto connection_listener = std::make_shared<ConnectionListenerMock>();
    TestServer test_server{test_port, timeout};

    auto io_service = std::make_shared<asio::io_service>();
    asio::strand strand{*io_service};
    loopser::asioloop::ThreadPool thread_pool{io_service, 1};

    auto logger = LoggerFactory::create_null_logger("test");
    const Ipv4Endpoint endpoint{"127.0.0.1", test_port};
    AsioTcpConnectionWrapper asio_tcp_connection{std::make_shared<AsioTcpConnection>(logger, io_service, strand, endpoint)};
    asio_tcp_connection->set_listener(connection_listener);

    SECTION("When sending data, then connection is established.")
    {
        test_server.start();

        send_test_data(strand, asio_tcp_connection, test_data.as_seq());

        REQUIRE(test_server.wait_for_connection() == true);
        REQUIRE(connection_listener->get_num_errors() == 0);

        SECTION("When sending data twice, then re-use same connection.")
        {
            send_test_data(strand, asio_tcp_connection, test_data.as_seq());

            REQUIRE(test_server.wait_for_connection() == false);
            REQUIRE(test_server.get_num_connections() == 1);
            REQUIRE(connection_listener->get_num_errors() == 0);
        }

        SECTION("When connection is lost, then a connection is re-established.")
        {
            test_server.stop();
            test_server.start();
            send_test_data(strand, asio_tcp_connection, test_data.as_seq());

            REQUIRE(test_server.wait_for_connection() == true);
            REQUIRE(test_server.wait_for_data() == true);
        }

        SECTION("When close, then effectively close the connection.")
        {
            close_connection(strand, asio_tcp_connection);

            REQUIRE(test_server.wait_for_close_connection() == true);
        }
    }

    SECTION("When server is unreachable, then an error is reported.")
    {
        send_test_data(strand, asio_tcp_connection, test_data.as_seq());

        REQUIRE(connection_listener->wait_for_error() == true);
    }

    SECTION("When write is completed, then it is reported.")
    {
        test_server.start();

        send_test_data(strand, asio_tcp_connection, test_data.as_seq());

        REQUIRE(connection_listener->wait_for_write_done() == true);
    }

    SECTION("When data is received, then data is reported.")
    {
        test_server.start();
        send_test_data(strand, asio_tcp_connection, test_data.as_seq());
        REQUIRE(test_server.wait_for_connection() == true);

        test_server.send(test_data.as_seq());

        REQUIRE(connection_listener->wait_for_data() == true);
    }
}

#include "catch.hpp"

#include <memory>
#include "asio.hpp"
#include "asiopal/ThreadPool.h"
#include "openpal/container/StaticBuffer.h"

#include "modbus/Ipv4Endpoint.h"
#include "channel/AsioTcpConnection.h"
#include "mocks/ConnectionListenerMock.h"
#include "mocks/TestServer.h"

using namespace modbus;

TEST_CASE("AsioTcpConnection")
{
    constexpr auto test_port = 502;
    auto io_service = std::make_shared<asio::io_service>();
    asio::strand strand{*io_service};
    openpal::ThreadPool thread_pool{io_service, 1};

    const Ipv4Endpoint endpoint{"127.0.0.1", test_port};
    auto asio_tcp_connection = std::make_shared<AsioTcpConnection>(io_service, strand, endpoint);

    ConnectionListenerMock connection_listener;
    asio_tcp_connection->set_listener(&connection_listener);

    TestServer test_server{test_port};

    openpal::StaticBuffer<unsigned int, 6> test_data;
    {
        auto dest = test_data.as_wseq();
        dest[0] = 'h';
        dest[1] = 'e';
        dest[2] = 'l';
        dest[3] = 'l';
        dest[4] = 'o';
        dest[5] = '\n';
    }

    SECTION("When sending data, then connection is established.")
    {
        test_server.start();

        asio_tcp_connection->send(test_data.as_seq());

        REQUIRE(test_server.wait_for_connection() == true);
        REQUIRE(connection_listener.get_num_errors() == 0);
    }

    SECTION("When sending data twice, then re-use same connection.")
    {
        test_server.start();
        asio_tcp_connection->send(test_data.as_seq());
        REQUIRE(test_server.wait_for_connection() == true);

        asio_tcp_connection->send(test_data.as_seq());
        REQUIRE(test_server.wait_for_connection() == false);
        REQUIRE(test_server.get_num_connections() == 1);
        REQUIRE(connection_listener.get_num_errors() == 0);
    }

    SECTION("When connection is lost, then a connection is re-establish.")
    {
        test_server.start();
        asio_tcp_connection->send(test_data.as_seq());
        REQUIRE(test_server.wait_for_connection() == true);

        test_server.stop();
        test_server.start();
        asio_tcp_connection->send(test_data.as_seq());

        REQUIRE(test_server.wait_for_connection() == true);
    }

    SECTION("When server is unreachable, then an error is reported.")
    {
        asio_tcp_connection->send(test_data.as_seq());

        REQUIRE(connection_listener.wait_for_error() == true);
    }

    SECTION("When data is received, then data is reported.")
    {
        test_server.start();
        asio_tcp_connection->send(test_data.as_seq());
        REQUIRE(test_server.wait_for_connection() == true);

        test_server.send(test_data.as_seq());

        REQUIRE(connection_listener.wait_for_data() == true);
    }

    io_service->stop();
}

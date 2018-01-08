#include "catch.hpp"

#include <memory>
#include "asio.hpp"
#include "asiopal/ThreadPool.h"
#include "openpal/container/StaticBuffer.h"

#include "modbus/Ipv4Endpoint.h"
#include "channel/AsioTcpConnection.h"
#include "mocks/TestServer.h"

using namespace modbus;

TEST_CASE("AsioTcpConnection")
{
    auto io_service = std::make_shared<asio::io_service>();
    asio::strand strand{*io_service};
    openpal::ThreadPool thread_pool{io_service, 1};

    const Ipv4Endpoint endpoint{"127.0.0.1", 8888};
    auto asio_tcp_connection = std::make_shared<AsioTcpConnection>(io_service, strand, endpoint);

    TestServer test_server{8888};

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
    }

    io_service->stop();
}

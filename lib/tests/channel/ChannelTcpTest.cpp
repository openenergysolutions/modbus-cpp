#include "catch.hpp"

#include <memory>
#include "modbus/logging/LoggerFactory.h"
#include "channel/ChannelTcp.h"
#include "mocks/ExecutorMock.h"
#include "mocks/RequestMock.h"
#include "mocks/TcpConnectionMock.h"

using namespace modbus;

TEST_CASE("ChannelTcp")
{
    auto unit_id = UnitIdentifier::default_unit_identifier();
    auto request = RequestMock{1, 0x42};
    auto timeout = std::chrono::seconds(5);
    auto empty_handler = [](const Expected<openpal::rseq_t>&) {};

    auto executor = std::make_shared<ExecutorMock>();
    auto logger = LoggerFactory::create_null_logger("test");
    auto tcp_connection = std::make_shared<TcpConnectionMock>();

    auto channel = std::make_shared<ChannelTcp>(executor, logger, tcp_connection);

    SECTION("When send request, then request is sent to the connection")
    {
        channel->send_request(unit_id, request, timeout, empty_handler);

        REQUIRE(tcp_connection->get_num_requests() == 1);
    }
}

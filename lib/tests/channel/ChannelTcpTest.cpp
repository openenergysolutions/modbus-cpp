#include "catch.hpp"

#include <memory>
#include "modbus/logging/LoggerFactory.h"
#include "channel/ChannelTcp.h"
#include "mocks/ExecutorMock.h"
#include "mocks/RequestMock.h"
#include "mocks/TcpConnectionMock.h"

using namespace modbus;

void receive(std::shared_ptr<ChannelTcp> channel,
             const UnitIdentifier& unit_id,
             const TransactionIdentifier transaction_id,
             const IRequest& request)
{
    openpal::Buffer buffer{260};
    auto view = buffer.as_wslice();
    auto serialized_request = MbapMessage::build_message(unit_id, transaction_id, request, view);
    channel->on_receive(serialized_request);
}

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

    SECTION("Queuing")
    {
        SECTION("When send request, then request is sent to the connection")
        {
            channel->send_request(unit_id, request, timeout, empty_handler);

            REQUIRE(tcp_connection->get_num_requests() == 1);
            REQUIRE(tcp_connection->get_requests()[0].unit_id == unit_id);
            REQUIRE(tcp_connection->get_requests()[0].data.length() == 1);

            SECTION("When send another request, then wait for reception of the current request")
            {
                channel->send_request(unit_id, request, timeout, empty_handler);
                REQUIRE(tcp_connection->get_num_requests() == 1);

                receive(channel, unit_id, tcp_connection->get_requests()[0].transaction_id, request);
                REQUIRE(tcp_connection->get_num_requests() == 2);
            }
        }

        SECTION("When send multiple requests, then requests are sent in order")
        {
            channel->send_request(unit_id, RequestMock{1, 0x01}, timeout, empty_handler);
            channel->send_request(unit_id, RequestMock{1, 0x02}, timeout, empty_handler);
            channel->send_request(unit_id, RequestMock{1, 0x03}, timeout, empty_handler);

            REQUIRE(tcp_connection->get_num_requests() == 1);
            REQUIRE(tcp_connection->get_requests()[0].data[0] == 0x01);

            receive(channel, unit_id, tcp_connection->get_requests()[0].transaction_id, request);

            REQUIRE(tcp_connection->get_num_requests() == 2);
            REQUIRE(tcp_connection->get_requests()[1].data[0] == 0x02);

            receive(channel, unit_id, tcp_connection->get_requests()[1].transaction_id, request);

            REQUIRE(tcp_connection->get_num_requests() == 3);
            REQUIRE(tcp_connection->get_requests()[2].data[0] == 0x03);

            SECTION("And all transaction numbers are different")
            {
                auto transaction_id_1 = tcp_connection->get_requests()[0].transaction_id;
                auto transaction_id_2 = tcp_connection->get_requests()[1].transaction_id;
                auto transaction_id_3 = tcp_connection->get_requests()[2].transaction_id;

                REQUIRE(transaction_id_1 != transaction_id_2);
                REQUIRE(transaction_id_2 != transaction_id_3);
                REQUIRE(transaction_id_1 != transaction_id_3);
            }
        }

        SECTION("When connection error, then discard all pending requests")
        {
            channel->send_request(unit_id, RequestMock{1, 0x01}, timeout, empty_handler);
            channel->send_request(unit_id, RequestMock{1, 0x02}, timeout, empty_handler);
            channel->send_request(unit_id, RequestMock{1, 0x03}, timeout, empty_handler);
            REQUIRE(tcp_connection->get_num_requests() == 1);

            channel->on_error();

            REQUIRE(tcp_connection->get_num_requests() == 1);

            channel->send_request(unit_id, RequestMock{1, 0x04}, timeout, empty_handler);
            REQUIRE(tcp_connection->get_num_requests() == 2);
            REQUIRE(tcp_connection->get_requests()[1].data[0] == 0x04);
        }
    }

    SECTION("Message handling")
    {
        SECTION("When receive corresponding response, report it back")
        {

        }

        SECTION("When receive response with wrong transaction id, then continue waiting")
        {

        }

        SECTION("When receive response with wrong unit identifier, then continue waiting")
        {

        }

        SECTION("When request times out, then report timeout")
        {

        }

        SECTION("When connection error, then report error and reset parser")
        {

        }
    }

    SECTION("Miscellaneous")
    {
        SECTION("When shutdown, then discard all pending requests and close the connection")
        {

        }

        SECTION("All public methods are executed through the Executor")
        {

        }
    }
}

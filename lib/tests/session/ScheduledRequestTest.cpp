#include "catch.hpp"

#include "modbus/Expected.h"
#include "modbus/exceptions/ModbusException.h"
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/session/ISchedule.h"
#include "modbus/session/ScheduleFactory.h"
#include "mocks/ExecutorMock.h"
#include "mocks/ISessionMock.h"
#include "mocks/ISessionResponseHandlerMock.h"
#include "session/ScheduledRequest.h"

using namespace modbus;

TEST_CASE("ScheduledRequest")
{
    auto session = std::make_shared<ISessionMock>();
    auto session_response_handler = std::make_shared<ISessionResponseHandlerMock>();
    auto executor = std::make_shared<ExecutorMock>();
    auto request = ReadHoldingRegistersRequest{Address{1}, 0x42};
    auto timeout = std::chrono::seconds(5);
    auto schedule_time = std::chrono::seconds(2);
    auto schedule = ScheduleFactory::create_periodic_schedule(schedule_time);
    auto scheduled_req = std::make_shared<ScheduledRequest<ReadHoldingRegistersRequest, ReadHoldingRegistersResponse>>(session,
                                                                                                                       session_response_handler,
                                                                                                                       executor,
                                                                                                                       request,
                                                                                                                       timeout,
                                                                                                                       std::move(schedule));

    SECTION("When start, then send request.")
    {
        scheduled_req->start();
        REQUIRE(scheduled_req->is_running() == true);
        REQUIRE(session->get_num_read_holding_registers_request_sent() == 1);
        auto handler = session->get_last_read_holding_registers_request_handler();

        SECTION("Don't re-send request until previous response was received or timed-out.")
        {
            executor->advance_time(schedule_time);

            REQUIRE(session->get_num_read_holding_registers_request_sent() == 1);
        }

        SECTION("When proper response is received, then send it to ISessionResponseHandler.")
        {
            handler(Expected<ReadHoldingRegistersResponse>{ReadHoldingRegistersResponse{}});

            REQUIRE(session_response_handler->get_num_read_holding_registers_response_received() == 1);
        }

        SECTION("When exception response is received, then send it to ISessionResponseHandler.")
        {
            handler(Expected<ReadHoldingRegistersResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress}));

            REQUIRE(session_response_handler->get_num_exceptions_received() == 1);
        }

        SECTION("When timeout response is received, then send it to ISessionResponseHandler.")
        {
            handler(Expected<ReadHoldingRegistersResponse>::from_exception(TimeoutException{}));

            REQUIRE(session_response_handler->get_num_timeouts_received() == 1);
        }

        SECTION("When response is received, then send a request after waiting the specified timeout.")
        {
            handler(Expected<ReadHoldingRegistersResponse>{ReadHoldingRegistersResponse{}});
            executor->advance_time(schedule_time);

            REQUIRE(session->get_num_read_holding_registers_request_sent() == 2);
        }

        SECTION("When stop, then stop sending requests.")
        {
            scheduled_req->stop();
            executor->advance_time(schedule_time);

            REQUIRE(scheduled_req->is_running() == false);
            REQUIRE(session->get_num_read_holding_registers_request_sent() == 1);

            SECTION("When stop called twice, then ignore second call.")
            {
                scheduled_req->stop();
                executor->advance_time(schedule_time);

                REQUIRE(scheduled_req->is_running() == false);
                REQUIRE(session->get_num_read_holding_registers_request_sent() == 1);
            }

            SECTION("When start called, then restart sending requests.")
            {
                scheduled_req->start();

                REQUIRE(scheduled_req->is_running() == true);
                REQUIRE(session->get_num_read_holding_registers_request_sent() == 2);
            }
        }
    }
}

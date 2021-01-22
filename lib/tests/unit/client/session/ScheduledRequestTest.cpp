/*
 * Copyright 2017-2018 Duke Energy Corporation and Open Energy Solutions, Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <catch2/catch.hpp>

#include "exe4cpp/MockExecutor.h"
#include "modbus/Expected.h"
#include "modbus/exceptions/ModbusException.h"
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "mocks/IClientSessionMock.h"
#include "client/session/ScheduledRequest.h"

using namespace modbus;

TEST_CASE("ScheduledRequest")
{
    auto session = std::make_shared<IClientSessionMock>();
    auto executor = std::make_shared<exe4cpp::MockExecutor>();
    auto request = ReadHoldingRegistersRequest{Address{1}, 0x42};
    auto timeout = std::chrono::seconds(5);
    auto frequency = std::chrono::seconds(2);

    // Handler
    unsigned int num_handler_success = 0;
    unsigned int num_handler_timeout = 0;
    unsigned int num_handler_error = 0;
    auto test_handler = [&] (const Expected<ReadHoldingRegistersResponse>& response) {
        if(response.is_valid())
        {
            ++num_handler_success;
        }
        else if(response.has_exception<TimeoutException>())
        {
            ++num_handler_timeout;
        }
        else
        {
            ++num_handler_error;
        }
    };
    auto scheduled_req = std::make_shared<ScheduledRequest<ReadHoldingRegistersRequest, ReadHoldingRegistersResponse>>(session,
                                                                                                                       test_handler,
                                                                                                                       executor,
                                                                                                                       request,
                                                                                                                       timeout,
                                                                                                                       frequency);

    SECTION("When start, then send request.")
    {
        scheduled_req->start();
        executor->run_one();
        REQUIRE(scheduled_req->is_running() == true);
        REQUIRE(session->get_num_read_holding_registers_request_sent() == 1);
        auto session_handler = session->get_last_read_holding_registers_request_handler();

        SECTION("Don't re-send request until previous response was received or timed-out.")
        {
            executor->advance_time(frequency);
            executor->run_one();

            REQUIRE(session->get_num_read_holding_registers_request_sent() == 1);
        }

        SECTION("When proper response is received, then send it to the handler.")
        {
            session_handler(Expected<ReadHoldingRegistersResponse>{ReadHoldingRegistersResponse{}});

            REQUIRE(num_handler_success == 1);
            REQUIRE(num_handler_timeout == 0);
            REQUIRE(num_handler_error == 0);
        }

        SECTION("When exception response is received, then send it to the handler.")
        {
            session_handler(Expected<ReadHoldingRegistersResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress}));

            REQUIRE(num_handler_success == 0);
            REQUIRE(num_handler_timeout == 0);
            REQUIRE(num_handler_error == 1);
        }

        SECTION("When timeout response is received, then send it to the handler.")
        {
            session_handler(Expected<ReadHoldingRegistersResponse>::from_exception(TimeoutException{}));

            REQUIRE(num_handler_success == 0);
            REQUIRE(num_handler_timeout == 1);
            REQUIRE(num_handler_error == 0);
        }

        SECTION("When response is received, then send a request after waiting the specified timeout.")
        {
            session_handler(Expected<ReadHoldingRegistersResponse>{ReadHoldingRegistersResponse{}});
            executor->advance_time(frequency);
            executor->run_one();

            REQUIRE(session->get_num_read_holding_registers_request_sent() == 2);
        }

        SECTION("When set new frequency, then start sending requests with new frequency.")
        {
            auto new_frequency = std::chrono::seconds(1);
            scheduled_req->set_frequency(new_frequency);
            executor->run_one();

            session_handler(Expected<ReadHoldingRegistersResponse>{ReadHoldingRegistersResponse{}});

            executor->advance_time(new_frequency);
            executor->run_one();
            REQUIRE(session->get_num_read_holding_registers_request_sent() == 2);
        }

        SECTION("When stop, then stop sending requests.")
        {
            scheduled_req->stop();
            executor->run_one();
            executor->advance_time(frequency);

            REQUIRE(scheduled_req->is_running() == false);
            REQUIRE(session->get_num_read_holding_registers_request_sent() == 1);

            SECTION("When stop called twice, then ignore second call.")
            {
                scheduled_req->stop();
                executor->run_one();
                executor->advance_time(frequency);

                REQUIRE(scheduled_req->is_running() == false);
                REQUIRE(session->get_num_read_holding_registers_request_sent() == 1);
            }

            SECTION("When receive response of old request, don't start the timer.")
            {
                session_handler(Expected<ReadHoldingRegistersResponse>{ReadHoldingRegistersResponse{}});
                executor->run_one();
                executor->advance_time(frequency);

                REQUIRE(session->get_num_read_holding_registers_request_sent() == 1);
            }

            SECTION("When start called, then restart sending requests.")
            {
                scheduled_req->start();
                executor->run_one();

                REQUIRE(scheduled_req->is_running() == true);
                REQUIRE(session->get_num_read_holding_registers_request_sent() == 2);
            }
        }
    }
}

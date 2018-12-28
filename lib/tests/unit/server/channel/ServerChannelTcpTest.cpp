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
#include "catch.hpp"

#include <memory>
#include "exe4cpp/MockExecutor.h"
#include "ser4cpp/container/Buffer.h"
#include "ser4cpp/serialization/BigEndian.h"

#include "modbus/logging/LoggerFactory.h"
#include "modbus/messages/ReadCoilsResponse.h"
#include "modbus/messages/ReadDiscreteInputsResponse.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/messages/ReadInputRegistersResponse.h"
#include "modbus/messages/WriteMultipleCoilsResponse.h"
#include "modbus/messages/WriteMultipleRegistersResponse.h"
#include "modbus/messages/WriteSingleCoilResponse.h"
#include "modbus/messages/WriteSingleRegisterResponse.h"
#include "messages/ReadCoilsRequestImpl.h"
#include "messages/ReadDiscreteInputsRequestImpl.h"
#include "messages/ReadHoldingRegistersRequestImpl.h"
#include "messages/ReadInputRegistersRequestImpl.h"
#include "messages/WriteMultipleCoilsRequestImpl.h"
#include "messages/WriteMultipleRegistersRequestImpl.h"
#include "messages/WriteSingleCoilRequestImpl.h"
#include "messages/WriteSingleRegisterRequestImpl.h"
#include "server/channel/ServerChannelTcp.h"
#include "server/channel/ServerConnectionListenerBuilder.h"
#include "mocks/IServerMock.h"
#include "mocks/IServerSessionMock.h"
#include "mocks/TcpConnectionMock.h"

using trompeloeil::_;
using namespace modbus;

void send_message(UnitIdentifier unit_id,
                  TransactionIdentifier transaction_id,
                  const IMessage& message,
                  std::shared_ptr<ServerChannelTcp> channel,
                  ITcpConnection& connection)
{
    ser4cpp::Buffer buffer{message.get_message_length()};
    message.build_message(buffer.as_wslice());

    channel->on_mbap(MbapMessage{unit_id, transaction_id, buffer.as_rslice()}, connection);
}

void check_response_and_function_code(const TcpConnectionMock& connection,
                                      UnitIdentifier unit_id,
                                      TransactionIdentifier transaction_id,
                                      uint8_t function_code)
{
    REQUIRE(connection.get_num_requests() == 1);
    MbapMessage msg = connection.get_requests()[0];
    REQUIRE(msg.unit_id == unit_id);
    REQUIRE(msg.transaction_id == transaction_id);

    auto view = msg.data;
    uint8_t received_function_code;
    ser4cpp::UInt8::read_from(view, received_function_code);
    REQUIRE(received_function_code == function_code);
}

void check_response_and_exception_code(const TcpConnectionMock& connection,
                                       UnitIdentifier unit_id,
                                       TransactionIdentifier transaction_id,
                                       uint8_t function_code,
                                       uint8_t exception_code)
{
    REQUIRE(connection.get_num_requests() == 1);
    MbapMessage msg = connection.get_requests()[0];
    REQUIRE(msg.unit_id == unit_id);
    REQUIRE(msg.transaction_id == transaction_id);

    auto view = msg.data;
    REQUIRE(view.length() == 2);
    uint8_t received_function_code;
    uint8_t received_exception_code;
    ser4cpp::UInt8::read_from(view, received_function_code);
    ser4cpp::UInt8::read_from(view, received_exception_code);
    REQUIRE(received_function_code == function_code);
    REQUIRE(received_exception_code == exception_code);
}

TEST_CASE("ServerChannelTcp")
{
    auto session = std::make_shared<IServerSessionMock>();
    ALLOW_CALL(*session, shutdown());

    auto logger = LoggerFactory::create_null_logger("test");
    auto executor = std::make_shared<exe4cpp::MockExecutor>();
    auto server = std::make_shared<IServerMock>();
    ALLOW_CALL(*server, shutdown());
    auto channel = std::make_shared<ServerChannelTcp>(logger, executor, server);

    SECTION("When start, then start the server in the server thread")
    {
        channel->start();
        REQUIRE_CALL(*server, start(_));
        executor->run_one();

        SECTION("When shutdown, then shutdown the server in the server thread")
        {
            channel->shutdown();
            REQUIRE_CALL(*server, shutdown());
            executor->run_one();
        }

        SECTION("With session")
        {
            UnitIdentifier unit_id{0x42};
            TransactionIdentifier transaction_id{0x1234};

            TcpConnectionMock connection{};
            channel->add_session(unit_id, session);
            executor->run_one();

            SECTION("When receive a message to unknown session, then don't send it anywhere")
            {
                send_message(UnitIdentifier{0x24},
                             transaction_id,
                             ReadCoilsRequestImpl{ReadCoilsRequest{}},
                             channel,
                             connection);
            }

            SECTION("When receive an empty message, then do nothing")
            {
                ser4cpp::Buffer buf{0};
                MbapMessage message{unit_id, transaction_id, buf.as_rslice()};
                channel->on_mbap(message, connection);
            }

            SECTION("When receive a message with unknown function code, then send Modbus exception")
            {
                ser4cpp::Buffer buf{1};
                ser4cpp::UInt8::write_to(buf.as_wslice(), 0x42);
                MbapMessage message{unit_id, transaction_id, buf.as_rslice()};
                channel->on_mbap(message, connection);

                check_response_and_exception_code(connection, unit_id, transaction_id, 0xC2, 0x01);
            }

            SECTION("When receive a ReadCoilsRequest, then send appropriate response")
            {
                REQUIRE_CALL(*session, on_request(ANY(ReadCoilsRequest)))
                    .RETURN(ReadCoilsResponse{});

                send_message(unit_id,
                             transaction_id,
                             ReadCoilsRequestImpl{ReadCoilsRequest{}},
                             channel,
                             connection);

                check_response_and_function_code(connection, unit_id, transaction_id, 0x01);
            }

            SECTION("When receive a ReadCoilsRequest and session returns exception, then send Modbus exception")
            {
                REQUIRE_CALL(*session, on_request(ANY(ReadCoilsRequest)))
                    .RETURN(Expected<ReadCoilsResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress}));

                send_message(unit_id,
                             transaction_id,
                             ReadCoilsRequestImpl{ReadCoilsRequest{}},
                             channel,
                             connection);

                check_response_and_exception_code(connection, unit_id, transaction_id, 0x81, 0x02);
            }

            SECTION("When receive a ReadDiscreteInputsRequest, then send appropriate response")
            {
                REQUIRE_CALL(*session, on_request(ANY(ReadDiscreteInputsRequest)))
                    .RETURN(ReadDiscreteInputsResponse{});

                send_message(unit_id,
                             transaction_id,
                             ReadDiscreteInputsRequestImpl{ReadDiscreteInputsRequest{}},
                             channel,
                             connection);

                check_response_and_function_code(connection, unit_id, transaction_id, 0x02);
            }

            SECTION("When receive a ReadDiscreteInputsRequest and session returns exception, then send Modbus exception")
            {
                REQUIRE_CALL(*session, on_request(ANY(ReadDiscreteInputsRequest)))
                    .RETURN(Expected<ReadDiscreteInputsResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress}));

                send_message(unit_id,
                             transaction_id,
                             ReadDiscreteInputsRequestImpl{ReadDiscreteInputsRequest{}},
                             channel,
                             connection);

                check_response_and_exception_code(connection, unit_id, transaction_id, 0x82, 0x02);
            }

            SECTION("When receive a ReadHoldingRegistersRequest, then send appropriate response")
            {
                REQUIRE_CALL(*session, on_request(ANY(ReadHoldingRegistersRequest)))
                    .RETURN(ReadHoldingRegistersResponse{});

                send_message(unit_id,
                             transaction_id,
                             ReadHoldingRegistersRequestImpl{ReadHoldingRegistersRequest{}},
                             channel,
                             connection);

                check_response_and_function_code(connection, unit_id, transaction_id, 0x03);
            }

            SECTION("When receive a ReadHoldingRegistersRequest and session returns exception, then send Modbus exception")
            {
                REQUIRE_CALL(*session, on_request(ANY(ReadHoldingRegistersRequest)))
                    .RETURN(Expected<ReadHoldingRegistersResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress}));

                send_message(unit_id,
                             transaction_id,
                             ReadHoldingRegistersRequestImpl{ReadHoldingRegistersRequest{}},
                             channel,
                             connection);

                check_response_and_exception_code(connection, unit_id, transaction_id, 0x83, 0x02);
            }

            SECTION("When receive a ReadInputRegistersRequest, then send appropriate response")
            {
                REQUIRE_CALL(*session, on_request(ANY(ReadInputRegistersRequest)))
                    .RETURN(ReadInputRegistersResponse{});

                send_message(unit_id,
                             transaction_id,
                             ReadInputRegistersRequestImpl{ReadInputRegistersRequest{}},
                             channel,
                             connection);

                check_response_and_function_code(connection, unit_id, transaction_id, 0x04);
            }

            SECTION("When receive a ReadInputRegistersRequest and session returns exception, then send Modbus exception")
            {
                REQUIRE_CALL(*session, on_request(ANY(ReadInputRegistersRequest)))
                    .RETURN(Expected<ReadInputRegistersResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress}));

                send_message(unit_id,
                             transaction_id,
                             ReadInputRegistersRequestImpl{ReadInputRegistersRequest{}},
                             channel,
                             connection);

                check_response_and_exception_code(connection, unit_id, transaction_id, 0x84, 0x02);
            }

            SECTION("When receive a WriteSingleCoilRequest, then send appropriate response")
            {
                REQUIRE_CALL(*session, on_request(ANY(WriteSingleCoilRequest)))
                    .RETURN(WriteSingleCoilResponse{});

                send_message(unit_id,
                             transaction_id,
                             WriteSingleCoilRequestImpl{WriteSingleCoilRequest{}},
                             channel,
                             connection);

                check_response_and_function_code(connection, unit_id, transaction_id, 0x05);
            }

            SECTION("When receive a WriteSingleCoilRequest and session returns exception, then send Modbus exception")
            {
                REQUIRE_CALL(*session, on_request(ANY(WriteSingleCoilRequest)))
                    .RETURN(Expected<WriteSingleCoilResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress}));

                send_message(unit_id,
                             transaction_id,
                             WriteSingleCoilRequestImpl{WriteSingleCoilRequest{}},
                             channel,
                             connection);

                check_response_and_exception_code(connection, unit_id, transaction_id, 0x85, 0x02);
            }

            SECTION("When receive a WriteSingleRegisterRequest, then send appropriate response")
            {
                REQUIRE_CALL(*session, on_request(ANY(WriteSingleRegisterRequest)))
                    .RETURN(WriteSingleRegisterResponse{});

                send_message(unit_id,
                             transaction_id,
                             WriteSingleRegisterRequestImpl{WriteSingleRegisterRequest{}},
                             channel,
                             connection);

                check_response_and_function_code(connection, unit_id, transaction_id, 0x06);
            }

            SECTION("When receive a WriteSingleRegisterRequest and session returns exception, then send Modbus exception")
            {
                REQUIRE_CALL(*session, on_request(ANY(WriteSingleRegisterRequest)))
                    .RETURN(Expected<WriteSingleRegisterResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress}));

                send_message(unit_id,
                             transaction_id,
                             WriteSingleRegisterRequestImpl{WriteSingleRegisterRequest{}},
                             channel,
                             connection);

                check_response_and_exception_code(connection, unit_id, transaction_id, 0x86, 0x02);
            }

            SECTION("When receive a WriteMultipleCoilsRequest, then send appropriate response")
            {
                REQUIRE_CALL(*session, on_request(ANY(WriteMultipleCoilsRequest)))
                    .RETURN(WriteMultipleCoilsResponse{});

                send_message(unit_id,
                             transaction_id,
                             WriteMultipleCoilsRequestImpl{WriteMultipleCoilsRequest{0x1234, {true, false}}},
                             channel,
                             connection);

                check_response_and_function_code(connection, unit_id, transaction_id, 0x0F);
            }

            SECTION("When receive a WriteMultipleCoilsRequest and session returns exception, then send Modbus exception")
            {
                REQUIRE_CALL(*session, on_request(ANY(WriteMultipleCoilsRequest)))
                    .RETURN(Expected<WriteMultipleCoilsResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress}));

                send_message(unit_id,
                             transaction_id,
                             WriteMultipleCoilsRequestImpl{WriteMultipleCoilsRequest{0x1234, {true, false}}},
                             channel,
                             connection);

                check_response_and_exception_code(connection, unit_id, transaction_id, 0x8F, 0x02);
            }

            SECTION("When receive a WriteMultipleRegistersRequest, then send appropriate response")
            {
                REQUIRE_CALL(*session, on_request(ANY(WriteMultipleRegistersRequest)))
                    .RETURN(WriteMultipleRegistersResponse{});

                send_message(unit_id,
                             transaction_id,
                             WriteMultipleRegistersRequestImpl{WriteMultipleRegistersRequest{0x1234, {0x1234, 0x5678}}},
                             channel,
                             connection);

                check_response_and_function_code(connection, unit_id, transaction_id, 0x10);
            }

            SECTION("When receive a WriteMultipleRegistersRequest and session returns exception, then send Modbus exception")
            {
                REQUIRE_CALL(*session, on_request(ANY(WriteMultipleRegistersRequest)))
                    .RETURN(Expected<WriteMultipleRegistersResponse>::from_exception(ModbusException{ExceptionType::IllegalDataAddress}));

                send_message(unit_id,
                             transaction_id,
                             WriteMultipleRegistersRequestImpl{WriteMultipleRegistersRequest{0x1234, {0x1234, 0x5678}}},
                             channel,
                             connection);

                check_response_and_exception_code(connection, unit_id, transaction_id, 0x90, 0x02);
            }
        }
    }

    SECTION("When shutdown before being started, then do not shutdown the server")
    {
        FORBID_CALL(*server, shutdown());

        channel->shutdown();
    }
}

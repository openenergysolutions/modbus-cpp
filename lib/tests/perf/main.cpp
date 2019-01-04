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
#include <iostream>
#include <random>
#include <memory>
#include <vector>

#include "spdlog/spdlog.h"

#include "modbus/IModbusManager.h"
#include "modbus/exceptions/IException.h"
#include "modbus/logging/LoggerFactory.h"

#include "ServerSession.h"

using namespace modbus;

int main(int argc, char* argv[])
{
    // Random generators
    std::random_device rd{};
    std::default_random_engine gen{ rd() };

    // Loggers
    auto logger = spdlog::stdout_color_mt("Debug logger");
    auto lib_logger = spdlog::basic_logger_mt("Lib logger", "test.log", true);

    std::unique_ptr<IModbusManager> modbus_manager = IModbusManager::create(LoggerFactory::create_custom_logger(lib_logger), 16);

    for(size_t i = 0; i < 100; ++i)
    {
        unsigned int port = i + 8000;
        std::string server_name = "Server channel " + std::to_string(port);

        // Create server
        auto server_session = std::make_shared<ServerSession>();
        auto server_channel = modbus_manager->create_server_tcp_channel(server_name, Ipv4Endpoint{"127.0.0.1", port}, 16, LoggingLevel::Debug);
        server_channel->add_session(UnitIdentifier::default_unit_identifier(), server_session);
        server_channel->start();
        
        // Create clients
        for(unsigned int j = 0; j < 20; ++j)
        {
            std::string client_name = "Client channel " + std::to_string(port) + "-" + std::to_string(j);
            auto client_channel = modbus_manager->create_client_tcp_channel(client_name, Ipv4Endpoint{"127.0.0.1", port}, LoggingLevel::Debug);
            auto client_session = client_channel->create_session(UnitIdentifier::default_unit_identifier(),
                                                                    std::chrono::seconds(5));

            ReadHoldingRegistersRequest req{
                static_cast<uint16_t>(std::uniform_int_distribution<>(0,2000)(gen)), // Starting address
                static_cast<uint16_t>(std::uniform_int_distribution<>(1,123)(gen)) // Quantity of registers
            };
            client_session->schedule_request(req, std::chrono::seconds(std::uniform_int_distribution<>(1,10)(gen)), [=](Expected<ReadHoldingRegistersResponse> response) {
                if(!response.is_valid())
                {
                    auto exception_msg = response.get_exception<IException>().get_message();
                    logger->error("Exception from instance {}-{}: {}", i, j, exception_msg);
                    return;
                }

                logger->trace("Instance {}-{} received response", i, j);
            });
        }
    }

    while (true)
    {
        char cmd;
        std::cin >> cmd;

        switch (cmd)
        {
            case 'q':
                return 0;

            default:
                break;
        }
    }
}

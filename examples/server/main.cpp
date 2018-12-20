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
#include <memory>

#include "modbus/IModbusManager.h"
#include "modbus/exceptions/IException.h"
#include "modbus/exceptions/TimeoutException.h"
#include "modbus/logging/LoggerFactory.h"

using namespace modbus;

class CustomSession : public IServerSession
{
    Expected<ReadCoilsResponse> on_request(const ReadCoilsRequest& request) override
    {
        SingleBitValue value{};
        value.address = 16;
        value.value = true;

        ReadCoilsResponse response{};
        response.values.push_back(value);

        return Expected<ReadCoilsResponse>{response};
    }
};

int main(int argc, char* argv[])
{
    // Create a console logger
    auto logger = modbus::LoggerFactory::create_console_logger("Hello");

    // Create the modbus manager
    // This will create the necessary background threads
    std::unique_ptr<IModbusManager> modbusManager = IModbusManager::create(logger);

    // Create a TCP server channel
    // Each channel has its own strand of execution
    auto channel = modbusManager->create_server_tcp_channel("Example channel",
                                                            Ipv4Endpoint{ "127.0.0.1", 502 });

    // Create a custom session and add it to the channel
    auto session = std::make_shared<CustomSession>();
    channel->add_session(UnitIdentifier{0x01}, session);

    // Start the server
    channel->start();

    while (true)
    {
        char cmd;
        std::cin >> cmd;

        switch (cmd)
        {
        case 'q':
            // Quit
            return 0;

        default:
            break;
        }
    }
}

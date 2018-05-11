#include <iostream>
#include <memory>

#include "spdlog/spdlog.h"

#include "modbus/IModbusManager.h"
#include "modbus/exceptions/IException.h"
#include "modbus/logging/LoggerFactory.h"

using namespace modbus;

int main(int argc, char* argv[])
{
    auto logger = spdlog::stdout_color_mt("Perf tests");
    auto lib_logger = LoggerFactory::create_null_logger("Lib logger");

    std::unique_ptr<IModbusManager> modbusManager = IModbusManager::create(lib_logger, 16);

    for(size_t i = 0; i < 1000; ++i)
    {
        unsigned int port = i + 8000;
        std::string name = "Channel " + std::to_string(port);
        auto channel = modbusManager->create_tcp_channel(name, Ipv4Endpoint{ "127.0.0.1", port });

        auto session = channel->create_session(UnitIdentifier::default_unit_identifier(),
                                               std::chrono::seconds(3));

        ReadHoldingRegistersRequest req{ 0x0000, 16 };
        session->schedule_request(req, std::chrono::seconds(2), [=](Expected<ReadHoldingRegistersResponse> response) {
            if (!response.is_valid())
            {
                auto exception_msg = response.get_exception<IException>().get_message();
                logger->error("Exception from instance {}: {}", i, exception_msg);
                return;
            }

            logger->trace("Instance {} received response", i);
        });
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

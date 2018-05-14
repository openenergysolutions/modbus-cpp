#include <iostream>
#include <memory>

#include "modbus/IModbusManager.h"
#include "modbus/exceptions/IException.h"
#include "modbus/exceptions/TimeoutException.h"
#include "modbus/logging/LoggerFactory.h"

using namespace modbus;

int main(int argc, char* argv[])
{
    // Create a console logger
    auto logger = modbus::LoggerFactory::create_console_logger("Hello");

    // Create the modbus manager
    // This will create the necessary background threads
    std::unique_ptr<IModbusManager> modbusManager = IModbusManager::create(logger);

    // Create a TCP channel
    // Each channel has its own strand of execution
    auto channel = modbusManager->create_tcp_channel("Example channel",
                                                     Ipv4Endpoint{ "127.0.0.1", 502 });

    // Create a session with a specific unit identifier
    // Users will mainly play with the session to obtain what they want
    auto session = channel->create_session(UnitIdentifier{0x01},
                                           std::chrono::seconds(3));

    // Schedule a recurring request
    ReadHoldingRegistersRequest req{ 0x0024, 59 };
    auto scheduled_req = session->schedule_request(req,
                                                   std::chrono::seconds(2),
                                                   [](Expected<ReadHoldingRegistersResponse> response) {
        std::cout << "Scheduled request received response." << std::endl;
    });

    while (true)
    {
        char cmd;
        std::cin >> cmd;

        switch (cmd)
        {
        case 'r':
        {
            // Send a request and print the result
            // You can override the default timeout value set when creating the session
            ReadCoilsRequest req{ 0x0024, 120 };
            session->send_request(req, std::chrono::seconds(3), [](const Expected<ReadCoilsResponse>& response) {
                // If the exception is set, then an error occured
                if (!response.is_valid())
                {
                    // Check for specific error instance
                    if(response.has_exception<TimeoutException>())
                    {
                        std::cout << "Timeout reached." << std::endl;
                    }
                    else
                    {
                        // IException is the base class of all the errors
                        std::cout << response.get_exception<IException>().get_message() << std::endl;
                    }
                    return;
                }

                // Otherwise, everything went good and the response is available
                for (auto& value : response.get().values)
                {
                    std::cout << value.address << ": " << value.value << std::endl;
                }
            });
            break;
        }
        case 'w':
        {
            WriteMultipleRegistersRequest req{ 0x0024, {11, 22, 33} };

            session->send_request(req, [](const Expected<WriteMultipleRegistersResponse>& response) {
                if (!response.is_valid())
                {
                    std::cout << response.get_exception<IException>().get_message() << std::endl;
                    return;
                }

                std::cout << "Write result= " << response.get().starting_address << ": " << response.get().qty_of_registers << std::endl;
            });
            break;
        }
        case 's':
        {
            if(scheduled_req->is_running())
            {
                scheduled_req->stop();
            }
            else
            {
                scheduled_req->start();
            }
            break;
        }
        case 'f':
        {
            if(scheduled_req->get_frequency() == std::chrono::seconds(2))
            {
                scheduled_req->set_frequency(std::chrono::seconds(5));
            }
            else
            {
                scheduled_req->set_frequency(std::chrono::seconds(2));
            }
            break;
        }
        case 'q':
            // Quit
            return 0;

        default:
            break;
        }
    }
}

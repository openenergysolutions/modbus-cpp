#include <iostream>
#include <memory>

#include "modbus/IModbusManager.h"
#include "modbus/channel/IChannel.h"
#include "modbus/Ipv4Endpoint.h"
#include "modbus/session/ISchedule.h"
#include "modbus/session/ScheduleFactory.h"
#include "modbus/session/ISession.h"
#include "modbus/session/ISessionResponseHandler.h"

#include "modbus/exceptions/IException.h"

#include "modbus/logging/LoggerFactory.h"

#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/messages/WriteMultipleRegistersRequest.h"
#include "modbus/messages/WriteMultipleRegistersResponse.h"

using namespace modbus;

class MySessionResponseHandler : public ISessionResponseHandler
{
public:
    void on_response(const ReadHoldingRegistersResponse& response) override
    {
        // A scheduled response was received
        for(auto& value : response.get_values())
        {
            std::cout << value.address << ": " << value.value <<  std::endl;
        }
    }

    void on_exception(const IException& exception) override
    {
        // A scheduled request produced an exception
        std::cout << "Error: " << exception.get_message() << " received." << std::endl;
    }

    void on_timeout() override
    {
        // A scheduled request timed-out
        std::cout << "Timeout" << std::endl;
    }
};

int main(int argc, char* argv[])
{
    // Create a console logger
    auto logger = modbus::LoggerFactory::create_console_logger("Hello");

    {
        std::cout << "Start" << std::endl;
        {
            // Create the modbus manager
            // This will create the necessary background threads, initialize Asio io_services
            std::unique_ptr<IModbusManager> modbusManager = IModbusManager::create(logger);

            // Create a TCP channel
            // Each channel has its own Executor with a strand to avoid many multithreading issues
            auto channel = modbusManager->create_tcp_channel("Example channel", Ipv4Endpoint{ "127.0.0.1", 502 },
                ScheduleFactory::create_periodic_schedule(std::chrono::seconds(5)));

            // Create a session with a specific unit identifier
            // Users will mainly play with the session to obtain what they want
            auto session = channel->create_session(UnitIdentifier{0x01},
                std::chrono::seconds(3),
                std::make_shared<MySessionResponseHandler>());

            // Schedule a recurring request
            // All the scheduled requests will be handled by the ISessionResponseHandler registered on session creation
            ReadHoldingRegistersRequest req{ 0x0024, 59 };
            session->schedule_request(req, ScheduleFactory::create_periodic_schedule(std::chrono::seconds(2)));

            while (true)
            {
                char cmd;
                std::cin >> cmd;

                switch (cmd)
                {
                case 'a':
                {
                    // Send a request and print the result
                    // You can override the default timeout value set when creating the session
                    ReadHoldingRegistersRequest req{ 0x0024, 59 };
                    session->send_request(req, std::chrono::seconds(3), [](const Expected<ReadHoldingRegistersResponse>& response) {
                        // If the exception is set, then an error occured
                        if (!response.is_valid())
                        {
                            std::cout << response.get_exception<IException>().get_message() << std::endl;
                            return;
                        }

                        // Otherwise, everything went good and the response is available
                        for (auto& value : response.get().get_values())
                        {
                            std::cout << value.address << ": " << value.value << std::endl;
                        }
                    });
                    break;
                }
                case 'w':
                {
                    WriteMultipleRegistersRequest req{ 0x0024 };
                    req.add_register(11);
                    req.add_register(22);
                    req.add_register(33);

                    session->send_request(req, [](const Expected<WriteMultipleRegistersResponse>& response) {
                        if (!response.is_valid())
                        {
                            std::cout << response.get_exception<IException>().get_message() << std::endl;
                            return;
                        }

                        std::cout << "Write result= " << response.get().get_starting_address() << ": " << response.get().get_qty_of_registers() << std::endl;
                    });
                    break;
                }
                case 'q':
                    // Quit
                    goto end;

                default:
                    break;
                }

            }
        }
    }

end:
    return 0;
}

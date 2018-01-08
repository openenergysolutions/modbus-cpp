#include <iostream>
#include <memory>

#include "modbus/IModbusManager.h"
#include "modbus/channel/IChannel.h"
#include "modbus/Ipv4Endpoint.h"
#include "modbus/session/ISession.h"
#include "modbus/ISessionResponseHandler.h"
#include "modbus/ISchedule.h"
#include "modbus/ScheduleFactory.h"
#include "modbus/TimeoutException.h"

#include "modbus/logging/LoggerFactory.h"

#include "modbus/messages/Exception.h"
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"

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

    void on_exception(const Exception& exception) override
    {
        // A scheduled request produced an exception
        std::cout << "Error: " << exception.get_type() << " received." << std::endl;
    }

    void on_timeout() override
    {
        // A scheduled request timed-out
        std::cout << "Timeout" << std::endl;
    }
};

/*#include <array>
int main(int argc, char* argv[])
{
    std::cout << "Start" << std::endl;
    {
        auto test = std::make_shared<std::array<int, 1000000>>();
        test->at(54) = 28;
        std::cout << test->at(54) << std::endl;
    }
    std::cout << "End" << std::endl;
    
}*/

int main(int argc, char* argv[])
{
    // Create a console logger
    auto logger = modbus::LoggerFactory::create_console_logger("Hello");

    std::cout << "Start" << std::endl;
    {
        // Create the modbus manager
        // This will create the necessary background threads, initialize Asio io_services
        std::unique_ptr<IModbusManager> modbusManager = IModbusManager::create(logger);

        // Create a TCP channel
        // Each channel has its own Executor with a strand to avoid many multithreading issues
        auto channel = modbusManager->create_tcp_channel(Ipv4Endpoint{ "127.0.0.1", 8888 },
            ScheduleFactory::create_periodic_schedule(std::chrono::seconds(5)));

        // Create a session with a specific unit identifier
        // Users will mainly play with the session to obtain what they want
        auto session = channel->create_session(UnitIdentifier::default_unit_identifier(),
            std::chrono::seconds(3),
            std::make_shared<MySessionResponseHandler>());

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
                ReadHoldingRegistersRequest req{ 0x0024, 3 };
                session->send_request(req, std::chrono::seconds(3), [](const Expected<ReadHoldingRegistersResponse>& response) {
                    // If the exception is set, then an error occured
                    if (!response.is_valid())
                    {
                        if (response.has_exception<Exception>())
                        {
                            auto e = response.get_exception<Exception>();
                            std::cout << "Modbus exception: " << e.get_type() << " plz help" << std::endl;
                        }
                        if (response.has_exception<TimeoutException>())
                        {
                            std::cout << "Timeout was reached" << std::endl;
                        }
                        else
                        {
                            std::cout << "Unknown exception" << std::endl;
                        }

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
            case 'q':
                // Quit
                goto end;

            default:
                break;
            }

        }

        // Schedule a recurring request
        // All the scheduled requests will be handled by the ISessionResponseHandler registered on session creation
        //session->schedule_request(req, ScheduleFactory::create_periodic_schedule(std::chrono::seconds(5)));

        // Ignore this line, this is just a test
        //modbusManager->run();
    }

end:
    std::cout << "The end." << std::endl;

    return 0;
}

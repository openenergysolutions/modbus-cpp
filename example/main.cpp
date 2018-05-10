#include <iostream>
#include <memory>

#include "modbus/IModbusManager.h"
#include "modbus/logging/LoggerFactory.h"

using namespace modbus;

class MySessionResponseHandler : public ISessionResponseHandler
{
public:
    void on_response(const ReadHoldingRegistersResponse& response) override
    {
        // A scheduled response was received
        for(auto& value : response.values)
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
                                           std::chrono::seconds(3),
                                           std::make_shared<MySessionResponseHandler>());

    // Schedule a recurring request
    // All the scheduled requests will be handled by the ISessionResponseHandler registered on session creation
    ReadHoldingRegistersRequest req{ 0x0024, 59 };
    auto scheduled_req = session->schedule_request(req, std::chrono::seconds(2));

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
        case 'c':
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
        case 'z':
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

    return 0;
}

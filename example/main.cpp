#include <iostream>
#include <memory>

#include "modbus/IModbusManager.h"
#include "modbus/IChannel.h"
#include "modbus/ISchedule.h"
#include "modbus/ISession.h"
#include "modbus/ISessionResponseHandler.h"

#include "modbus/logging/LoggerFactory.h"

#include "modbus/messages/Exception.h"
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"

using namespace modbus;

class MySessionResponseHandler : public ISessionResponseHandler
{
public:
    void OnResponse(const ReadHoldingRegistersResponse& response)
    {
        // A scheduled response was received
        for(auto& value : response.GetValues())
        {
            std::cout << value.address << ": " << value.value <<  std::endl;
        }
    }

    void OnException(const Exception& exception)
    {
        // A scheduled request produced an exception
        std::cout << "Error: " << exception.GetExceptionType() << " received." << std::endl;
    }
};

int main(int argc, char* argv[])
{
    // Create a console logger
    auto logger = modbus::LoggerFactory().CreateConsoleLogger("Hello");

    // Create the modbus manager
    // This will create the necessary background threads, initialize Asio io_services
    std::unique_ptr<IModbusManager> modbusManager = IModbusManager::Create(logger);

    // Create a TCP channel
    // Each channel has its own Executor with a strand to avoid many multithreading issues
    std::unique_ptr<IChannel> channel = modbusManager->CreateTcpChannel();

    // Create a session with a specific unit identifier
    // Users will mainly play with the session to obtain what they want
    std::unique_ptr<ISession> session = channel->CreateSession(UnitIdentifier::Default(),
                                                               openpal::TimeDuration::seconds(3),
                                                               std::make_shared<MySessionResponseHandler>());

    // Send a request and print the result
    ReadHoldingRegistersRequest req(0x0024, 3);
    session->SendRequest(req, [](ReadHoldingRegistersResponse res, Exception e) {
        // If the exception is set, then an error occured (similar to Asio)
        if(e)
        {
            std::cout << "Error: " << e.GetExceptionType() << " plz help" << std::endl;
            return;
        }

        // Otherwise, everything went good and the response is available
        for(auto& value : res.GetValues())
        {
            std::cout << value.address << ": " << value.value <<  std::endl;
        }
    });

    // Schedule a recurring request
    // All the scheduled requests will be handled by the ISessionResponseHandler registered on session creation
    session->ScheduleRequest(req, std::make_unique<PeriodicSchedule>(openpal::TimeDuration::seconds(5)));

    // Ignore this line, this is just a test
    modbusManager->run();

    return 0;
}

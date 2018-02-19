#include <iostream>
#include <memory>

#include "spdlog/spdlog.h"

#include "modbus/IModbusManager.h"
#include "modbus/Ipv4Endpoint.h"
#include "modbus/channel/IChannel.h"
#include "modbus/exceptions/IException.h"
#include "modbus/logging/LoggerFactory.h"
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/session/ISession.h"
#include "modbus/session/ISessionResponseHandler.h"

using namespace modbus;

class MySessionResponseHandler : public ISessionResponseHandler
{
public:
    MySessionResponseHandler()
        : m_logger{ spdlog::stdout_color_mt("Test") }
    {

    }

    void on_response(const ReadHoldingRegistersResponse& response) override
    {
        m_logger->trace("Received response");
    }

    void on_exception(const IException& exception) override
    {
        m_logger->error("Exception: {}", exception.get_message());
    }

    void on_timeout() override
    {
        m_logger->warn("Timeout");
    }

private:
    std::shared_ptr<spdlog::logger> m_logger;
};

int main(int argc, char* argv[])
{
    auto logger = LoggerFactory::create_null_logger("Hello");
    auto session_response_handler = std::make_shared<MySessionResponseHandler>();

    std::unique_ptr<IModbusManager> modbusManager = IModbusManager::create(logger, 8);

    for(size_t i = 0; i < 1000; ++i)
    {
        unsigned int port = i + 8000;
        std::string name = "Channel " + std::to_string(port);
        auto channel = modbusManager->create_tcp_channel(name, Ipv4Endpoint{ "10.248.239.56", port });

        auto session = channel->create_session(UnitIdentifier::default_unit_identifier(),
                                               std::chrono::seconds(3),
                                               session_response_handler);

        ReadHoldingRegistersRequest req{ 0x0000, 16 };
        session->schedule_request(req, std::chrono::seconds(2));
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

#include "catch.hpp"
#include "fakeit.hpp"

#include "session/SessionImpl.h"

#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/Expected.h"
#include "modbus/messages/Address.h"
#include "modbus/channel/IChannel.h"
#include "modbus/messages/IRequest.h"
#include "modbus/logging/LoggerFactory.h"
#include "modbus/ISessionResponseHandler.h"

using namespace fakeit;
using namespace modbus;

TEST_CASE("SessionImpl")
{
    auto executor = Mock<openpal::IExecutor>();
    auto executor_ptr = std::shared_ptr<openpal::IExecutor>(&executor.get(), [](...) {});

    auto logger = LoggerFactory::create_console_logger("test");

    auto channel = Mock<IChannel>();
    Fake(Method(channel, send_request));
    auto channel_ptr = std::shared_ptr<IChannel>(&channel.get(),[](...){});

    auto session_handler = Mock<ISessionResponseHandler>();
    auto session_handler_ptr = std::shared_ptr<ISessionResponseHandler>(&session_handler.get(), [](...) {});

    const auto unit_identifier = UnitIdentifier::default_unit_identifier();
    const auto timeout = std::chrono::seconds(5);

    SessionImpl session{ executor_ptr, logger, channel_ptr, unit_identifier, std::chrono::seconds(5), session_handler_ptr };

    /*SECTION("ReadHoldingRegister sends request to channel")
    {
        ReadHoldingRegistersRequest req{ Address{0x0001}, 5 };
        session.send_request(req, [=](const Expected<ReadHoldingRegistersResponse>&)
        {
            
        });
        Verify(Method(channel, send_request).Matching([=](const UnitIdentifier& r_unit_identifier,
                                                         const IRequest& r_request,
                                                         const openpal::duration_t& r_timeout,
                                                         ResponseHandler<openpal::rseq_t> r_response_handler)
        {
            return r_unit_identifier == unit_identifier &&
                r_timeout == timeout;
        })).Exactly(Once);
    }*/
}

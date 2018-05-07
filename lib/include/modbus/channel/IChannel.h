#ifndef MODBUS_ICHANNEL_H
#define MODBUS_ICHANNEL_H

#include <memory>
#include "exe4cpp/Typedefs.h"
#include "modbus/channel/UnitIdentifier.h"

namespace modbus
{

class ISession;
class ISessionResponseHandler;

class IChannel : public std::enable_shared_from_this<IChannel>
{
public:
    virtual ~IChannel() = default;

    virtual std::shared_ptr<ISession> create_session(const UnitIdentifier& unit_identifier,
                                                     const exe4cpp::duration_t& default_timeout,
                                                     std::shared_ptr<ISessionResponseHandler> session_response_handler) = 0;

    virtual void shutdown() = 0;
};

} // namespace modbus

#endif //MODBUS_ICHANNEL_H

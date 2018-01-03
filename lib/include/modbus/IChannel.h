#ifndef MODBUS_ICHANNEL_H
#define MODBUS_ICHANNEL_H

#include <memory>

#include "openpal/executor/Typedefs.h"

#include "modbus/UnitIdentifier.h"

namespace modbus
{

class ISession;
class ISessionResponseHandler;

class IChannel
{
public:
    virtual ~IChannel() = default;

    virtual std::unique_ptr<ISession> create_session(UnitIdentifier unit_identifier,
                                                     const openpal::duration_t& default_timeout,
                                                     std::shared_ptr<ISessionResponseHandler> session_response_handler) = 0;
};

} // namespace modbus

#endif //MODBUS_ICHANNEL_H

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

    virtual std::unique_ptr<ISession> CreateSession(UnitIdentifier unitIdentifier,
                                                    const openpal::duration_t& defaultTimeout,
                                                    std::shared_ptr<ISessionResponseHandler> sessionResponseHandler) = 0;
};

} // namespace modbus

#endif //MODBUS_ICHANNEL_H

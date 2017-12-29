#ifndef MODBUS_ICHANNEL_H
#define MODBUS_ICHANNEL_H

#include <memory>

#include "openpal/executor/TimeDuration.h"

#include "modbus/ISessionResponseHandler.h"
#include "modbus/ISession.h"
#include "modbus/UnitIdentifier.h"

namespace modbus
{

class IChannel
{
public:
    virtual ~IChannel() = default;

    virtual std::unique_ptr<ISession> CreateSession(UnitIdentifier unitIdentifier,
                                                    openpal::TimeDuration defaultTimeout,
                                                    std::shared_ptr<ISessionResponseHandler> sessionResponseHandler) = 0;
};

} // namespace modbus

#endif //MODBUS_ICHANNEL_H

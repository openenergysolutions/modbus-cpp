#ifndef MODBUS_ICHANNELIMPL_H
#define MODBUS_ICHANNELIMPL_H

#include "ser4cpp/container/SequenceTypes.h"

#include "modbus/channel/IChannel.h"
#include "modbus/ResponseHandler.h"

namespace modbus
{

class IRequest;
class ISession;
class ISessionResponseHandler;

class IChannelImpl : public IChannel
{
public:
    virtual ~IChannelImpl() = default;

    virtual void send_request(const UnitIdentifier& unit_identifier,
                              const IRequest& request,
                              const exe4cpp::duration_t& timeout,
                              ResponseHandler<ser4cpp::rseq_t> response_handler) = 0;
};

} // namespace modbus

#endif //MODBUS_ICHANNELIMPL_H

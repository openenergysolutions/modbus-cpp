#ifndef MODBUS_ICHANNEL_H
#define MODBUS_ICHANNEL_H

#include <memory>

#include "openpal/container/SequenceTypes.h"
#include "openpal/executor/Typedefs.h"

#include "modbus/ResponseHandler.h"
#include "modbus/channel/UnitIdentifier.h"

namespace modbus
{

class IRequest;
class ISession;
class ISessionResponseHandler;

class IChannel : public std::enable_shared_from_this<IChannel>
{
public:
    virtual ~IChannel() = default;

    virtual std::shared_ptr<ISession> create_session(const UnitIdentifier& unit_identifier,
                                                     const openpal::duration_t& default_timeout,
                                                     std::shared_ptr<ISessionResponseHandler> session_response_handler) = 0;

    virtual void send_request(const UnitIdentifier& unit_identifier,
                              const IRequest& request,
                              const openpal::duration_t& timeout,
                              ResponseHandler<openpal::rseq_t> response_handler) = 0;

    virtual void shutdown() = 0;
};

} // namespace modbus

#endif //MODBUS_ICHANNEL_H

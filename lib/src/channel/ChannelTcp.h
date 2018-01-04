#ifndef MODBUS_CHANNELTCP_H
#define MODBUS_CHANNELTCP_H

#include "modbus/channel/IChannel.h"

namespace modbus
{

class ChannelTcp : public IChannel
{
    std::unique_ptr<ISession> create_session(const UnitIdentifier& unit_identifier,
                                             const openpal::duration_t& default_timeout,
                                             std::shared_ptr<ISessionResponseHandler> session_response_handler) override;

    void send_request(const UnitIdentifier& unit_identifier,
                      const IRequest& request,
                      const openpal::duration_t& timeout,
                      ResponseHandler<openpal::rseq_t> response_handler) override;
};

} // namespace modbus

#endif //MODBUS_CHANNELTCP_H

#ifndef MODBUS_SESSIONIMPL_H
#define MODBUS_SESSIONIMPL_H

#include <memory>

#include "modbus/session/ISession.h"
#include "modbus/channel/UnitIdentifier.h"

namespace modbus
{

class Logger;
class IChannel;
class ISessionResponseHandler;
class UnitIdentifier;

class SessionImpl : public ISession
{
public:
    SessionImpl(std::shared_ptr<Logger> logger,
                std::shared_ptr<IChannel> channel,
                const UnitIdentifier& unit_identifier,
                const openpal::duration_t& default_timeout,
                std::shared_ptr<ISessionResponseHandler> session_response_handler);

    // One-time requests
    void send_request(const ReadHoldingRegistersRequest& request,
                      ResponseHandler<ReadHoldingRegistersResponse> handler) override;
    void send_request(const ReadHoldingRegistersRequest& request,
                      const openpal::duration_t& timeout,
                      ResponseHandler<ReadHoldingRegistersResponse> handler) override;

    void send_request(const ReadInputRegistersRequest& request,
                      ResponseHandler<ReadInputRegistersResponse> handler) override;
    void send_request(const ReadInputRegistersRequest& request,
                      const openpal::duration_t& timeout,
                      ResponseHandler<ReadInputRegistersResponse> handler) override;

    // Scheduled requests
    void schedule_request(const ReadHoldingRegistersRequest& request,
                          std::unique_ptr<ISchedule> schedule) override;
    void schedule_request(const ReadHoldingRegistersRequest& request,
                          const openpal::duration_t& timeout,
                          std::unique_ptr<ISchedule> schedule) override;

    void schedule_request(const ReadInputRegistersRequest& request,
                          std::unique_ptr<ISchedule> schedule) override;
    void schedule_request(const ReadInputRegistersRequest& request,
                          const openpal::duration_t& timeout,
                          std::unique_ptr<ISchedule> schedule) override;

private:
    template<typename TRequest, typename TResponse>
    void send_request(const TRequest& request,
                      ResponseHandler<TResponse> handler)
    {

    }

    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<IChannel> m_channel;
    UnitIdentifier m_unit_identifier;
    openpal::duration_t m_default_timeout;
    std::shared_ptr<ISessionResponseHandler> m_session_response_handler;
};

} // namespace modbus

#endif //MODBUS_SESSIONIMPL_H

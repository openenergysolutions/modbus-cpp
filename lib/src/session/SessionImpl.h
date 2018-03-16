#ifndef MODBUS_SESSIONIMPL_H
#define MODBUS_SESSIONIMPL_H

#include <memory>
#include <vector>
#include "loopser/executor/IExecutor.h"
#include "loopser/executor/Timer.h"
#include "modbus/channel/UnitIdentifier.h"
#include "modbus/session/IScheduledRequest.h"
#include "modbus/session/ISession.h"

namespace modbus
{

class Logger;
class IChannel;
class ISessionResponseHandler;
class UnitIdentifier;

class SessionImpl final : public ISession
{
public:
    SessionImpl(std::shared_ptr<loopser::IExecutor> executor,
                std::shared_ptr<Logger> logger,
                std::shared_ptr<IChannel> channel,
                const UnitIdentifier& unit_identifier,
                const loopser::duration_t& default_timeout,
                std::shared_ptr<ISessionResponseHandler> session_response_handler);
    ~SessionImpl() = default;

    void shutdown() override;

    // One-time requests
    void send_request(const ReadHoldingRegistersRequest& request,
                      ResponseHandler<ReadHoldingRegistersResponse> handler) override;
    void send_request(const ReadHoldingRegistersRequest& request,
                      const loopser::duration_t& timeout,
                      ResponseHandler<ReadHoldingRegistersResponse> handler) override;

    void send_request(const ReadInputRegistersRequest& request,
                      ResponseHandler<ReadInputRegistersResponse> handler) override;
    void send_request(const ReadInputRegistersRequest& request,
                      const loopser::duration_t& timeout,
                      ResponseHandler<ReadInputRegistersResponse> handler) override;

    void send_request(const WriteSingleRegisterRequest& request,
                      ResponseHandler<WriteSingleRegisterResponse> handler) override;
    void send_request(const WriteSingleRegisterRequest& request,
                      const loopser::duration_t& timeout,
                      ResponseHandler<WriteSingleRegisterResponse> handler) override;

    void send_request(const WriteMultipleRegistersRequest& request,
                      ResponseHandler<WriteMultipleRegistersResponse> handler) override;
    void send_request(const WriteMultipleRegistersRequest& request,
                      const loopser::duration_t& timeout,
                      ResponseHandler<WriteMultipleRegistersResponse> handler) override;


    // Scheduled requests
    std::shared_ptr<IScheduledRequest> schedule_request(const ReadHoldingRegistersRequest& request,
                                                        const loopser::duration_t& frequency) override;
    std::shared_ptr<IScheduledRequest> schedule_request(const ReadHoldingRegistersRequest& request,
                                                        const loopser::duration_t& timeout,
                                                        const loopser::duration_t& frequency) override;

    std::shared_ptr<IScheduledRequest> schedule_request(const ReadInputRegistersRequest& request,
                                                        const loopser::duration_t& frequency) override;
    std::shared_ptr<IScheduledRequest> schedule_request(const ReadInputRegistersRequest& request,
                                                        const loopser::duration_t& timeout,
                                                        const loopser::duration_t& frequency) override;


    loopser::Timer start(const loopser::duration_t& duration, const loopser::action_t& action) override;
    loopser::Timer start(const loopser::steady_time_t& expiration, const loopser::action_t& action) override;

private:
    template<typename TRequest, typename TResponse>
    void meta_send_request(const TRequest& request,
                           const loopser::duration_t& timeout,
                           ResponseHandler<TResponse> handler);

    template<typename TRequest, typename TResponse>
    std::shared_ptr<IScheduledRequest> meta_schedule_request(const TRequest& request,
                                                             const loopser::duration_t& timeout,
                                                             const loopser::duration_t& frequency);

    std::shared_ptr<loopser::IExecutor> m_executor;
    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<IChannel> m_channel;
    UnitIdentifier m_unit_identifier;
    loopser::duration_t m_default_timeout;
    std::shared_ptr<ISessionResponseHandler> m_session_response_handler;

    std::vector<std::shared_ptr<IScheduledRequest>> m_scheduled_requests;
};

} // namespace modbus

#endif //MODBUS_SESSIONIMPL_H

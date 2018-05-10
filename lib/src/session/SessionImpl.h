#ifndef MODBUS_SESSIONIMPL_H
#define MODBUS_SESSIONIMPL_H

#include <memory>
#include <vector>
#include "exe4cpp/IExecutor.h"
#include "modbus/channel/UnitIdentifier.h"
#include "modbus/session/IScheduledRequest.h"
#include "modbus/session/ISession.h"

namespace modbus
{

class Logger;
class IChannelImpl;
class ISessionResponseHandler;
class UnitIdentifier;

class SessionImpl final : public ISession
{
public:
    SessionImpl(std::shared_ptr<exe4cpp::IExecutor> executor,
                std::shared_ptr<Logger> logger,
                std::shared_ptr<IChannelImpl> channel,
                const UnitIdentifier& unit_identifier,
                const duration_t& default_timeout,
                std::shared_ptr<ISessionResponseHandler> session_response_handler);
    ~SessionImpl() = default;

    void shutdown() override;

    // One-time requests
    void send_request(const ReadHoldingRegistersRequest& request,
                      ResponseHandler<ReadHoldingRegistersResponse> handler) override;
    void send_request(const ReadHoldingRegistersRequest& request,
                      const duration_t& timeout,
                      ResponseHandler<ReadHoldingRegistersResponse> handler) override;

    void send_request(const ReadInputRegistersRequest& request,
                      ResponseHandler<ReadInputRegistersResponse> handler) override;
    void send_request(const ReadInputRegistersRequest& request,
                      const duration_t& timeout,
                      ResponseHandler<ReadInputRegistersResponse> handler) override;

    void send_request(const WriteSingleRegisterRequest& request,
                      ResponseHandler<WriteSingleRegisterResponse> handler) override;
    void send_request(const WriteSingleRegisterRequest& request,
                      const duration_t& timeout,
                      ResponseHandler<WriteSingleRegisterResponse> handler) override;

    void send_request(const WriteMultipleRegistersRequest& request,
                      ResponseHandler<WriteMultipleRegistersResponse> handler) override;
    void send_request(const WriteMultipleRegistersRequest& request,
                      const duration_t& timeout,
                      ResponseHandler<WriteMultipleRegistersResponse> handler) override;


    // Scheduled requests
    std::shared_ptr<IScheduledRequest> schedule_request(const ReadHoldingRegistersRequest& request,
                                                        const duration_t& frequency) override;
    std::shared_ptr<IScheduledRequest> schedule_request(const ReadHoldingRegistersRequest& request,
                                                        const duration_t& timeout,
                                                        const duration_t& frequency) override;

    std::shared_ptr<IScheduledRequest> schedule_request(const ReadInputRegistersRequest& request,
                                                        const duration_t& frequency) override;
    std::shared_ptr<IScheduledRequest> schedule_request(const ReadInputRegistersRequest& request,
                                                        const duration_t& timeout,
                                                        const duration_t& frequency) override;


    std::unique_ptr<ITimer> start(const duration_t& duration, const action_t& action) override;
    std::unique_ptr<ITimer> start(const steady_time_t& expiration, const action_t& action) override;

private:
    template<typename TRequestImpl, typename TResponseImpl, typename TRequest, typename TResponse>
    void meta_send_request(const TRequest& request,
                           const duration_t& timeout,
                           ResponseHandler<TResponse> handler);

    template<typename TRequest, typename TResponse>
    std::shared_ptr<IScheduledRequest> meta_schedule_request(const TRequest& request,
                                                             const duration_t& timeout,
                                                             const duration_t& frequency);

    std::shared_ptr<exe4cpp::IExecutor> m_executor;
    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<IChannelImpl> m_channel;
    UnitIdentifier m_unit_identifier;
    duration_t m_default_timeout;
    std::shared_ptr<ISessionResponseHandler> m_session_response_handler;

    std::vector<std::shared_ptr<IScheduledRequest>> m_scheduled_requests;
};

} // namespace modbus

#endif //MODBUS_SESSIONIMPL_H

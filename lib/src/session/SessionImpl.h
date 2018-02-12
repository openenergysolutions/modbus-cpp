#ifndef MODBUS_SESSIONIMPL_H
#define MODBUS_SESSIONIMPL_H

#include <memory>
#include <vector>

#include "openpal/executor/IExecutor.h"
#include "openpal/executor/Timer.h"
#include "modbus/session/ISession.h"
#include "modbus/channel/UnitIdentifier.h"
#include "session/IScheduledRequest.h"

namespace modbus
{

class Logger;
class IChannel;
class ISessionResponseHandler;
class UnitIdentifier;

class SessionImpl : public ISession
{
public:
    SessionImpl(std::shared_ptr<openpal::IExecutor> executor,
                std::shared_ptr<Logger> logger,
                std::shared_ptr<IChannel> channel,
                const UnitIdentifier& unit_identifier,
                const openpal::duration_t& default_timeout,
                std::shared_ptr<ISessionResponseHandler> session_response_handler);
    ~SessionImpl();

    void shutdown() override;

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

    void send_request(const WriteSingleRegisterRequest& request,
                      ResponseHandler<WriteSingleRegisterResponse> handler) override;
    void send_request(const WriteSingleRegisterRequest& request,
                      const openpal::duration_t& timeout,
                      ResponseHandler<WriteSingleRegisterResponse> handler) override;

    void send_request(const WriteMultipleRegistersRequest& request,
                      ResponseHandler<WriteMultipleRegistersResponse> handler) override;
    void send_request(const WriteMultipleRegistersRequest& request,
                      const openpal::duration_t& timeout,
                      ResponseHandler<WriteMultipleRegistersResponse> handler) override;


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
    void meta_send_request(const TRequest& request,
                           const openpal::duration_t& timeout,
                           ResponseHandler<TResponse> handler);

    template<typename TRequest, typename TResponse>
    void meta_schedule_request(const TRequest& request,
                               const openpal::duration_t& timeout,
                               std::unique_ptr<ISchedule> schedule);

    std::shared_ptr<openpal::IExecutor> m_executor;
    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<IChannel> m_channel;
    UnitIdentifier m_unit_identifier;
    openpal::duration_t m_default_timeout;
    std::shared_ptr<ISessionResponseHandler> m_session_response_handler;
    std::vector<std::shared_ptr<IScheduledRequest>> m_scheduled_requests;
};

} // namespace modbus

#endif //MODBUS_SESSIONIMPL_H

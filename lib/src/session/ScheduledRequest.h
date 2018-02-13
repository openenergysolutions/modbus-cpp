#ifndef MODBUS_SCHEDULEDREQUEST_H
#define MODBUS_SCHEDULEDREQUEST_H

#include <memory>
#include "openpal/executor/IExecutor.h"
#include "openpal/executor/Timer.h"
#include "openpal/executor/Typedefs.h"
#include "modbus/session/IScheduledRequest.h"

namespace modbus
{

class ISession;
class ISessionResponseHandler;

template<typename TRequest, typename TResponse>
class ScheduledRequest : public IScheduledRequest
{
public:
    ScheduledRequest(std::shared_ptr<ISession> session,
                     std::shared_ptr<ISessionResponseHandler> session_response_handler,
                     std::shared_ptr<openpal::IExecutor> executor,
                     const TRequest& request,
                     const openpal::duration_t& timeout,
                     const openpal::duration_t& frequency);
    ~ScheduledRequest() = default;

    void start() override;
    void stop() override;
    bool is_running() const override;

    void set_frequency(const openpal::duration_t& frequency) override;
    openpal::duration_t get_frequency() const override;

private:
    void execute();

    std::shared_ptr<ISession> m_session;
    std::shared_ptr<ISessionResponseHandler> m_session_response_handler;
    std::shared_ptr<openpal::IExecutor> m_executor;
    TRequest m_request;
    openpal::duration_t m_timeout;
    openpal::duration_t m_frequency;

    bool m_running;
    openpal::Timer m_timer;
};

} // namespace modbus

#include "session/ScheduledRequest.hpp"

#endif //MODBUS_SCHEDULEDREQUEST_H

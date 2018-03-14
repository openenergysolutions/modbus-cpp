#ifndef MODBUS_SCHEDULEDREQUEST_H
#define MODBUS_SCHEDULEDREQUEST_H

#include <memory>
#include "loopser/executor/IExecutor.h"
#include "loopser/executor/Timer.h"
#include "loopser/executor/Typedefs.h"
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
                     std::shared_ptr<loopser::IExecutor> executor,
                     const TRequest& request,
                     const loopser::duration_t& timeout,
                     const loopser::duration_t& frequency);
    ~ScheduledRequest() = default;

    void start() override;
    void stop() override;
    bool is_running() const override;

    void set_frequency(const loopser::duration_t& frequency) override;
    loopser::duration_t get_frequency() const override;

private:
    void execute();

    std::shared_ptr<ISession> m_session;
    std::shared_ptr<ISessionResponseHandler> m_session_response_handler;
    std::shared_ptr<loopser::IExecutor> m_executor;
    TRequest m_request;
    loopser::duration_t m_timeout;
    loopser::duration_t m_frequency;

    bool m_running;
    loopser::Timer m_timer;
};

} // namespace modbus

#include "session/ScheduledRequest.hpp"

#endif //MODBUS_SCHEDULEDREQUEST_H

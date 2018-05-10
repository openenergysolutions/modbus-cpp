#ifndef MODBUS_SCHEDULEDREQUEST_H
#define MODBUS_SCHEDULEDREQUEST_H

#include <memory>
#include "exe4cpp/IExecutor.h"
#include "exe4cpp/Timer.h"
#include "modbus/Typedefs.h"
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
                     std::shared_ptr<exe4cpp::IExecutor> executor,
                     const TRequest& request,
                     const duration_t& timeout,
                     const duration_t& frequency);
    ~ScheduledRequest() = default;

    void start() override;
    void stop() override;
    bool is_running() const override;

    void set_frequency(const duration_t& frequency) override;
    duration_t get_frequency() const override;

private:
    void execute();

    std::shared_ptr<ISession> m_session;
    std::shared_ptr<ISessionResponseHandler> m_session_response_handler;
    std::shared_ptr<exe4cpp::IExecutor> m_executor;
    TRequest m_request;
    duration_t m_timeout;
    duration_t m_frequency;

    bool m_running;
    exe4cpp::Timer m_timer;
};

} // namespace modbus

#include "session/ScheduledRequest.hpp"

#endif //MODBUS_SCHEDULEDREQUEST_H

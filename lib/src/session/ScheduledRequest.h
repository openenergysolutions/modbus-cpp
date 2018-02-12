#ifndef MODBUS_SCHEDULEDREQUEST_H
#define MODBUS_SCHEDULEDREQUEST_H

#include <memory>
#include "openpal/executor/IExecutor.h"
#include "openpal/executor/Timer.h"
#include "openpal/executor/Typedefs.h"
#include "session/IScheduledRequest.h"

namespace modbus
{

class ISchedule;
class ISession;
class ISessionResponseHandler;

template<typename TRequest, typename TResponse>
class ScheduledRequest : public IScheduledRequest
{
public:
    ScheduledRequest(ISession* session,
                     ISessionResponseHandler* session_response_handler,
                     std::shared_ptr<openpal::IExecutor> executor,
                     const TRequest& request,
                     const openpal::duration_t& timeout,
                     std::unique_ptr<ISchedule> schedule);

    void start() override;
    void cancel() override;

private:
    ISession* m_session;
    ISessionResponseHandler* m_session_response_handler;
    std::shared_ptr<openpal::IExecutor> m_executor;
    TRequest m_request;
    openpal::duration_t m_timeout;
    std::unique_ptr<ISchedule> m_schedule;
    openpal::Timer m_timer;
};

} // namespace modbus

#include "session/ScheduledRequest.hpp"

#endif //MODBUS_SCHEDULEDREQUEST_H

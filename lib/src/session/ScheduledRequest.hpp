#ifndef MODBUS_SCHEDULEDREQUEST_HPP
#define MODBUS_SCHEDULEDREQUEST_HPP

#include "modbus/ISchedule.h"
#include "modbus/Expected.h"
#include "modbus/exceptions/IException.h"
#include "modbus/exceptions/TimeoutException.h"
#include "modbus/ISessionResponseHandler.h"
#include "modbus/session/ISession.h"

namespace modbus
{

template<typename TRequest, typename TResponse>
ScheduledRequest<TRequest, TResponse>::ScheduledRequest(ISession* session,
                                                        ISessionResponseHandler* session_response_handler,
                                                        std::shared_ptr<openpal::IExecutor> executor,
                                                        const TRequest& request,
                                                        const openpal::duration_t& timeout,
                                                        std::unique_ptr<ISchedule> schedule)
    : m_session{session},
      m_session_response_handler{session_response_handler},
      m_executor{executor},
      m_request{request},
      m_timeout{timeout},
      m_schedule{std::move(schedule)},
      m_timer{nullptr}
{

}

template<typename TRequest, typename TResponse>
void ScheduledRequest<TRequest, TResponse>::start()
{
    // Send the request
    m_session->send_request(m_request, m_timeout, [=, self = shared_from_this()](const Expected<TResponse>& response) {

        // Call the response handler
        if(m_session_response_handler)
        {
            if(!response.is_valid())
            {
                m_session_response_handler->on_exception(response.template get_exception<IException>());
            }
            else
            {
                m_session_response_handler->on_response(response.get());
            }
        }

        // Calculate next execution
        auto current_time = m_executor->get_time();
        if(response.is_valid())
        {
            m_schedule->on_success(current_time);
        }
        else if(response.template has_exception<TimeoutException>())
        {
            m_schedule->on_timeout(current_time);
        }
        else
        {
            m_schedule->on_failure(current_time);
        }

        // Start the timer for the next execution
        m_timer = m_executor->start(m_schedule->get_next_execution(), [=, self2 = self]() {
            start();
        });
    });
}

template<typename TRequest, typename TResponse>
void ScheduledRequest<TRequest, TResponse>::cancel()
{
    m_timer.cancel();
}

} // namespace modbus

#endif //MODBUS_SCHEDULEDREQUEST_HPP

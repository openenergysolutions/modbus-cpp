/*
 * Copyright 2017-2018 Duke Energy Corporation and Open Energy Solutions, Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MODBUS_SCHEDULEDREQUEST_HPP
#define MODBUS_SCHEDULEDREQUEST_HPP

#include "modbus/Expected.h"
#include "modbus/client/IClientSession.h"
#include "modbus/exceptions/IException.h"
#include "modbus/exceptions/TimeoutException.h"

namespace modbus
{

template<typename TRequest, typename TResponse>
ScheduledRequest<TRequest, TResponse>::ScheduledRequest(std::shared_ptr<IClientSession> session,
                                                        ResponseHandler<TResponse> handler,
                                                        std::shared_ptr<exe4cpp::IExecutor> executor,
                                                        const TRequest& request,
                                                        const duration_t& timeout,
                                                        const duration_t& frequency)
    : m_session{session},
      m_handler{handler},
      m_executor{executor},
      m_request{request},
      m_timeout{timeout},
      m_frequency{frequency},
      m_running{false},
      m_timer{nullptr}
{

}

template<typename TRequest, typename TResponse>
void ScheduledRequest<TRequest, TResponse>::start()
{
    m_executor->post([=, self = shared_from_this()]() {
        if(!m_running)
        {
            execute();
            m_running = true;
        }
    });
}

template<typename TRequest, typename TResponse>
void ScheduledRequest<TRequest, TResponse>::stop()
{
    m_executor->post([=, self = shared_from_this()]() {
        if(m_running)
        {
            m_timer.cancel();
            m_running = false;
        }
    });
}

template<typename TRequest, typename TResponse>
void ScheduledRequest<TRequest, TResponse>::set_frequency(const duration_t& frequency)
{
    m_executor->post([=, self = shared_from_this()]() {
        m_frequency = frequency;
    });
};

template<typename TRequest, typename TResponse>
duration_t ScheduledRequest<TRequest, TResponse>::get_frequency() const
{
    return m_frequency;
};

template<typename TRequest, typename TResponse>
bool ScheduledRequest<TRequest, TResponse>::is_running() const
{
    return m_running;
};

template<typename TRequest, typename TResponse>
void ScheduledRequest<TRequest, TResponse>::execute()
{
    // Send the request
    m_session->send_request(m_request, m_timeout, [=, self = shared_from_this()](const Expected<TResponse>& response) {

        // Call the response handler
        m_handler(response);

        if(is_running())
        {
            // Start the timer for the next execution
            m_timer = m_executor->start(m_frequency, [=, self2 = self]() {
                execute();
            });
        }
    });
}

} // namespace modbus

#endif //MODBUS_SCHEDULEDREQUEST_HPP

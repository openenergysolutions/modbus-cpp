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
#ifndef MODBUS_SCHEDULEDREQUEST_H
#define MODBUS_SCHEDULEDREQUEST_H

#include <memory>
#include "exe4cpp/IExecutor.h"
#include "exe4cpp/Timer.h"
#include "exe4cpp/Typedefs.h"
#include "modbus/Typedefs.h"
#include "modbus/client/IScheduledRequest.h"
#include "modbus/client/ResponseHandler.h"

namespace modbus
{

class IClientSession;

template<typename TRequest, typename TResponse>
class ScheduledRequest : public IScheduledRequest
{
public:
    ScheduledRequest(std::shared_ptr<IClientSession> session,
                     ResponseHandler<TResponse> handler,
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

    std::shared_ptr<IClientSession> m_session;
    ResponseHandler<TResponse> m_handler;
    std::shared_ptr<exe4cpp::IExecutor> m_executor;
    TRequest m_request;
    duration_t m_timeout;
    duration_t m_frequency;

    bool m_running;
    exe4cpp::Timer m_timer;
};

} // namespace modbus

#include "client/session/ScheduledRequest.hpp"

#endif //MODBUS_SCHEDULEDREQUEST_H

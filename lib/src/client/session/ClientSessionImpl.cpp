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
#include "client/session/ClientSessionImpl.h"

#include "TimerWrapper.h"
#include "client/channel/IClientChannelImpl.h"
#include "client/session/ScheduledRequest.h"
#include "messages/ReadCoilsRequestImpl.h"
#include "messages/ReadCoilsResponseImpl.h"
#include "messages/ReadDiscreteInputsRequestImpl.h"
#include "messages/ReadDiscreteInputsResponseImpl.h"
#include "messages/ReadHoldingRegistersRequestImpl.h"
#include "messages/ReadHoldingRegistersResponseImpl.h"
#include "messages/ReadInputRegistersRequestImpl.h"
#include "messages/ReadInputRegistersResponseImpl.h"
#include "messages/WriteMultipleCoilsRequestImpl.h"
#include "messages/WriteMultipleCoilsResponseImpl.h"
#include "messages/WriteMultipleRegistersRequestImpl.h"
#include "messages/WriteMultipleRegistersResponseImpl.h"
#include "messages/WriteSingleCoilRequestImpl.h"
#include "messages/WriteSingleCoilResponseImpl.h"
#include "messages/WriteSingleRegisterRequestImpl.h"
#include "messages/WriteSingleRegisterResponseImpl.h"

namespace modbus
{

ClientSessionImpl::ClientSessionImpl(std::shared_ptr<exe4cpp::IExecutor> executor,
                                     std::shared_ptr<Logger> logger,
                                     std::shared_ptr<IClientChannelImpl> channel,
                                     const UnitIdentifier& unit_identifier,
                                     const duration_t& default_timeout)
: m_executor{executor},
  m_logger{logger},
  m_channel{channel},
  m_unit_identifier{unit_identifier},
  m_default_timeout{default_timeout}
{

}

void ClientSessionImpl::shutdown()
{
    m_executor->post([=, self = shared_from_this()] {
        for(auto& req : m_scheduled_requests)
        {
            req->stop();
        }

        m_scheduled_requests.clear();
    });
}

void ClientSessionImpl::send_request(const ReadCoilsRequest& request,
                                     ResponseHandler<ReadCoilsResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void ClientSessionImpl::send_request(const ReadCoilsRequest& request,
                                     const duration_t& timeout,
                                     ResponseHandler<ReadCoilsResponse> handler)
{
    meta_send_request<ReadCoilsRequestImpl, ReadCoilsResponseImpl>(request, timeout, handler);
}

void ClientSessionImpl::send_request(const ReadDiscreteInputsRequest& request,
                                     ResponseHandler<ReadDiscreteInputsResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void ClientSessionImpl::send_request(const ReadDiscreteInputsRequest& request,
                                     const duration_t& timeout,
                                     ResponseHandler<ReadDiscreteInputsResponse> handler)
{
    meta_send_request<ReadDiscreteInputsRequestImpl, ReadDiscreteInputsResponseImpl>(request, timeout, handler);
}

void ClientSessionImpl::send_request(const ReadHoldingRegistersRequest& request,
                                     ResponseHandler<ReadHoldingRegistersResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void ClientSessionImpl::send_request(const ReadHoldingRegistersRequest& request,
                                     const duration_t& timeout,
                                     ResponseHandler<ReadHoldingRegistersResponse> handler)
{
    meta_send_request<ReadHoldingRegistersRequestImpl, ReadHoldingRegistersResponseImpl>(request, timeout, handler);
}

void ClientSessionImpl::send_request(const ReadInputRegistersRequest& request,
                                     ResponseHandler<ReadInputRegistersResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void ClientSessionImpl::send_request(const ReadInputRegistersRequest& request,
                                     const duration_t& timeout,
                                     ResponseHandler<ReadInputRegistersResponse> handler)
{
    meta_send_request<ReadInputRegistersRequestImpl, ReadInputRegistersResponseImpl>(request, timeout, handler);
}

void ClientSessionImpl::send_request(const WriteSingleCoilRequest& request,
                                     ResponseHandler<WriteSingleCoilResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void ClientSessionImpl::send_request(const WriteSingleCoilRequest& request,
                                     const duration_t& timeout,
                                     ResponseHandler<WriteSingleCoilResponse> handler)
{
    meta_send_request<WriteSingleCoilRequestImpl, WriteSingleCoilResponseImpl>(request, timeout, handler);
}

void ClientSessionImpl::send_request(const WriteSingleRegisterRequest& request,
                                     ResponseHandler<WriteSingleRegisterResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void ClientSessionImpl::send_request(const WriteSingleRegisterRequest& request,
                                     const duration_t& timeout,
                                     ResponseHandler<WriteSingleRegisterResponse> handler)
{
    meta_send_request<WriteSingleRegisterRequestImpl, WriteSingleRegisterResponseImpl>(request, timeout, handler);
}

void ClientSessionImpl::send_request(const WriteMultipleCoilsRequest& request,
                                     ResponseHandler<WriteMultipleCoilsResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void ClientSessionImpl::send_request(const WriteMultipleCoilsRequest& request,
                                     const duration_t& timeout,
                                     ResponseHandler<WriteMultipleCoilsResponse> handler)
{
    meta_send_request<WriteMultipleCoilsRequestImpl, WriteMultipleCoilsResponseImpl>(request, timeout, handler);
}

void ClientSessionImpl::send_request(const WriteMultipleRegistersRequest& request,
                                     ResponseHandler<WriteMultipleRegistersResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void ClientSessionImpl::send_request(const WriteMultipleRegistersRequest& request,
                                     const duration_t& timeout,
                                     ResponseHandler<WriteMultipleRegistersResponse> handler)
{
    meta_send_request<WriteMultipleRegistersRequestImpl, WriteMultipleRegistersResponseImpl>(request, timeout, handler);
}

std::shared_ptr<IScheduledRequest> ClientSessionImpl::schedule_request(const ReadCoilsRequest& request,
                                                                       const duration_t& frequency,
                                                                       ResponseHandler<ReadCoilsResponse> handler)
{
    return schedule_request(request, m_default_timeout, frequency, handler);
}

std::shared_ptr<IScheduledRequest> ClientSessionImpl::schedule_request(const ReadCoilsRequest& request,
                                                                       const duration_t& timeout,
                                                                       const duration_t& frequency,
                                                                       ResponseHandler<ReadCoilsResponse> handler)
{
    return meta_schedule_request(request, timeout, frequency, handler);
}

std::shared_ptr<IScheduledRequest> ClientSessionImpl::schedule_request(const ReadDiscreteInputsRequest& request,
                                                                       const duration_t& frequency,
                                                                       ResponseHandler<ReadDiscreteInputsResponse> handler)
{
    return schedule_request(request, m_default_timeout, frequency, handler);
}

std::shared_ptr<IScheduledRequest> ClientSessionImpl::schedule_request(const ReadDiscreteInputsRequest& request,
                                                                       const duration_t& timeout,
                                                                       const duration_t& frequency,
                                                                       ResponseHandler<ReadDiscreteInputsResponse> handler)
{
    return meta_schedule_request(request, timeout, frequency, handler);
}

std::shared_ptr<IScheduledRequest> ClientSessionImpl::schedule_request(const ReadHoldingRegistersRequest& request,
                                                                       const duration_t& frequency,
                                                                       ResponseHandler<ReadHoldingRegistersResponse> handler)
{
    return schedule_request(request, m_default_timeout, frequency, handler);
}

std::shared_ptr<IScheduledRequest> ClientSessionImpl::schedule_request(const ReadHoldingRegistersRequest& request,
                                                                       const duration_t& timeout,
                                                                       const duration_t& frequency,
                                                                       ResponseHandler<ReadHoldingRegistersResponse> handler)
{
    return meta_schedule_request(request, timeout, frequency, handler);
}

std::shared_ptr<IScheduledRequest> ClientSessionImpl::schedule_request(const ReadInputRegistersRequest& request,
                                                                       const duration_t& frequency,
                                                                       ResponseHandler<ReadInputRegistersResponse> handler)
{
    return schedule_request(request, m_default_timeout, frequency, handler);
}

std::shared_ptr<IScheduledRequest> ClientSessionImpl::schedule_request(const ReadInputRegistersRequest& request,
                                                                       const duration_t& timeout,
                                                                       const duration_t& frequency,
                                                                       ResponseHandler<ReadInputRegistersResponse> handler)
{
    return meta_schedule_request(request, timeout, frequency, handler);
}

std::unique_ptr<ITimer> ClientSessionImpl::start(const duration_t& duration, const action_t& action)
{
    return std::make_unique<TimerWrapper>(this->m_executor->start(duration, action));
}

std::unique_ptr<ITimer> ClientSessionImpl::start(const steady_time_t& expiration, const action_t& action)
{
    return std::make_unique<TimerWrapper>(this->m_executor->start(expiration, action));
}

template<typename TRequestImpl, typename TResponseImpl, typename TRequest, typename TResponse>
void ClientSessionImpl::meta_send_request(const TRequest& request,
                                          const duration_t& timeout,
                                          ResponseHandler<TResponse> handler)
{
    TRequestImpl request_impl{request};
    m_channel->send_request(m_unit_identifier, request_impl, timeout, [=, self = shared_from_this()](const Expected<ser4cpp::rseq_t>& response)
    {
        if(!response.is_valid())
        {
            handler(Expected<TResponse>::from_exception(response.get_exception()));
        }
        else
        {
            handler(TResponseImpl::parse(request_impl, response.get()));
        }
    });
}

template<typename TRequest, typename TResponse>
std::shared_ptr<IScheduledRequest> ClientSessionImpl::meta_schedule_request(const TRequest& request,
                                                                            const duration_t& timeout,
                                                                            const duration_t& frequency,
                                                                            ResponseHandler<TResponse> handler)
{
    auto scheduled_request = std::make_shared<ScheduledRequest<TRequest, TResponse>>(shared_from_this(),
                                                                                     handler,
                                                                                     m_executor,
                                                                                     request,
                                                                                     timeout,
                                                                                     frequency);
    scheduled_request->start();

    m_executor->post([=, self = shared_from_this()]() {
        m_scheduled_requests.push_back(scheduled_request);
    });

    return scheduled_request;
};

} // namespace modbus

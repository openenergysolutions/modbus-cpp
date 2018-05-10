#include "session/SessionImpl.h"

#include "modbus/session/ISessionResponseHandler.h"
#include "TimerWrapper.h"
#include "messages/ReadHoldingRegistersRequestImpl.h"
#include "messages/ReadHoldingRegistersResponseImpl.h"
#include "messages/ReadInputRegistersRequestImpl.h"
#include "messages/ReadInputRegistersResponseImpl.h"
#include "messages/WriteMultipleRegistersRequestImpl.h"
#include "messages/WriteMultipleRegistersResponseImpl.h"
#include "messages/WriteSingleRegisterRequestImpl.h"
#include "messages/WriteSingleRegisterResponseImpl.h"
#include "channel/IChannelImpl.h"
#include "session/ScheduledRequest.h"

namespace modbus
{

SessionImpl::SessionImpl(std::shared_ptr<exe4cpp::IExecutor> executor,
                         std::shared_ptr<Logger> logger,
                         std::shared_ptr<IChannelImpl> channel,
                         const UnitIdentifier& unit_identifier,
                         const duration_t& default_timeout,
                         std::shared_ptr<ISessionResponseHandler> session_response_handler)
: m_executor{executor},
  m_logger{logger},
  m_channel{channel},
  m_unit_identifier{unit_identifier},
  m_default_timeout{default_timeout},
  m_session_response_handler{session_response_handler}
{

}

void SessionImpl::shutdown()
{
    m_executor->post([=, self = shared_from_this()] {
        for(auto& req : m_scheduled_requests)
        {
            req->stop();
        }

        m_scheduled_requests.clear();
    });
}

void SessionImpl::send_request(const ReadHoldingRegistersRequest& request,
                               ResponseHandler<ReadHoldingRegistersResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void SessionImpl::send_request(const ReadHoldingRegistersRequest& request,
                               const duration_t& timeout,
                               ResponseHandler<ReadHoldingRegistersResponse> handler)
{
    meta_send_request<ReadHoldingRegistersRequestImpl, ReadHoldingRegistersResponseImpl>(request, timeout, handler);
}

void SessionImpl::send_request(const ReadInputRegistersRequest& request,
                               ResponseHandler<ReadInputRegistersResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void SessionImpl::send_request(const ReadInputRegistersRequest& request,
                               const duration_t& timeout,
                               ResponseHandler<ReadInputRegistersResponse> handler)
{
    meta_send_request<ReadInputRegistersRequestImpl, ReadInputRegistersResponseImpl>(request, timeout, handler);
}

void SessionImpl::send_request(const WriteSingleRegisterRequest& request,
                               ResponseHandler<WriteSingleRegisterResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void SessionImpl::send_request(const WriteSingleRegisterRequest& request,
                               const duration_t& timeout,
                               ResponseHandler<WriteSingleRegisterResponse> handler)
{
    meta_send_request<WriteSingleRegisterRequestImpl, WriteSingleRegisterResponseImpl>(request, timeout, handler);
}

void SessionImpl::send_request(const WriteMultipleRegistersRequest& request,
                               ResponseHandler<WriteMultipleRegistersResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void SessionImpl::send_request(const WriteMultipleRegistersRequest& request,
                               const duration_t& timeout,
                               ResponseHandler<WriteMultipleRegistersResponse> handler)
{
    meta_send_request<WriteMultipleRegistersRequestImpl, WriteMultipleRegistersResponseImpl>(request, timeout, handler);
}

std::shared_ptr<IScheduledRequest> SessionImpl::schedule_request(const ReadHoldingRegistersRequest& request,
                                                                 const duration_t& frequency)
{
    return schedule_request(request, m_default_timeout, frequency);
}

std::shared_ptr<IScheduledRequest> SessionImpl::schedule_request(const ReadHoldingRegistersRequest& request,
                                                                 const duration_t& timeout,
                                                                 const duration_t& frequency)
{
    return meta_schedule_request<ReadHoldingRegistersRequest, ReadHoldingRegistersResponse>(request,
                                                                                            timeout,
                                                                                            frequency);
}

std::shared_ptr<IScheduledRequest> SessionImpl::schedule_request(const ReadInputRegistersRequest& request,
                                                                 const duration_t& frequency)
{
    return schedule_request(request, m_default_timeout, frequency);
}

std::shared_ptr<IScheduledRequest> SessionImpl::schedule_request(const ReadInputRegistersRequest& request,
                                                                 const duration_t& timeout,
                                                                 const duration_t& frequency)
{
    return meta_schedule_request<ReadInputRegistersRequest, ReadInputRegistersResponse>(request,
                                                                                        timeout,
                                                                                        frequency);
}

std::unique_ptr<ITimer> SessionImpl::start(const duration_t& duration, const action_t& action)
{
    return std::make_unique<TimerWrapper>(this->m_executor->start(duration, action));
}

std::unique_ptr<ITimer> SessionImpl::start(const steady_time_t& expiration, const action_t& action)
{
    return std::make_unique<TimerWrapper>(this->m_executor->start(expiration, action));
}

template<typename TRequestImpl, typename TResponseImpl, typename TRequest, typename TResponse>
void SessionImpl::meta_send_request(const TRequest& request,
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
            handler(TResponseImpl::parse(request, response.get()));
        }
    });
}

template<typename TRequest, typename TResponse>
std::shared_ptr<IScheduledRequest> SessionImpl::meta_schedule_request(const TRequest& request,
                                                                      const duration_t& timeout,
                                                                      const duration_t& frequency)
{
    auto scheduled_request = std::make_shared<ScheduledRequest<TRequest, TResponse>>(shared_from_this(),
                                                                                     m_session_response_handler,
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

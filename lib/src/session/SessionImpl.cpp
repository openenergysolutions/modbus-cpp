#include "session/SessionImpl.h"

#include "modbus/channel/IChannel.h"
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/messages/ReadInputRegistersRequest.h"
#include "modbus/messages/ReadInputRegistersResponse.h"
#include "modbus/messages/WriteMultipleRegistersRequest.h"
#include "modbus/messages/WriteMultipleRegistersResponse.h"
#include "modbus/messages/WriteSingleRegisterRequest.h"
#include "modbus/messages/WriteSingleRegisterResponse.h"
#include "modbus/session/ISessionResponseHandler.h"
#include "session/ScheduledRequest.h"

namespace modbus
{

SessionImpl::SessionImpl(std::shared_ptr<openpal::IExecutor> executor,
                         std::shared_ptr<Logger> logger,
                         std::shared_ptr<IChannel> channel,
                         const UnitIdentifier& unit_identifier,
                         const openpal::duration_t& default_timeout,
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
                               const openpal::duration_t& timeout,
                               ResponseHandler<ReadHoldingRegistersResponse> handler)
{
    meta_send_request(request, timeout, handler);
}

void SessionImpl::send_request(const ReadInputRegistersRequest& request,
                               ResponseHandler<ReadInputRegistersResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void SessionImpl::send_request(const ReadInputRegistersRequest& request,
                               const openpal::duration_t& timeout,
                               ResponseHandler<ReadInputRegistersResponse> handler)
{
    meta_send_request(request, timeout, handler);
}

void SessionImpl::send_request(const WriteSingleRegisterRequest& request,
                               ResponseHandler<WriteSingleRegisterResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void SessionImpl::send_request(const WriteSingleRegisterRequest& request,
                               const openpal::duration_t& timeout,
                               ResponseHandler<WriteSingleRegisterResponse> handler)
{
    meta_send_request(request, timeout, handler);
}

void SessionImpl::send_request(const WriteMultipleRegistersRequest& request,
                               ResponseHandler<WriteMultipleRegistersResponse> handler)
{
    send_request(request, m_default_timeout, handler);
}

void SessionImpl::send_request(const WriteMultipleRegistersRequest& request,
                               const openpal::duration_t& timeout,
                               ResponseHandler<WriteMultipleRegistersResponse> handler)
{
    meta_send_request(request, timeout, handler);
}

std::shared_ptr<IScheduledRequest> SessionImpl::schedule_request(const ReadHoldingRegistersRequest& request,
                                                                 const openpal::duration_t& frequency)
{
    return schedule_request(request, m_default_timeout, frequency);
}

std::shared_ptr<IScheduledRequest> SessionImpl::schedule_request(const ReadHoldingRegistersRequest& request,
                                                                 const openpal::duration_t& timeout,
                                                                 const openpal::duration_t& frequency)
{
    return meta_schedule_request<ReadHoldingRegistersRequest, ReadHoldingRegistersResponse>(request,
                                                                                            timeout,
                                                                                            frequency);
}

std::shared_ptr<IScheduledRequest> SessionImpl::schedule_request(const ReadInputRegistersRequest& request,
                                                                 const openpal::duration_t& frequency)
{
    return schedule_request(request, m_default_timeout, frequency);
}

std::shared_ptr<IScheduledRequest> SessionImpl::schedule_request(const ReadInputRegistersRequest& request,
                                                                 const openpal::duration_t& timeout,
                                                                 const openpal::duration_t& frequency)
{
    return meta_schedule_request<ReadInputRegistersRequest, ReadInputRegistersResponse>(request,
                                                                                        timeout,
                                                                                        frequency);
}

template<typename TRequest, typename TResponse>
void SessionImpl::meta_send_request(const TRequest& request,
                                    const openpal::duration_t& timeout,
                                    ResponseHandler<TResponse> handler)
{
    m_channel->send_request(m_unit_identifier, request, timeout, [=, self = shared_from_this()](const Expected<openpal::rseq_t>& response)
    {
        if(!response.is_valid())
        {
            handler(Expected<TResponse>::from_exception(response.get_exception()));
        }
        else
        {
            handler(TResponse::parse(request, response.get()));
        }
    });
}

template<typename TRequest, typename TResponse>
std::shared_ptr<IScheduledRequest> SessionImpl::meta_schedule_request(const TRequest& request,
                                                                      const openpal::duration_t& timeout,
                                                                      const openpal::duration_t& frequency)
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

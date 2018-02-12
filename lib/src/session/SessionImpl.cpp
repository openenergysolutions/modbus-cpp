#include "session/SessionImpl.h"

#include "openpal/executor/IExecutor.h"

#include "modbus/ISchedule.h"
#include "modbus/ISessionResponseHandler.h"
#include "modbus/channel/IChannel.h"
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/messages/ReadInputRegistersRequest.h"
#include "modbus/messages/ReadInputRegistersResponse.h"
#include "modbus/messages/WriteMultipleRegistersRequest.h"
#include "modbus/messages/WriteMultipleRegistersResponse.h"
#include "modbus/messages/WriteSingleRegisterRequest.h"
#include "modbus/messages/WriteSingleRegisterResponse.h"
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

SessionImpl::~SessionImpl()
{
    shutdown();
}

void SessionImpl::shutdown()
{
    for(auto& req : m_scheduled_requests)
    {
        req->cancel();
    }
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

void SessionImpl::schedule_request(const ReadHoldingRegistersRequest& request,
                                   std::unique_ptr<ISchedule> schedule)
{
    schedule_request(request, m_default_timeout, std::move(schedule));
}

void SessionImpl::schedule_request(const ReadHoldingRegistersRequest& request,
                                   const openpal::duration_t& timeout,
                                   std::unique_ptr<ISchedule> schedule)
{
    meta_schedule_request<ReadHoldingRegistersRequest, ReadHoldingRegistersResponse>(request, timeout, std::move(schedule));
}

void SessionImpl::schedule_request(const ReadInputRegistersRequest& request,
                                   std::unique_ptr<ISchedule> schedule)
{

}

void SessionImpl::schedule_request(const ReadInputRegistersRequest& request,
                                   const openpal::duration_t& timeout,
                                   std::unique_ptr<ISchedule> schedule)
{

}

template<typename TRequest, typename TResponse>
void SessionImpl::meta_send_request(const TRequest& request,
                                    const openpal::duration_t& timeout,
                                    ResponseHandler<TResponse> handler)
{
    m_executor->post([=, self = shared_from_this()] {
        m_channel->send_request(m_unit_identifier, request, timeout, [=, self2 = self](const Expected<openpal::rseq_t>& response)
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
    });
}

template<typename TRequest, typename TResponse>
void SessionImpl::meta_schedule_request(const TRequest& request,
                                        const openpal::duration_t& timeout,
                                        std::unique_ptr<ISchedule> schedule)
{
    auto shared_schedule = std::shared_ptr<ISchedule>{std::move(schedule)};
    m_executor->post([=, self = shared_from_this()] {
        auto scheduled_request = std::make_shared<ScheduledRequest<TRequest, TResponse>>(this, m_session_response_handler.get(), m_executor, request, timeout, shared_schedule->clone());
        m_scheduled_requests.push_back(scheduled_request);
        scheduled_request->start();
    });
};

} // namespace modbus

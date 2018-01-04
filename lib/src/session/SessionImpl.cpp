#include "session/SessionImpl.h"

#include "modbus/channel/IChannel.h"
#include "modbus/ISchedule.h"
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/messages/ReadInputRegistersRequest.h"
#include "modbus/messages/ReadInputRegistersResponse.h"

namespace modbus
{

SessionImpl::SessionImpl(std::shared_ptr<Logger> logger,
                         std::shared_ptr<IChannel> channel,
                         const UnitIdentifier& unit_identifier,
                         const openpal::duration_t& default_timeout,
                         std::shared_ptr<ISessionResponseHandler> session_response_handler)
: m_logger{logger},
  m_channel{channel},
  m_unit_identifier{unit_identifier},
  m_default_timeout{default_timeout},
  m_session_response_handler{session_response_handler}
{

}

void SessionImpl::send_request(const ReadHoldingRegistersRequest& request,
                               ResponseHandler<ReadHoldingRegistersResponse> handler)
{
    m_channel->send_request(m_unit_identifier, request, m_default_timeout, [=](const Expected<openpal::rseq_t>&)
    {
        
    });
}

void SessionImpl::send_request(const ReadHoldingRegistersRequest& request,
                               const openpal::duration_t& timeout,
                               ResponseHandler<ReadHoldingRegistersResponse> handler)
{

}

void SessionImpl::send_request(const ReadInputRegistersRequest& request,
                               ResponseHandler<ReadInputRegistersResponse> handler)
{

}

void SessionImpl::send_request(const ReadInputRegistersRequest& request,
                               const openpal::duration_t& timeout,
                               ResponseHandler<ReadInputRegistersResponse> handler)
{

}

void SessionImpl::schedule_request(const ReadHoldingRegistersRequest& request,
                                   std::unique_ptr<ISchedule> schedule)
{

}

void SessionImpl::schedule_request(const ReadHoldingRegistersRequest& request,
                                   const openpal::duration_t& timeout,
                                   std::unique_ptr<ISchedule> schedule)
{

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

} // namespace modbus
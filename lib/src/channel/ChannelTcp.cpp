#include "channel/ChannelTcp.h"

#include "openpal/container/Buffer.h"

#include "modbus/messages/IRequest.h"
#include "channel/ITcpConnection.h"
#include "session/SessionImpl.h"

namespace modbus
{

ChannelTcp::ChannelTcp(std::shared_ptr<openpal::IExecutor> executor,
                       std::shared_ptr<Logger> logger,
                       std::shared_ptr<ITcpConnection> tcp_connection)
: m_executor{std::move(executor)},
  m_logger{std::move(logger)},
  m_tcp_connection{tcp_connection}
{
    m_tcp_connection->set_listener(this);
}

std::shared_ptr<ISession> ChannelTcp::create_session(const UnitIdentifier& unit_identifier,
                                                     const openpal::duration_t& default_timeout,
                                                     std::shared_ptr<ISessionResponseHandler> session_response_handler)
{
    return std::make_shared<SessionImpl>(m_executor,
                                         m_logger->clone("session"),
                                         shared_from_this(),
                                         unit_identifier,
                                         default_timeout,
                                         session_response_handler);
}

void ChannelTcp::send_request(const UnitIdentifier& unit_identifier,
                              const IRequest& request,
                              const openpal::duration_t& timeout,
                              ResponseHandler<openpal::rseq_t> response_handler)
{


    auto pending_request = std::make_unique<PendingRequest>(unit_identifier, (uint32_t)request.get_request_length(), timeout, response_handler);

    auto wdata = pending_request->request.as_wslice();
    request.build_request(wdata);

    m_pending_requests.push_back(std::move(pending_request));

    check_pending_requests();
}

void ChannelTcp::on_receive(const openpal::rseq_t& data)
{
    if(m_current_request)
    {
        m_current_request->response_handler(Expected<openpal::rseq_t>{data});
        m_current_request.release();
    }
}

void ChannelTcp::on_error()
{
    if(m_current_request)
    {
        m_current_request->response_handler(Expected<openpal::rseq_t>::from_exception(std::domain_error("connection failure")));
        m_current_request.release();
    }
}

void ChannelTcp::check_pending_requests()
{
    if(!m_current_request && !m_pending_requests.empty())
    {
        m_current_request = std::move(m_pending_requests.front());
        m_pending_requests.pop_front();

        m_tcp_connection->send(m_current_request->request.as_rslice());
    }
}

} // namespace modbus

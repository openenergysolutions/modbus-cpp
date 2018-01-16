#include "channel/ChannelTcp.h"

#include "openpal/container/StaticBuffer.h"
#include "modbus/exceptions/ConnectionException.h"
#include "modbus/exceptions/TimeoutException.h"
#include "channel/ITcpConnection.h"
#include "channel/PendingRequest.h"
#include "session/SessionImpl.h"

namespace modbus
{

ChannelTcp::ChannelTcp(std::shared_ptr<openpal::IExecutor> executor,
                       std::shared_ptr<Logger> logger,
                       std::shared_ptr<ITcpConnection> tcp_connection)
    : m_executor{std::move(executor)},
      m_logger{std::move(logger)},
      m_tcp_connection{tcp_connection},
      m_parser{this},
      m_current_timer{nullptr},
      m_next_transaction_id{0x0000}
{
    
}

ChannelTcp::~ChannelTcp()
{
    m_tcp_connection->close();
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
    auto pending_request = std::make_unique<PendingRequest>(unit_identifier,
                                                            m_next_transaction_id,
                                                            request,
                                                            timeout,
                                                            response_handler);
    m_pending_requests.push_back(std::move(pending_request));

    m_next_transaction_id = TransactionIdentifier{(uint16_t)(m_next_transaction_id.get_value() + 1)};

    check_pending_requests();
}

void ChannelTcp::shutdown()
{
    m_executor->post([=, self = shared_from_this()] {
        m_pending_requests.clear();
        m_current_request.reset(nullptr);
        m_current_timer.cancel();

        m_tcp_connection->close();
    });
}

void ChannelTcp::on_receive(const openpal::rseq_t& data)
{
    m_parser.parse(data);
}

void ChannelTcp::on_error()
{
    m_parser.reset();

    cancel_all_pending_requests();
    cancel_current_request();
}

void ChannelTcp::on_mbap_message(const MbapMessage& message)
{
    if(m_current_request)
    {
        if(message.transaction_id == m_current_request->transaction_id &&
           message.unit_id == m_current_request->unit_id)
        {
            m_current_timer.cancel();
            m_current_request->response_handler(Expected<openpal::rseq_t>{message.data});
            m_current_request.reset(nullptr);

            check_pending_requests();
        }
    }
}

void ChannelTcp::check_pending_requests()
{
    if(!m_current_request && !m_pending_requests.empty())
    {
        m_current_request = std::move(m_pending_requests.front());
        m_pending_requests.pop_front();

        openpal::StaticBuffer<uint32_t, 260> buffer;
        auto view = buffer.as_wseq();
        auto serialized_request = MbapMessage::build_message(m_current_request->unit_id,
                                                             m_current_request->transaction_id,
                                                             *m_current_request->request,
                                                             view);

        m_tcp_connection->set_listener(std::dynamic_pointer_cast<ChannelTcp>(shared_from_this()));
        m_tcp_connection->send(serialized_request);

        m_current_timer = m_executor->start(m_current_request->timeout, [=, self = shared_from_this()]() {
            cancel_current_request();
        });
    }
}

void ChannelTcp::cancel_current_request()
{
    if(m_current_request)
    {
        m_current_request->response_handler(Expected<openpal::rseq_t>::from_exception(TimeoutException{}));
        m_current_request.reset(nullptr);
        m_current_timer.cancel();

        check_pending_requests();
    }
}

void ChannelTcp::cancel_all_pending_requests()
{
    for(auto& req : m_pending_requests)
    {
        req->response_handler(Expected<openpal::rseq_t>::from_exception(ConnectionException{}));
    }

    m_pending_requests.clear();
}

} // namespace modbus

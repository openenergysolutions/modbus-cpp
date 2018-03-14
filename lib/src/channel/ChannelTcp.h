#ifndef MODBUS_CHANNELTCP_H
#define MODBUS_CHANNELTCP_H

#include "modbus/channel/IChannel.h"

#include <deque>
#include <memory>
#include "loopser/container/Buffer.h"
#include "loopser/executor/IExecutor.h"
#include "modbus/messages/IRequest.h"
#include "channel/IConnectionListener.h"
#include "channel/IMbapSink.h"
#include "channel/MbapParser.h"
#include "channel/PendingRequest.h"
#include "logging/Logger.h"

namespace modbus
{

class ITcpConnection;

class ChannelTcp : public IChannel, public IConnectionListener, public IMbapSink
{
public:
    ChannelTcp(std::shared_ptr<loopser::IExecutor> executor,
               std::shared_ptr<Logger> logger,
               std::shared_ptr<ITcpConnection> tcp_connection);
    ~ChannelTcp() = default;

    std::shared_ptr<ISession> create_session(const UnitIdentifier& unit_identifier,
                                             const loopser::duration_t& default_timeout,
                                             std::shared_ptr<ISessionResponseHandler> session_response_handler) override;

    void send_request(const UnitIdentifier& unit_identifier,
                      const IRequest& request,
                      const loopser::duration_t& timeout,
                      ResponseHandler<loopser::rseq_t> response_handler) override;

    void shutdown() override;

    // Connection listener
    void on_receive(const loopser::rseq_t& data) override;
    void on_error() override;

    // MBAP sink
    void on_mbap_message(const MbapMessage& message) override;

private:
    void check_pending_requests();
    void cancel_current_request();
    void cancel_all_pending_requests();

    std::shared_ptr<loopser::IExecutor> m_executor;
    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<ITcpConnection> m_tcp_connection;

    bool m_is_shutdown;
    std::vector<std::shared_ptr<ISession>> m_sessions;
    MbapParser m_parser;
    std::deque<std::unique_ptr<PendingRequest>> m_pending_requests;
    std::unique_ptr<PendingRequest> m_current_request;
    loopser::Timer m_current_timer;
    TransactionIdentifier m_next_transaction_id;
};

} // namespace modbus

#endif //MODBUS_CHANNELTCP_H

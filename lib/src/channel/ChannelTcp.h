#ifndef MODBUS_CHANNELTCP_H
#define MODBUS_CHANNELTCP_H

#include <deque>
#include <memory>
#include "ser4cpp/container/Buffer.h"
#include "exe4cpp/IExecutor.h"
#include "channel/IChannelImpl.h"
#include "channel/IConnectionListener.h"
#include "channel/IMbapSink.h"
#include "channel/MbapParser.h"
#include "channel/PendingRequest.h"
#include "logging/Logger.h"
#include "messages/IRequest.h"

namespace modbus
{

class ITcpConnection;

class ChannelTcp : public IChannelImpl, public IConnectionListener, public IMbapSink
{
public:
    ChannelTcp(std::shared_ptr<exe4cpp::IExecutor> executor,
               std::shared_ptr<Logger> logger,
               std::shared_ptr<ITcpConnection> tcp_connection);
    ~ChannelTcp() = default;

    std::shared_ptr<ISession> create_session(const UnitIdentifier& unit_identifier,
                                             const exe4cpp::duration_t& default_timeout) override;

    void send_request(const UnitIdentifier& unit_identifier,
                      const IRequest& request,
                      const exe4cpp::duration_t& timeout,
                      ResponseHandler<ser4cpp::rseq_t> response_handler) override;

    void shutdown() override;

    // Connection listener
    void on_receive(const ser4cpp::rseq_t& data) override;
    void on_error(const std::string& message) override;

    // MBAP sink
    void on_mbap_message(const MbapMessage& message) override;

private:
    void check_pending_requests();
    void cancel_current_request();
    void cancel_all_pending_requests(const std::string& message);

    std::shared_ptr<exe4cpp::IExecutor> m_executor;
    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<ITcpConnection> m_tcp_connection;

    bool m_is_shutdown;
    std::vector<std::shared_ptr<ISession>> m_sessions;
    MbapParser m_parser;
    std::deque<std::unique_ptr<PendingRequest>> m_pending_requests;
    std::unique_ptr<PendingRequest> m_current_request;
    exe4cpp::Timer m_current_timer;
    TransactionIdentifier m_next_transaction_id;
};

} // namespace modbus

#endif //MODBUS_CHANNELTCP_H

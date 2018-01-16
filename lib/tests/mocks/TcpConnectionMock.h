#ifndef MODBUS_TCPCONNECTIONMOCK_H
#define MODBUS_TCPCONNECTIONMOCK_H

#include <memory>
#include <vector>
#include "openpal/container/Buffer.h"
#include "channel/IMbapSink.h"
#include "channel/ITcpConnection.h"
#include "channel/MbapParser.h"

class TcpConnectionMock : public modbus::ITcpConnection, public modbus::IMbapSink
{
public:
    TcpConnectionMock();

    void set_listener(std::weak_ptr<modbus::IConnectionListener> listener) override;
    void send(const openpal::rseq_t& data) override;
    void close() override;

    void on_mbap_message(const modbus::MbapMessage& message) override;

    std::size_t get_num_requests() const;
    const std::vector<modbus::MbapMessage>& get_requests() const;

private:
    modbus::MbapParser m_parser;
    std::vector<modbus::MbapMessage> m_requests;
    std::vector<std::unique_ptr<openpal::Buffer>> m_request_buffers;
};

#endif //MODBUS_TCPCONNECTIONMOCK_H

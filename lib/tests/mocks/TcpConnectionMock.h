#ifndef MODBUS_TCPCONNECTIONMOCK_H
#define MODBUS_TCPCONNECTIONMOCK_H

#include <memory>
#include <vector>
#include "openpal/container/Buffer.h"
#include "channel/ITcpConnection.h"

class TcpConnectionMock : public modbus::ITcpConnection
{
public:
    TcpConnectionMock();

    void set_listener(std::weak_ptr<modbus::IConnectionListener> listener) override;
    void send(const openpal::rseq_t& data) override;
    void close() override;

    std::size_t get_num_requests() const;
    const std::vector<std::unique_ptr<openpal::Buffer>>& get_requests() const;

private:
    std::vector<std::unique_ptr<openpal::Buffer>> m_requests;
};

#endif //MODBUS_TCPCONNECTIONMOCK_H

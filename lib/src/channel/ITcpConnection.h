#ifndef MODBUS_TCPCONNECTION_H
#define MODBUS_TCPCONNECTION_H

#include <memory>

#include "openpal/container/SequenceTypes.h"

#include "IConnectionListener.h"

namespace modbus
{

class Ipv4Endpoint;

class ITcpConnection : public std::enable_shared_from_this<ITcpConnection>
{
public:
    virtual ~ITcpConnection() = default;

    virtual void set_listener(IConnectionListener* listener) = 0;
    virtual void send(const openpal::rseq_t& data) = 0;
    virtual void close() = 0;
};

} // namespace modbus

#endif //MODBUS_TCPCONNECTION_H

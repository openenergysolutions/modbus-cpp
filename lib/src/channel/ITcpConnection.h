#ifndef MODBUS_ITCPCONNECTION_H
#define MODBUS_ITCPCONNECTION_H

#include <memory>

#include "ser4cpp/container/SequenceTypes.h"

namespace modbus
{

class IConnectionListener;
class Ipv4Endpoint;

class ITcpConnection : public std::enable_shared_from_this<ITcpConnection>
{
public:
    virtual ~ITcpConnection() = default;

    virtual void set_listener(std::weak_ptr<IConnectionListener> listener) = 0;
    virtual void send(const ser4cpp::rseq_t& data) = 0;
    virtual void close() = 0;
};

} // namespace modbus

#endif //MODBUS_ITCPCONNECTION_H

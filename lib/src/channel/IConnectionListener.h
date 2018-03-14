#ifndef MODBUS_ICONNECTIONLISTENER_H
#define MODBUS_ICONNECTIONLISTENER_H

#include <loopser/container/SequenceTypes.h>

namespace modbus
{

class IConnectionListener
{
public:
    virtual ~IConnectionListener() = default;

    virtual void on_write_done() {};
    virtual void on_receive(const loopser::rseq_t& data) {};
    virtual void on_error() {};
};

} // namespace modbus

#endif //MODBUS_ICONNECTIONLISTENER_H

#ifndef MODBUS_ICONNECTIONLISTENER_H
#define MODBUS_ICONNECTIONLISTENER_H

#include <openpal/container/SequenceTypes.h>

namespace modbus
{

class IConnectionListener
{
public:
    virtual ~IConnectionListener() = default;

    virtual void on_receive(const openpal::rseq_t& data) = 0;
    virtual void on_error() = 0;
};

} // namespace modbus

#endif //MODBUS_ICONNECTIONLISTENER_H

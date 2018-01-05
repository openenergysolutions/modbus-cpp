#ifndef MODBUS_CONNECTIONCALLBACK_H
#define MODBUS_CONNECTIONCALLBACK_H

#include <openpal/container/SequenceTypes.h>

namespace modbus
{

class ConnectionListener
{
public:
    virtual ~ConnectionListener() = default;

    virtual void on_receive(const openpal::rseq_t& data) = 0;
    virtual void on_error() = 0;
};

} // namespace modbus

#endif //MODBUS_CONNECTIONCALLBACK_H

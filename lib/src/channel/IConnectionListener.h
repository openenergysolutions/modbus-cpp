#ifndef MODBUS_ICONNECTIONLISTENER_H
#define MODBUS_ICONNECTIONLISTENER_H

#include <string>
#include <ser4cpp/container/SequenceTypes.h>

namespace modbus
{

class IConnectionListener
{
public:
    virtual ~IConnectionListener() = default;

    virtual void on_write_done() {};
    virtual void on_receive(const ser4cpp::rseq_t& data) {};
    virtual void on_error(const std::string& message) {};
};

} // namespace modbus

#endif //MODBUS_ICONNECTIONLISTENER_H

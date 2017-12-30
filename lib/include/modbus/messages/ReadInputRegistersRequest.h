#ifndef MODBUS_READINPUTREGISTERSREQUEST_H
#define MODBUS_READINPUTREGISTERSREQUEST_H

#include "modbus/messages/IRequest.h"
#include "modbus/messages/Address.h"

namespace modbus
{

class ReadInputRegistersRequest : public IRequest
{
public:
    ReadInputRegistersRequest(Address startingAddress, uint16_t qtyOfRegisters);

    void BuildRequest(openpal::Buffer &buffer) override;

private:
    Address m_startingAddress;
    uint16_t qtyOfRegisters;
};

} // namespace modbus

#endif //MODBUS_READINPUTREGISTERSREQUEST_H

#ifndef MODBUS_READHOLDINGREGISTERSREQUEST_H
#define MODBUS_READHOLDINGREGISTERSREQUEST_H

#include "modbus/messages/IRequest.h"
#include "modbus/messages/Address.h"

namespace modbus
{
namespace messages
{

class ReadHoldingRegistersRequest : public IRequest
{
public:
    ReadHoldingRegistersRequest(Address startingAddress, uint16_t qtyOfRegisters);

    void BuildRequest(openpal::Buffer &buffer) override;

private:
    Address m_startingAddress;
    uint16_t m_qtyOfRegisters;
};

} // namespace messages
} // namespace modbus

#endif //MODBUS_READHOLDINGREGISTERSREQUEST_H

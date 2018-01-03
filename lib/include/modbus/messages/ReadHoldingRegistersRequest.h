#ifndef MODBUS_READHOLDINGREGISTERSREQUEST_H
#define MODBUS_READHOLDINGREGISTERSREQUEST_H

#include "modbus/messages/IRequest.h"
#include "modbus/messages/Address.h"

namespace modbus
{

class ReadHoldingRegistersRequest : public IRequest
{
public:
    ReadHoldingRegistersRequest(Address starting_address, uint16_t qty_of_registers);

    void build_request(openpal::wseq_t& buffer) override;

private:
    Address m_starting_address;
    uint16_t m_qty_of_registers;
};

} // namespace modbus

#endif //MODBUS_READHOLDINGREGISTERSREQUEST_H

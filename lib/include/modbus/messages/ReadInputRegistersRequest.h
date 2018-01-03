#ifndef MODBUS_READINPUTREGISTERSREQUEST_H
#define MODBUS_READINPUTREGISTERSREQUEST_H

#include "modbus/messages/IRequest.h"
#include "modbus/messages/Address.h"

namespace modbus
{

class ReadInputRegistersRequest : public IRequest
{
public:
    ReadInputRegistersRequest(Address starting_address, uint16_t qty_of_registers);

    void build_request(openpal::wseq_t& buffer) override;

private:
    Address m_starting_address;
    uint16_t qty_of_registers;
};

} // namespace modbus

#endif //MODBUS_READINPUTREGISTERSREQUEST_H

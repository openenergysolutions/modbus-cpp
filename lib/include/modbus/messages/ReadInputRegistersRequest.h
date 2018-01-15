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

    std::unique_ptr<IRequest> clone() const override;

    size_t get_request_length() const override;
    void build_request(openpal::wseq_t& buffer) const override;

private:
    Address m_starting_address;
    uint16_t qty_of_registers;
};

} // namespace modbus

#endif //MODBUS_READINPUTREGISTERSREQUEST_H

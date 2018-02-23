#ifndef MODBUS_READREGISTERSREQUEST_H
#define MODBUS_READREGISTERSREQUEST_H

#include "modbus/messages/IRequest.h"
#include "modbus/messages/Address.h"

namespace modbus
{

template <uint8_t function_code>
class ReadRegistersRequest : public IRequest
{
public:
    ReadRegistersRequest(Address starting_address, uint16_t qty_of_registers);

    std::unique_ptr<IRequest> clone() const override;

    size_t get_request_length() const override;
    void build_request(openpal::wseq_t& buffer) const override;

    const Address& get_starting_address() const;
    uint16_t get_qty_of_registers() const;

private:
    Address m_starting_address;
    uint16_t m_qty_of_registers;
};

extern template class ReadRegistersRequest<0x03>;
extern template class ReadRegistersRequest<0x04>;

} // namespace modbus

#endif //MODBUS_READREGISTERSREQUEST_H

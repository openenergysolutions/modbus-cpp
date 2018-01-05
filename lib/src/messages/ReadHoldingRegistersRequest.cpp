#include "modbus/messages/ReadHoldingRegistersRequest.h"

namespace modbus
{

ReadHoldingRegistersRequest::ReadHoldingRegistersRequest(Address starting_address, uint16_t qty_of_registers)
        : m_starting_address{starting_address},
          m_qty_of_registers{qty_of_registers}
{

}

size_t ReadHoldingRegistersRequest::get_request_length() const
{
    return 5;
}

void ReadHoldingRegistersRequest::build_request(openpal::wseq_t& buffer) const
{
    buffer.put('h');
    buffer.put('e');
    buffer.put('l');
    buffer.put('l');
    buffer.put('o');


}

} // namespace modbus

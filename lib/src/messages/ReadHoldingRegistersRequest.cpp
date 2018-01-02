#include "modbus/messages/ReadHoldingRegistersRequest.h"

namespace modbus
{

ReadHoldingRegistersRequest::ReadHoldingRegistersRequest(Address startingAddress, uint16_t qtyOfRegisters)
:m_startingAddress{startingAddress},
 m_qtyOfRegisters{qtyOfRegisters}
{

}

void ReadHoldingRegistersRequest::BuildRequest(openpal::wseq_t& buffer)
{

}

} // namespace modbus

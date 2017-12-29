#include "modbus/messages/ReadHoldingRegistersRequest.h"

namespace modbus
{
namespace messages
{

ReadHoldingRegistersRequest::ReadHoldingRegistersRequest(Address startingAddress, uint16_t qtyOfRegisters)
:m_startingAddress(startingAddress),
 m_qtyOfRegisters(qtyOfRegisters)
{

}

void ReadHoldingRegistersRequest::BuildRequest(openpal::Buffer &buffer)
{

}

} // namespace messages
} // namespace modbus

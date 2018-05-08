#ifndef MODBUS_READHOLDINGREGISTERSRESPONSEIMPL_H
#define MODBUS_READHOLDINGREGISTERSRESPONSEIMPL_H

#include "messages/ReadRegistersResponseImpl.h"

namespace modbus
{

using ReadHoldingRegistersResponseImpl = ReadRegistersResponseImpl<0x03, ReadHoldingRegistersRequest, ReadHoldingRegistersResponse>;

} // namespace modbus

#endif //MODBUS_READHOLDINGREGISTERSRESPONSEIMPL_H

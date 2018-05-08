#ifndef MODBUS_READINPUTREGISTERSRESPONSEIMPL_H
#define MODBUS_READINPUTREGISTERSRESPONSEIMPL_H

#include "messages/ReadRegistersResponseImpl.h"

namespace modbus
{

using ReadInputRegistersResponseImpl = ReadRegistersResponseImpl<0x04, ReadInputRegistersRequest, ReadInputRegistersResponse>;

} // namespace modbus

#endif //MODBUS_READINPUTREGISTERSRESPONSEIMPL_H

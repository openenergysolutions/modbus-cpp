#ifndef MODBUS_READINPUTREGISTERSRESPONSE_H
#define MODBUS_READINPUTREGISTERSRESPONSE_H

#include "modbus/messages/ReadRegistersResponse.h"

namespace modbus
{

using ReadInputRegistersResponse = ReadRegistersResponse<0x04>;

} // namespace modbus

#endif //MODBUS_READINPUTREGISTERSRESPONSE_H

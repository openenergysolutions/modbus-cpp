#ifndef MODBUS_READHOLDINGREGISTERSRESPONSE_H
#define MODBUS_READHOLDINGREGISTERSRESPONSE_H

#include "modbus/messages/ReadRegistersResponse.h"

namespace modbus
{

using ReadHoldingRegistersResponse = ReadRegistersResponse<0x03>;

} // namespace modbus

#endif //MODBUS_READHOLDINGREGISTERSRESPONSE_H

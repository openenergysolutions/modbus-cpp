#ifndef MODBUS_READHOLDINGREGISTERSREQUEST_H
#define MODBUS_READHOLDINGREGISTERSREQUEST_H

#include "modbus/messages/ReadRegistersRequest.h"

namespace modbus
{

using ReadHoldingRegistersRequest = ReadRegistersRequest<0x03>;

} // namespace modbus

#endif //MODBUS_READHOLDINGREGISTERSREQUEST_H

#ifndef MODBUS_READINPUTREGISTERSREQUEST_H
#define MODBUS_READINPUTREGISTERSREQUEST_H

#include "modbus/messages/ReadRegistersRequest.h"

namespace modbus
{

using ReadInputRegistersRequest = ReadRegistersRequest<0x04>;

} // namespace modbus

#endif //MODBUS_READINPUTREGISTERSREQUEST_H

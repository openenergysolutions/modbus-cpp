#ifndef MODBUS_READINPUTREGISTERSREQUESTIMPL_H
#define MODBUS_READINPUTREGISTERSREQUESTIMPL_H

#include "messages/ReadRegistersRequestImpl.h"

namespace modbus
{

using ReadInputRegistersRequestImpl = ReadRegistersRequestImpl<0x04, ReadInputRegistersRequest>;

} // namespace modbus

#endif //MODBUS_READINPUTREGISTERSREQUESTIMPL_H

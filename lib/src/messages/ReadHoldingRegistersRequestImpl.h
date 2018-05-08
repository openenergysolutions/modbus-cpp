#ifndef MODBUS_READHOLDINGREGISTERSREQUESTIMPL_H
#define MODBUS_READHOLDINGREGISTERSREQUESTIMPL_H

#include "messages/ReadRegistersRequestImpl.h"

namespace modbus
{

using ReadHoldingRegistersRequestImpl = ReadRegistersRequestImpl<0x03, ReadHoldingRegistersRequest>;

} // namespace modbus

#endif //MODBUS_READHOLDINGREGISTERSREQUESTIMPL_H

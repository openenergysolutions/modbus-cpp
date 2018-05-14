#ifndef MODBUS_READCOILSREQUESTIMPL_H
#define MODBUS_READCOILSREQUESTIMPL_H

#include "messages/ReadBitsRequestImpl.h"

namespace modbus
{

using ReadCoilsRequestImpl = ReadBitsRequestImpl<0x01, ReadCoilsRequest>;

} // namespace modbus

#endif //MODBUS_READCOILSREQUESTIMPL_H

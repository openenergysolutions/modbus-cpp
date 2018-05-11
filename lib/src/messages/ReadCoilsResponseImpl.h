#ifndef MODBUS_READCOILSRESPONSEIMPL_H
#define MODBUS_READCOILSRESPONSEIMPL_H

#include "messages/ReadBitsResponseImpl.h"

namespace modbus
{

using ReadCoilsResponseImpl = ReadBitsResponseImpl<0x01, ReadCoilsRequest, ReadCoilsResponse>;

} // namespace modbus

#endif //MODBUS_READCOILSRESPONSEIMPL_H

#ifndef MODBUS_READDISCRETEINPUTSRESPONSEIMPL_H
#define MODBUS_READDISCRETEINPUTSRESPONSEIMPL_H

#include "messages/ReadBitsResponseImpl.h"

namespace modbus
{

using ReadDiscreteInputsResponseImpl = ReadBitsResponseImpl<0x02, ReadDiscreteInputsRequest, ReadDiscreteInputsResponse>;

} // namespace modbus

#endif //MODBUS_READDISCRETEINPUTSRESPONSEIMPL_H

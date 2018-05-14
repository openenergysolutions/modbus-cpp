#ifndef MODBUS_READDISCRETEINPUTSREQUESTIMPL_H
#define MODBUS_READDISCRETEINPUTSREQUESTIMPL_H

#include "messages/ReadBitsRequestImpl.h"

namespace modbus
{

using ReadDiscreteInputsRequestImpl = ReadBitsRequestImpl<0x02, ReadDiscreteInputsRequest>;

} // namespace modbus

#endif //MODBUS_READDISCRETEINPUTSREQUESTIMPL_H

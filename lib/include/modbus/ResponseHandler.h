#ifndef MODBUS_RESPONSEHANDLER_H
#define MODBUS_RESPONSEHANDLER_H

#include <functional>

#include "modbus/Expected.h"

namespace modbus
{

template<typename TResponse>
using ResponseHandler = std::function<void(const Expected<TResponse>&)>;

} // namespace modbus

#endif //MODBUS_RESPONSEHANDLER_H

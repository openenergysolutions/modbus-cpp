#ifndef MODBUS_RESPONSEHANDLER_H
#define MODBUS_RESPONSEHANDLER_H

#include <functional>

#include "modbus/messages/Exception.h"

namespace modbus
{

template<typename TResponse>
using ResponseHandler = std::function<void(const TResponse&, const messages::Exception&)>;

} // namespace modbus

#endif //MODBUS_RESPONSEHANDLER_H

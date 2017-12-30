#ifndef MODBUS_RESPONSEHANDLER_H
#define MODBUS_RESPONSEHANDLER_H

#include <functional>

namespace modbus
{

class Exception;

template<typename TResponse>
using ResponseHandler = std::function<void(const TResponse&, const Exception&)>;

} // namespace modbus

#endif //MODBUS_RESPONSEHANDLER_H

#ifndef MODBUS_RESPONSEHANDLER_H
#define MODBUS_RESPONSEHANDLER_H

#include <functional>

#include "modbus/Expected.h"

namespace modbus
{

/**
 * @brief Response handler used in callbacks
 *
 * The response handler will be called with the expected response. Be sure to
 * check that the expectation is valid with @ref Expected::is_valid() before using it.
 *
 * @warning All the response handlers are called from background threads created by
 * the @ref IModbusManager. Therefore, all response handlers *should not block* in any
 * way. Otherwise, you risk having other channels not performing their tasks because
 * the thread is blocked or even deadlocks. Also, be sure to use the appropriate mechanisms
 * to eliminate the risks of race conditions in the user code.
 *
 * @see @ref ISession
 */
template<typename TResponse>
using ResponseHandler = std::function<void(const Expected<TResponse>&)>;

} // namespace modbus

#endif //MODBUS_RESPONSEHANDLER_H

#ifndef MODBUS_TIMEOUTEXCEPTION_H
#define MODBUS_TIMEOUTEXCEPTION_H

#include "modbus/exceptions/IException.h"

namespace modbus
{

/**
 * @brief Timeout error
 *
 * This error is emitted whenever the timeout of a request is reached.
 */
class TimeoutException : public IException
{
public:
    /**
     * @brief Constructor
     */
    TimeoutException() : IException{"Timeout reached"} {}
};

} // namespace modbus

#endif //MODBUS_TIMEOUTEXCEPTION_H

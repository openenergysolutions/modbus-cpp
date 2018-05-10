#ifndef MODBUS_ITIMER_H
#define MODBUS_ITIMER_H

/** @file
* @brief Interface @ref modbus::ITimer
*/

#include "modbus/Typedefs.h"

namespace modbus
{

/**
 * @brief Timer interface for custom polling
 *
 * This class is returned by @ref ISession::start() method in order to offer the user
 * the ability to implement custom polling behaviors.
 */
class ITimer
{
public:
    virtual ~ITimer() = default;

    /**
     * @brief Cancel the execution of the timer
     *
     * The action won't be called except if it was already
     * queued in a thread.
     */
    virtual void cancel() = 0;

    /**
     * @brief Get the time at which the timer will expire
     * @return Time at which the timer will expire
     */
    virtual steady_time_t expires_at() = 0;
};

} // namespace modbus

#endif //MODBUS_ITIMER_H

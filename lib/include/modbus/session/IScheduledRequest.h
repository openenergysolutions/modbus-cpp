#ifndef MODBUS_ISCHEDULEDREQUEST_H
#define MODBUS_ISCHEDULEDREQUEST_H

/** @file
 * @brief Interface @ref modbus::IScheduledRequest
 */

#include <memory>

#include "modbus/Typedefs.h"

namespace modbus
{

/**
 * @brief Scheduled request
 *
 * An instance implementing this interface is returned by every scheduling
 * methods of @ref ISession. You can use to it to start the scheduled request,
 * to stop it or to change its polling frequency.
 *
 * The polling behavior is as follow. When the polling is started, a request is immediately
 * sent. When a response or an error is received or a timeout occurs, then a timer is started
 * to wait for the time configured. When this timer elapses, a request is sent and so on.
 * Therefore, it is important to note that the polling frequency is actually the *minimum*
 * time between each requests. In practive, the time between two requests will be longer than
 * what is set in the frequency, because it will include the round-trip on the wire.
 *
 * This design decision was made to eliminate the risks of clogging the channel with numerous
 * requests when a device is polled at small intervals but does not respond.
 *
 * @see ISession
 */
class IScheduledRequest : public std::enable_shared_from_this<IScheduledRequest>
{
public:
    /**
     * Destructor
     */
    virtual ~IScheduledRequest() = default;

    /**
     * @brief Start polling if not done running already
     *
     * The polling will start immediately in one of the background threads.
     */
    virtual void start() = 0;

    /**
     * @brief Stop the polling if it's running
     *
     * @note If a request is waiting for a response, the response will be waited
     * before effectively stopping the polling.
     */
    virtual void stop() = 0;

    /**
     * @brief Check if a scheduled request is currently running
     * @return @cpp true @ce if the scheduled request is running, @cpp false @ce otherwise
     *
     * @note When callind @ref start(), a following call to this method may still return
     * @cpp false @ce. This is due to the fact that @ref start() actually modifies the running
     * state in a background thread.
     */
    virtual bool is_running() const = 0;

    /**
     * @brief Modify the frequency of the polling
     * @param frequency New frequency to poll
     *
     * @note The new frequency will be effective after the *next* polling. In other words,
     * the timer used between polls is *not* modified by this method until it elapses.
     */
    virtual void set_frequency(const duration_t& frequency) = 0;

    /**
     * @brief Retrieve the frequency of the polling
     * @return Polling frequency
     *
     * @note When callind @ref set_frequency(), a following call to this method may still return the old
     * value. This is due to the fact that @ref set_frequency() actually modifies the frequency in
     * a background thread.
     */
    virtual duration_t get_frequency() const = 0;
};

} // namespace modbus

#endif //MODBUS_ISCHEDULEDREQUEST_H

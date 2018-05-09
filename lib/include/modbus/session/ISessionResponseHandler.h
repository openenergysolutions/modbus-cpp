#ifndef MODBUS_ISESSIONRESPONSEHANDLER_H
#define MODBUS_ISESSIONRESPONSEHANDLER_H

#include "modbus/exceptions/IException.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/messages/ReadInputRegistersResponse.h"

namespace modbus
{

/**
 * @brief Response handler for scheduled requests
 *
 * You should implement this interface to respond to scheduled requests of @ref ISession.
 * A shared pointer of an instance implementing this class is passed on channel creation
 * in @ref IChannel::create_session().
 *
 * For details on the polling behavior, see @ref IScheduledRequest.
 *
 * @warning All the response handlers are called from background threads created by
 * the @ref IModbusManager. Therefore, all response handlers *should not block* in any
 * way. Otherwise, you risk having other channels not performing their tasks because
 * the thread is blocked or even deadlocks. Also, be sure to use the appropriate mechanisms
 * to eliminate the risks of race conditions in the user code. See @ref IModbusManager and
 * @ref ResponseHandler for more details.
 *
 * @see @ref ISession
 */
class ISessionResponseHandler
{
public:
    /**
     * @brief Destructor
     */
    virtual ~ISessionResponseHandler() = default;

    /**
     * @brief Read Input Registers response handler
     * @param response Response receive
     *
     * This method is called whenever a scheduled @ref ReadInputRegistersRequest
     * receives a successful response.
     */
    virtual void on_response(const ReadInputRegistersResponse& response) {};

    /**
     * @brief Read Holding Registers response handler
     * @param response Response receive
     *
     * This method is called whenever a scheduled @ref ReadHoldingRegistersRequest
     * receives a successful response.
     */
    virtual void on_response(const ReadHoldingRegistersResponse& response) {};

    /**
     * @brief Error response handler
     * @param exception Exception received
     *
     * This method is called whenever a scheduled request produces an error
     * other than @ref TimeoutException. In the latter case, @ref on_timeout()
     * will be called.
     */
    virtual void on_exception(const IException& exception) {};

    /**
     * @brief Timeout response handler
     *
     * This method is called whenever a scheduled request did not receive a response
     * within a pre-determine time.
     */
    virtual void on_timeout() {};
};

} // namespace modbus

#endif //MODBUS_ISESSIONRESPONSEHANDLER_H

#ifndef MODBUS_ISESSION_H
#define MODBUS_ISESSION_H

/** @file
 * @brief Interface @ref modbus::ISession
 */

#include <memory>

#include "modbus/ITimer.h"
#include "modbus/ResponseHandler.h"
#include "modbus/Typedefs.h"
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/messages/ReadInputRegistersRequest.h"
#include "modbus/messages/ReadInputRegistersResponse.h"
#include "modbus/messages/WriteSingleRegisterRequest.h"
#include "modbus/messages/WriteSingleRegisterResponse.h"
#include "modbus/messages/WriteMultipleRegistersRequest.h"
#include "modbus/messages/WriteMultipleRegistersResponse.h"
#include "modbus/session/IScheduledRequest.h"

namespace modbus
{

/**
 * @brief Communication session with a Modbus device
 *
 * This class represents a session for communicatiing with a Modbus device. This is the class you'll be using
 * to send requests to the device and schedule them for periodic polling.
 *
 * An instance of this class can be obtained via @ref IChannel::create_session().
 *
 * It is possible to have multiple sessions on a single channel. For example, if you are communicating
 * with a gateway, you can have a single channel to the gateway and multiple sessions for each slave devices
 * connected to the gateway. The unit identifier for each device is set in @ref IChannel::create_session().
 *
 * All the responses of the scheduled requests are sent to the @ref ISessionResponseHandler registered
 * in @ref IChannel::create_session().
 *
 * For details on the polling behavior, see @ref IScheduledRequest.
 *
 * The default timeout is configured in @ref IChannel::create_session().
 *
 * This class provides access to timer in order to implement custom polling strategies. See
 * @ref start(const duration_t& duration, const action_t& action) and
 * @ref start(const steady_time_t& expiration, const action_t& action) for
 * more details.
 *
 * @warning All the response handlers are called from background threads created by
 * the @ref IModbusManager. Therefore, all response handlers *should not block* in any
 * way. Otherwise, you risk having other channels not performing their tasks because
 * the thread is blocked or even deadlocks. Also, be sure to use the appropriate mechanisms
 * to eliminate the risks of race conditions in the user code. See @ref IModbusManager and
 * @ref ResponseHandler for more details.
 *
 * @see @ref IModbusManager, @ref IChannel, @ref ISessionResponseHandler
 */
class ISession : public std::enable_shared_from_this<ISession>
{
public:
    /**
     * @brief Destructor
     *
     * @warning This destructor should never be called from user code. @ref IChannel
     * will hold a shared pointer to every sessions it has created and will release them in
     * @ref IChannel::shutdown(), which is probably called by @ref IModbusManager::shutdown().
     *
     * @see @ref IModbusManager::shutdown(), @ref IChannel::shutdown(), @ref shutdown()
     */
    virtual ~ISession() = default;

    /**
     * @brief Cancels all pending requests and all scheduled requests.
     *
     * @note This method is not usually called by user code, but by @ref IChannel::shutdown(), which is normally
     * called by @ref IModbusManager::shutdown(). It is
     * still possible to shutdown a session manually using this method, however please note that the
     * @ref IChannel will keep holding a shared pointer to it.
     *
     * @warning Once a session is shutdown, sending requests through it is not permitted. Sessions cannot
     * be restarted, but you can safely create a new one via @ref IChannel.
     *
     * @see @ref IModbusManager::shutdown(), @ref IChannel::shutdown()
     */
    virtual void shutdown() = 0;

    // ===== One-time requests =====
    /**
     * @brief Send a Read Holding Registers request to the device
     * @param request Request to send
     * @param handler Handler called when the response is received
     *
     * @note This method uses the default timout value set in @ref IChannel::create_session.
     */
    virtual void send_request(const ReadHoldingRegistersRequest& request,
                              ResponseHandler<ReadHoldingRegistersResponse> handler) = 0;
    /**
     * @brief Send a Read Holding Registers request to the device
     * @param request Request to send
     * @param timeout Maximum time to wait for a response
     * @param handler Handler called when the response is received
     */
    virtual void send_request(const ReadHoldingRegistersRequest& request,
                              const duration_t& timeout,
                              ResponseHandler<ReadHoldingRegistersResponse> handler) = 0;

    /**
     * @brief Send a Read Input Registers request to the device
     * @param request Request to send
     * @param handler Handler called when the response is received
     *
     * @note This method uses the default timout value set in @ref IChannel::create_session.
     */
    virtual void send_request(const ReadInputRegistersRequest& request,
                              ResponseHandler<ReadInputRegistersResponse> handler) = 0;
    /**
     * @brief Send a Read Input Registers request to the device
     * @param request Request to send
     * @param timeout Maximum time to wait for a response
     * @param handler Handler called when the response is received
     */
    virtual void send_request(const ReadInputRegistersRequest& request,
                              const duration_t& timeout,
                              ResponseHandler<ReadInputRegistersResponse> handler) = 0;

    /**
     * @brief Send a Write Single Register request to the device
     * @param request Request to send
     * @param handler Handler called when the response is received
     *
     * @note This method uses the default timout value set in @ref IChannel::create_session.
     */
    virtual void send_request(const WriteSingleRegisterRequest& request,
                              ResponseHandler<WriteSingleRegisterResponse> handler) = 0;
    /**
     * @brief Send a Write Single Register request to the device
     * @param request Request to send
     * @param timeout Maximum time to wait for a response
     * @param handler Handler called when the response is received
     */
    virtual void send_request(const WriteSingleRegisterRequest& request,
                              const duration_t& timeout,
                              ResponseHandler<WriteSingleRegisterResponse> handler) = 0;

    /**
     * @brief Send a Write Multiple Registers request to the device
     * @param request Request to send
     * @param handler Handler called when the response is received
     *
     * @note This method uses the default timout value set in @ref IChannel::create_session.
     */
    virtual void send_request(const WriteMultipleRegistersRequest& request,
                              ResponseHandler<WriteMultipleRegistersResponse> handler) = 0;
    /**
     * @brief Send a Write Multiple Registers request to the device
     * @param request Request to send
     * @param timeout Maximum time to wait for a response
     * @param handler Handler called when the response is received
     */
    virtual void send_request(const WriteMultipleRegistersRequest& request,
                              const duration_t& timeout,
                              ResponseHandler<WriteMultipleRegistersResponse> handler) = 0;

    // ===== Scheduled requests =====
    /**
     * @brief Schedule a Read Holding Registers request to be send periodically to the device
     * @param request Request to send periodically
     * @param frequency Frequency to send the request
     *
     * The @ref ISessionResponseHandler registered in @ref IChannel::create_session() will
     * be called everytime a response is received.
     *
     * @note This method uses the default timout value set in @ref IChannel::create_session.
     */
    virtual std::shared_ptr<IScheduledRequest> schedule_request(const ReadHoldingRegistersRequest& request,
                                                                const duration_t& frequency) = 0;
    /**
     * @brief Schedule a Read Holding Registers request to be send periodically to the device
     * @param request Request to send periodically
     * @param timeout Maximum time to wait for a response
     * @param frequency Frequency to send the request
     *
     * The @ref ISessionResponseHandler registered in @ref IChannel::create_session() will
     * be called everytime a response is received.
     */
    virtual std::shared_ptr<IScheduledRequest> schedule_request(const ReadHoldingRegistersRequest& request,
                                                                const duration_t& timeout,
                                                                const duration_t& frequency) = 0;

    /**
     * @brief Schedule a Read Input Registers request to be send periodically to the device
     * @param request Request to send periodically
     * @param frequency Frequency to send the request
     *
     * The @ref ISessionResponseHandler registered in @ref IChannel::create_session() will
     * be called everytime a response is received.
     *
     * @note This method uses the default timout value set in @ref IChannel::create_session.
     */
    virtual std::shared_ptr<IScheduledRequest> schedule_request(const ReadInputRegistersRequest& request,
                                                                const duration_t& frequency) = 0;
    /**
     * @brief Schedule a Read Input Registers request to be send periodically to the device
     * @param request Request to send periodically
     * @param timeout Maximum time to wait for a response
     * @param frequency Frequency to send the request
     *
     * The @ref ISessionResponseHandler registered in @ref IChannel::create_session() will
     * be called everytime a response is received.
     */
    virtual std::shared_ptr<IScheduledRequest> schedule_request(const ReadInputRegistersRequest& request,
                                                                const duration_t& timeout,
                                                                const duration_t& frequency) = 0;

    // ===== Underlying timer access =====
    /**
     * @brief Start a new timer based on relative time duration
     * @param duration  Relative time duration
     * @param action    Action to perform when the timer elapses
     * @return ITimer instance to manipulate
     */
    virtual std::unique_ptr<ITimer> start(const duration_t& duration, const action_t& action) = 0;

    /**
     * @brief Start a new timer based on an absolute timestamp
     * @param expiration    Timestamp after which the timer is considered elapsed
     * @param action        Action to perform when the timer elapses
     * @return ITimer instance to manipulate
     */
    virtual std::unique_ptr<ITimer> start(const steady_time_t& expiration, const action_t& action) = 0;
};

} // namespace modbus

#endif //MODBUS_ISESSION_H

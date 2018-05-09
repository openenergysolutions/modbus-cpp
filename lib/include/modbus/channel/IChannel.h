#ifndef MODBUS_ICHANNEL_H
#define MODBUS_ICHANNEL_H

#include <memory>
#include "exe4cpp/Typedefs.h"
#include "modbus/channel/UnitIdentifier.h"
#include "modbus/session/ISession.h"
#include "modbus/session/ISessionResponseHandler.h"

namespace modbus
{

/**
 * @brief Communication channel to a Modbus device
 *
 * This class represents a channel of communication with a Modbus device. It is responsible for
 * holding the physical connection, serializing the PDU in ADU and sequencing the outstanding requests.
 *
 * You cannot send requests directly from this class. You must create a session with the @ref create_session()
 * methods and send the requests from the returned @ref ISession.
 *
 * An instance of this class can be obtained via @ref IModbusManager::create_tcp_channel().
 *
 * @note Currently, the TCP channel supports only one outstanding request, meaning that the requests
 * are always sent sequentially. Therefore, before sending a new request, the channel waits for the response
 * of the previous request or its timeout.
 *
 * @see @ref IModbusManager, @ref ISession
 */
class IChannel : public std::enable_shared_from_this<IChannel>
{
public:
    /**
     * @brief Destructor
     *
     * @warning This destructor should never be called from user code. @ref IModbusManager
     * will hold a shared pointer to every channels it has created and will release them in
     * @ref IModbusManager::shutdown().
     *
     * @see @ref IModbusManager::shutdown(), @ref shutdown()
     */
    virtual ~IChannel() = default;

    /**
     * @brief Create a session using the channel
     * @param unit_identifier           Modbus unit identifier of the device. In TCP, this identifier is useless,
     *                                  but it must fit the slave unit identifier.
     * @param default_timeout           Default timeout of requests sent on the wire
     * @param session_response_handler  Global session response handler called on responses of scheduled response.
     *                                  This parameter can be a @cpp nullptr @ce if no request will be scheduled.
     * @return Shared pointer to an instance of @ref ISession
     */
    virtual std::shared_ptr<ISession> create_session(const UnitIdentifier& unit_identifier,
                                                     const exe4cpp::duration_t& default_timeout,
                                                     std::shared_ptr<ISessionResponseHandler> session_response_handler) = 0;

    /**
     * @brief Closes all the associated sessions.
     *
     * All the associated sessions are gracefully shutdown.
     *
     * @note This method is not usually called by user code, but by @ref IModbusManager::shutdown(). It is
     * still possible to shutdown a channel manually using this method, however please note that the
     * @ref IModbusManager will keep holding a shared pointer to it.
     *
     * @warning Once a channel is shutdown, sending requests through it is not permitted. Channels cannot
     * be restarted, but you can safely create a new one via @ref IModbusManager.
     *
     * @see @ref IModbusManager::shutdown()
     */
    virtual void shutdown() = 0;
};

} // namespace modbus

#endif //MODBUS_ICHANNEL_H

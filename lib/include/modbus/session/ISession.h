#ifndef MODBUS_ISESSION_H
#define MODBUS_ISESSION_H

#include <memory>

#include "loopser/executor/Typedefs.h"
#include "loopser/executor/Timer.h"

#include "modbus/ResponseHandler.h"
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"
#include "modbus/messages/ReadInputRegistersRequest.h"
#include "modbus/messages/ReadInputRegistersResponse.h"

namespace modbus
{

class IScheduledRequest;
class WriteSingleRegisterRequest;
class WriteSingleRegisterResponse;
class WriteMultipleRegistersRequest;
class WriteMultipleRegistersResponse;

class ISession : public std::enable_shared_from_this<ISession>
{
public:
    virtual ~ISession() = default;

    virtual void shutdown() = 0;

    // One-time requests
    virtual void send_request(const ReadHoldingRegistersRequest& request,
                              ResponseHandler<ReadHoldingRegistersResponse> handler) = 0;
    virtual void send_request(const ReadHoldingRegistersRequest& request,
                              const loopser::duration_t& timeout,
                              ResponseHandler<ReadHoldingRegistersResponse> handler) = 0;

    virtual void send_request(const ReadInputRegistersRequest& request,
                              ResponseHandler<ReadInputRegistersResponse> handler) = 0;
    virtual void send_request(const ReadInputRegistersRequest& request,
                              const loopser::duration_t& timeout,
                              ResponseHandler<ReadInputRegistersResponse> handler) = 0;

    virtual void send_request(const WriteSingleRegisterRequest& request,
                              ResponseHandler<WriteSingleRegisterResponse> handler) = 0;
    virtual void send_request(const WriteSingleRegisterRequest& request,
                              const loopser::duration_t& timeout,
                              ResponseHandler<WriteSingleRegisterResponse> handler) = 0;

    virtual void send_request(const WriteMultipleRegistersRequest& request,
                              ResponseHandler<WriteMultipleRegistersResponse> handler) = 0;
    virtual void send_request(const WriteMultipleRegistersRequest& request,
                              const loopser::duration_t& timeout,
                              ResponseHandler<WriteMultipleRegistersResponse> handler) = 0;

    // Scheduled requests
    virtual std::shared_ptr<IScheduledRequest> schedule_request(const ReadHoldingRegistersRequest& request,
                                                                const loopser::duration_t& frequency) = 0;
    virtual std::shared_ptr<IScheduledRequest> schedule_request(const ReadHoldingRegistersRequest& request,
                                                                const loopser::duration_t& timeout,
                                                                const loopser::duration_t& frequency) = 0;

    virtual std::shared_ptr<IScheduledRequest> schedule_request(const ReadInputRegistersRequest& request,
                                                                const loopser::duration_t& frequency) = 0;
    virtual std::shared_ptr<IScheduledRequest> schedule_request(const ReadInputRegistersRequest& request,
                                                                const loopser::duration_t& timeout,
                                                                const loopser::duration_t& frequency) = 0;

    // ---- provide access to the underlying timer implementation so that users can perform asynchronous timers -----

    /// @return start a new timer based on a relative time duration
    virtual loopser::Timer start(const loopser::duration_t& duration, const loopser::action_t& action) = 0;

    /// @return start a new timer based on an absolute timestamp of the steady clock
    virtual loopser::Timer start(const loopser::steady_time_t& expiration, const loopser::action_t& action) = 0;
};

} // namespace modbus

#endif //MODBUS_ISESSION_H

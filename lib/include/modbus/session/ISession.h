#ifndef MODBUS_ISESSION_H
#define MODBUS_ISESSION_H

#include <memory>

#include "openpal/executor/Typedefs.h"

#include "modbus/ResponseHandler.h"

namespace modbus
{

class ISchedule;
class ReadHoldingRegistersRequest;
class ReadHoldingRegistersResponse;
class ReadInputRegistersRequest;
class ReadInputRegistersResponse;
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
                              const openpal::duration_t& timeout,
                              ResponseHandler<ReadHoldingRegistersResponse> handler) = 0;

    virtual void send_request(const ReadInputRegistersRequest& request,
                              ResponseHandler<ReadInputRegistersResponse> handler) = 0;
    virtual void send_request(const ReadInputRegistersRequest& request,
                              const openpal::duration_t& timeout,
                              ResponseHandler<ReadInputRegistersResponse> handler) = 0;

    virtual void send_request(const WriteSingleRegisterRequest& request,
                              ResponseHandler<WriteSingleRegisterResponse> handler) = 0;
    virtual void send_request(const WriteSingleRegisterRequest& request,
                              const openpal::duration_t& timeout,
                              ResponseHandler<WriteSingleRegisterResponse> handler) = 0;

    virtual void send_request(const WriteMultipleRegistersRequest& request,
                              ResponseHandler<WriteMultipleRegistersResponse> handler) = 0;
    virtual void send_request(const WriteMultipleRegistersRequest& request,
                              const openpal::duration_t& timeout,
                              ResponseHandler<WriteMultipleRegistersResponse> handler) = 0;

    // Scheduled requests
    virtual void schedule_request(const ReadHoldingRegistersRequest& request,
                                  std::unique_ptr<ISchedule> schedule) = 0;
    virtual void schedule_request(const ReadHoldingRegistersRequest& request,
                                  const openpal::duration_t& timeout,
                                  std::unique_ptr<ISchedule> schedule) = 0;

    virtual void schedule_request(const ReadInputRegistersRequest& request,
                                  std::unique_ptr<ISchedule> schedule) = 0;
    virtual void schedule_request(const ReadInputRegistersRequest& request,
                                  const openpal::duration_t& timeout,
                                  std::unique_ptr<ISchedule> schedule) = 0;
};

} // namespace modbus

#endif //MODBUS_ISESSION_H

#ifndef MODBUS_ISESSIONMOCK_H
#define MODBUS_ISESSIONMOCK_H

#include "modbus/session/ISession.h"

class ISessionMock : public modbus::ISession
{
public:
    ISessionMock();

    void shutdown() override;

    // One-time requests
    void send_request(const modbus::ReadHoldingRegistersRequest& request,
                      modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler) override;
    void send_request(const modbus::ReadHoldingRegistersRequest& request,
                      const openpal::duration_t& timeout,
                      modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler) override;

    void send_request(const modbus::ReadInputRegistersRequest& request,
                      modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler) override;
    void send_request(const modbus::ReadInputRegistersRequest& request,
                      const openpal::duration_t& timeout,
                      modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler) override;

    void send_request(const modbus::WriteSingleRegisterRequest& request,
                      modbus::ResponseHandler<modbus::WriteSingleRegisterResponse> handler) override;
    void send_request(const modbus::WriteSingleRegisterRequest& request,
                      const openpal::duration_t& timeout,
                      modbus::ResponseHandler<modbus::WriteSingleRegisterResponse> handler) override;

    void send_request(const modbus::WriteMultipleRegistersRequest& request,
                      modbus::ResponseHandler<modbus::WriteMultipleRegistersResponse> handler) override;
    void send_request(const modbus::WriteMultipleRegistersRequest& request,
                      const openpal::duration_t& timeout,
                      modbus::ResponseHandler<modbus::WriteMultipleRegistersResponse> handler) override;

    // Scheduled requests
    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadHoldingRegistersRequest& request,
                                                        std::unique_ptr<modbus::ISchedule> schedule) override;
    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadHoldingRegistersRequest& request,
                                                        const openpal::duration_t& timeout,
                                                        std::unique_ptr<modbus::ISchedule> schedule) override;

    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadInputRegistersRequest& request,
                                                        std::unique_ptr<modbus::ISchedule> schedule) override;
    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadInputRegistersRequest& request,
                                                        const openpal::duration_t& timeout,
                                                        std::unique_ptr<modbus::ISchedule> schedule) override;

    bool is_shutdown_called() const;
    unsigned int get_num_read_holding_registers_request_sent() const;
    modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> get_last_read_holding_registers_request_handler() const;

private:
    bool m_shutdown_called;
    unsigned int m_num_read_holding_registers_request_sent;
    modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> m_last_read_holding_registers_request_handler;
};

#endif //MODBUS_ISESSIONMOCK_H

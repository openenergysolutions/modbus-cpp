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
                      const modbus::duration_t& timeout,
                      modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler) override;

    void send_request(const modbus::ReadInputRegistersRequest& request,
                      modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler) override;
    void send_request(const modbus::ReadInputRegistersRequest& request,
                      const modbus::duration_t& timeout,
                      modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler) override;

    void send_request(const modbus::WriteSingleRegisterRequest& request,
                      modbus::ResponseHandler<modbus::WriteSingleRegisterResponse> handler) override;
    void send_request(const modbus::WriteSingleRegisterRequest& request,
                      const modbus::duration_t& timeout,
                      modbus::ResponseHandler<modbus::WriteSingleRegisterResponse> handler) override;

    void send_request(const modbus::WriteMultipleRegistersRequest& request,
                      modbus::ResponseHandler<modbus::WriteMultipleRegistersResponse> handler) override;
    void send_request(const modbus::WriteMultipleRegistersRequest& request,
                      const modbus::duration_t& timeout,
                      modbus::ResponseHandler<modbus::WriteMultipleRegistersResponse> handler) override;

    // Scheduled requests
    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadHoldingRegistersRequest& request,
                                                                const modbus::duration_t& frequency,
                                                                modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler) override;
    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadHoldingRegistersRequest& request,
                                                                const modbus::duration_t& timeout,
                                                                const modbus::duration_t& frequency,
                                                                modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler) override;

    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadInputRegistersRequest& request,
                                                                const modbus::duration_t& frequency,
                                                                modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler) override;
    std::shared_ptr<modbus::IScheduledRequest> schedule_request(const modbus::ReadInputRegistersRequest& request,
                                                                const modbus::duration_t& timeout,
                                                                const modbus::duration_t& frequency,
                                                                modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler) override;

    std::unique_ptr<modbus::ITimer> start(const modbus::duration_t &duration, const modbus::action_t &action) override;
    std::unique_ptr<modbus::ITimer> start(const modbus::steady_time_t &expiration, const modbus::action_t &action) override;


    unsigned int get_num_read_holding_registers_request_sent() const;
    modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> get_last_read_holding_registers_request_handler() const;

private:
    unsigned int m_num_read_holding_registers_request_sent;
    modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> m_last_read_holding_registers_request_handler;
};

#endif //MODBUS_ISESSIONMOCK_H

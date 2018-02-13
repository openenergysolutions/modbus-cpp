#include "mocks/ISessionMock.h"

ISessionMock::ISessionMock()
    :m_shutdown_called{false},
     m_num_read_holding_registers_request_sent{0},
     m_last_read_holding_registers_request_handler{nullptr}
{

}

void ISessionMock::shutdown()
{
    m_shutdown_called = true;
}

// One-time requests
void ISessionMock::send_request(const modbus::ReadHoldingRegistersRequest& request,
                                modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler)
{
    send_request(request, std::chrono::seconds(1), handler);
}

void ISessionMock::send_request(const modbus::ReadHoldingRegistersRequest& request,
                                const openpal::duration_t& timeout,
                                modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler)
{
    ++m_num_read_holding_registers_request_sent;
    m_last_read_holding_registers_request_handler = handler;
}

void ISessionMock::send_request(const modbus::ReadInputRegistersRequest& request,
                                modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler)
{

}

void ISessionMock::send_request(const modbus::ReadInputRegistersRequest& request,
                                const openpal::duration_t& timeout,
                                modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler)
{

}

void ISessionMock::send_request(const modbus::WriteSingleRegisterRequest& request,
                                modbus::ResponseHandler<modbus::WriteSingleRegisterResponse> handler)
{

}

void ISessionMock::send_request(const modbus::WriteSingleRegisterRequest& request,
                                const openpal::duration_t& timeout,
                                modbus::ResponseHandler<modbus::WriteSingleRegisterResponse> handler)
{

}

void ISessionMock::send_request(const modbus::WriteMultipleRegistersRequest& request,
                                modbus::ResponseHandler<modbus::WriteMultipleRegistersResponse> handler)
{

}

void ISessionMock::send_request(const modbus::WriteMultipleRegistersRequest& request,
                                const openpal::duration_t& timeout,
                                modbus::ResponseHandler<modbus::WriteMultipleRegistersResponse> handler)
{

}

// Scheduled requests
std::shared_ptr<modbus::IScheduledRequest> ISessionMock::schedule_request(const modbus::ReadHoldingRegistersRequest& request,
                                                                          std::unique_ptr<modbus::ISchedule> schedule)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> ISessionMock::schedule_request(const modbus::ReadHoldingRegistersRequest& request,
                                                                          const openpal::duration_t& timeout,
                                                                          std::unique_ptr<modbus::ISchedule> schedule)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> ISessionMock::schedule_request(const modbus::ReadInputRegistersRequest& request,
                                                                          std::unique_ptr<modbus::ISchedule> schedule)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> ISessionMock::schedule_request(const modbus::ReadInputRegistersRequest& request,
                                                                          const openpal::duration_t& timeout,
                                                                          std::unique_ptr<modbus::ISchedule> schedule)
{
    return nullptr;
}

bool ISessionMock::is_shutdown_called() const
{
    return m_shutdown_called;
}

unsigned int ISessionMock::get_num_read_holding_registers_request_sent() const
{
    return m_num_read_holding_registers_request_sent;
}

modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> ISessionMock::get_last_read_holding_registers_request_handler() const
{
    return m_last_read_holding_registers_request_handler;
}

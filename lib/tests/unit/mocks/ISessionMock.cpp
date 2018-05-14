#include "mocks/ISessionMock.h"

ISessionMock::ISessionMock()
    :m_num_read_holding_registers_request_sent{0},
     m_last_read_holding_registers_request_handler{nullptr}
{

}

void ISessionMock::shutdown()
{

}

// One-time requests
void ISessionMock::send_request(const modbus::ReadCoilsRequest& request,
                                modbus::ResponseHandler<modbus::ReadCoilsResponse> handler)
{

}

void ISessionMock::send_request(const modbus::ReadCoilsRequest& request,
                                const modbus::duration_t& timeout,
                                modbus::ResponseHandler<modbus::ReadCoilsResponse> handler)
{

}

void ISessionMock::send_request(const modbus::ReadDiscreteInputsRequest& request,
                                modbus::ResponseHandler<modbus::ReadDiscreteInputsResponse> handler)
{

}

void ISessionMock::send_request(const modbus::ReadDiscreteInputsRequest& request,
                                const modbus::duration_t& timeout,
                                modbus::ResponseHandler<modbus::ReadDiscreteInputsResponse> handler)
{

}

void ISessionMock::send_request(const modbus::ReadHoldingRegistersRequest& request,
                                modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler)
{
    send_request(request, std::chrono::seconds(1), handler);
}

void ISessionMock::send_request(const modbus::ReadHoldingRegistersRequest& request,
                                const modbus::duration_t& timeout,
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
                                const modbus::duration_t& timeout,
                                modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler)
{

}

void ISessionMock::send_request(const modbus::WriteSingleCoilRequest& request,
                                modbus::ResponseHandler<modbus::WriteSingleCoilResponse> handler)
{

}

void ISessionMock::send_request(const modbus::WriteSingleCoilRequest& request,
                                const modbus::duration_t& timeout,
                                modbus::ResponseHandler<modbus::WriteSingleCoilResponse> handler)
{

}

void ISessionMock::send_request(const modbus::WriteSingleRegisterRequest& request,
                                modbus::ResponseHandler<modbus::WriteSingleRegisterResponse> handler)
{

}

void ISessionMock::send_request(const modbus::WriteSingleRegisterRequest& request,
                                const modbus::duration_t& timeout,
                                modbus::ResponseHandler<modbus::WriteSingleRegisterResponse> handler)
{

}

void ISessionMock::send_request(const modbus::WriteMultipleCoilsRequest& request,
                                modbus::ResponseHandler<modbus::WriteMultipleCoilsResponse> handler)
{

}

void ISessionMock::send_request(const modbus::WriteMultipleCoilsRequest& request,
                                const modbus::duration_t& timeout,
                                modbus::ResponseHandler<modbus::WriteMultipleCoilsResponse> handler)
{

}

void ISessionMock::send_request(const modbus::WriteMultipleRegistersRequest& request,
                                modbus::ResponseHandler<modbus::WriteMultipleRegistersResponse> handler)
{

}

void ISessionMock::send_request(const modbus::WriteMultipleRegistersRequest& request,
                                const modbus::duration_t& timeout,
                                modbus::ResponseHandler<modbus::WriteMultipleRegistersResponse> handler)
{

}

// Scheduled requests
std::shared_ptr<modbus::IScheduledRequest> ISessionMock::schedule_request(const modbus::ReadCoilsRequest& request,
                                                                          const modbus::duration_t& frequency,
                                                                          modbus::ResponseHandler<modbus::ReadCoilsResponse> handler)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> ISessionMock::schedule_request(const modbus::ReadCoilsRequest& request,
                                                                          const modbus::duration_t& timeout,
                                                                          const modbus::duration_t& frequency,
                                                                          modbus::ResponseHandler<modbus::ReadCoilsResponse> handler)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> ISessionMock::schedule_request(const modbus::ReadDiscreteInputsRequest& request,
                                                                          const modbus::duration_t& frequency,
                                                                          modbus::ResponseHandler<modbus::ReadDiscreteInputsResponse> handler)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> ISessionMock::schedule_request(const modbus::ReadDiscreteInputsRequest& request,
                                                                          const modbus::duration_t& timeout,
                                                                          const modbus::duration_t& frequency,
                                                                          modbus::ResponseHandler<modbus::ReadDiscreteInputsResponse> handler)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> ISessionMock::schedule_request(const modbus::ReadHoldingRegistersRequest& request,
                                                                          const modbus::duration_t& frequency,
                                                                          modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> ISessionMock::schedule_request(const modbus::ReadHoldingRegistersRequest& request,
                                                                          const modbus::duration_t& timeout,
                                                                          const modbus::duration_t& frequency,
                                                                          modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> handler)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> ISessionMock::schedule_request(const modbus::ReadInputRegistersRequest& request,
                                                                          const modbus::duration_t& frequency,
                                                                          modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler)
{
    return nullptr;
}

std::shared_ptr<modbus::IScheduledRequest> ISessionMock::schedule_request(const modbus::ReadInputRegistersRequest& request,
                                                                          const modbus::duration_t& timeout,
                                                                          const modbus::duration_t& frequency,
                                                                          modbus::ResponseHandler<modbus::ReadInputRegistersResponse> handler)
{
    return nullptr;
}

unsigned int ISessionMock::get_num_read_holding_registers_request_sent() const
{
    return m_num_read_holding_registers_request_sent;
}

modbus::ResponseHandler<modbus::ReadHoldingRegistersResponse> ISessionMock::get_last_read_holding_registers_request_handler() const
{
    return m_last_read_holding_registers_request_handler;
}

std::unique_ptr<modbus::ITimer> ISessionMock::start(const modbus::duration_t &duration, const modbus::action_t &action)
{
    return nullptr;
}

std::unique_ptr<modbus::ITimer> ISessionMock::start(const modbus::steady_time_t &expiration, const modbus::action_t &action)
{
    return nullptr;
}

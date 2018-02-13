#include "mocks/ISessionResponseHandlerMock.h"

ISessionResponseHandlerMock::ISessionResponseHandlerMock()
    : m_num_read_holding_registers_response_received{0},
      m_num_exceptions_received{0},
      m_num_timeouts_received{0}
{

}

void ISessionResponseHandlerMock::on_response(const modbus::ReadInputRegistersResponse& response)
{

}

void ISessionResponseHandlerMock::on_response(const modbus::ReadHoldingRegistersResponse& response)
{
    ++m_num_read_holding_registers_response_received;
}

void ISessionResponseHandlerMock::on_exception(const modbus::IException& exception)
{
    ++m_num_exceptions_received;
}

void ISessionResponseHandlerMock::on_timeout()
{
    ++m_num_timeouts_received;
}

unsigned int ISessionResponseHandlerMock::get_num_read_holding_registers_response_received() const
{
    return m_num_read_holding_registers_response_received;
}

unsigned int ISessionResponseHandlerMock::get_num_exceptions_received() const
{
    return m_num_exceptions_received;
}

unsigned int ISessionResponseHandlerMock::get_num_timeouts_received() const
{
    return m_num_timeouts_received;
}

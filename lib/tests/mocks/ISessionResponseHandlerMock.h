#ifndef MODBUS_ISESSIONRESPONSEHANDLERMOCK_H
#define MODBUS_ISESSIONRESPONSEHANDLERMOCK_H

#include "modbus/session/ISessionResponseHandler.h"

class ISessionResponseHandlerMock : public modbus::ISessionResponseHandler
{
public:
    ISessionResponseHandlerMock();

    void on_response(const modbus::ReadInputRegistersResponse& response) override;
    void on_response(const modbus::ReadHoldingRegistersResponse& response) override;

    void on_exception(const modbus::IException& exception) override;
    void on_timeout() override;

    unsigned int get_num_read_holding_registers_response_received() const;
    unsigned int get_num_exceptions_received() const;
    unsigned int get_num_timeouts_received() const;

private:
    unsigned int m_num_read_holding_registers_response_received;
    unsigned int m_num_exceptions_received;
    unsigned int m_num_timeouts_received;
};

#endif //MODBUS_ISESSIONRESPONSEHANDLERMOCK_H

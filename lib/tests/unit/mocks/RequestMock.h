#ifndef MODBUS_REQUESTMOCK_H
#define MODBUS_REQUESTMOCK_H

#include "modbus/messages/IRequest.h"

class RequestMock : public modbus::IRequest
{
public:
    RequestMock(std::size_t length, uint8_t value);

    std::unique_ptr<modbus::IRequest> clone() const override;

    size_t get_request_length() const override;
    void build_request(loopser::wseq_t& buffer) const override;

private:
    std::size_t m_length;
    uint8_t m_value;
};

#endif //MODBUS_REQUESTMOCK_H

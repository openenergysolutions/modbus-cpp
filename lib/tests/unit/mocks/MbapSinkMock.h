#ifndef MODBUS_MBAPSINKMOCK_H
#define MODBUS_MBAPSINKMOCK_H

#include <vector>
#include "channel/IMbapSink.h"

class MbapSinkMock : public modbus::IMbapSink
{
public:
    void on_mbap_message(const modbus::MbapMessage& message) override;

    std::size_t get_num_messages() const;
    const std::vector<modbus::MbapMessage>& get_messages() const;

private:
    std::vector<modbus::MbapMessage> m_messages;
};

#endif //MODBUS_MBAPSINKMOCK_H

#include "mocks/MbapSinkMock.h"

void MbapSinkMock::on_mbap_message(const modbus::MbapMessage& message)
{
    m_messages.push_back(message);
}

std::size_t MbapSinkMock::get_num_messages() const
{
    return m_messages.size();
}

const std::vector<modbus::MbapMessage>& MbapSinkMock::get_messages() const
{
    return m_messages;
}

#include "modbus/Ipv4Endpoint.h"

namespace modbus
{

Ipv4Endpoint::Ipv4Endpoint(const std::string& hostname, uint32_t port)
        : m_hostname{hostname},
          m_port{port}
{

}

const std::string& Ipv4Endpoint::get_hostname() const
{
    return m_hostname;
}

uint32_t Ipv4Endpoint::get_port() const
{
    return m_port;
}

bool Ipv4Endpoint::operator==(const Ipv4Endpoint& other) const
{
    return m_hostname == other.m_hostname &&
           m_port == other.m_port;
}

bool Ipv4Endpoint::operator!=(const Ipv4Endpoint& other) const
{
    return !(*this == other);
}

} // namespace modbus
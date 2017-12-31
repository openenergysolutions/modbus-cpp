#ifndef MODBUS_IPV4ENDPOINT_H
#define MODBUS_IPV4ENDPOINT_H

#include <cstdint>
#include <string>

namespace modbus
{

class Ipv4Endpoint
{
public:
    Ipv4Endpoint(const std::string& hostname, uint32_t port);

    const std::string& GetHostname() const;
    uint32_t GetPort() const;

    bool operator==(const Ipv4Endpoint& other) const;
    bool operator!=(const Ipv4Endpoint& other) const;

private:
    std::string m_hostname;
    uint32_t m_port;
};

} // namespace modbus

#endif //MODBUS_IPV4ENDPOINT_H

#ifndef MODBUS_IPV4ENDPOINT_H
#define MODBUS_IPV4ENDPOINT_H

/** @file
* @brief Class @ref modbus::Ipv4Endpoint
*/

#include <cstdint>
#include <string>

namespace modbus
{

/**
 * @brief IPv4 endpoint representation
 *
 * Used when creating a TCP channel in @ref IModbusManager::create_tcp_channel().
 */
class Ipv4Endpoint
{
public:
    /**
     * @brief Constructor
     * @param hostname  Hostname. The hostname can be an IP address or a resolvable name.
     * @param port      Port number. Modbus devices should normally communicate via port 502.
     *
     * Example:
     *
     * @cpp
     * // IP address endpoint
     * auto Ipv4Endpoint ip_endpoint{"127.0.0.1", 502};
     *
     * // DNS endpoint
     * auto Ipv4Endpoint dns_endpoint{"foo.com", 8888};
     * @ce
     */
    Ipv4Endpoint(const std::string& hostname, uint32_t port);

    /**
     * @brief Returns the hostname of the endpoint
     * @return String representing the hostname of the endpoint
     *
     * The string may be an IP address or a resolvable name.
     */
    const std::string& get_hostname() const;

    /**
     * @brief Returns the port of the endpoint
     * @return Port of the endpoint
     */
    uint32_t get_port() const;

    /**
     * @brief Checks if two endpoints are the same
     * @param other Other endpoint to compare
     * @return @cpp true @ce if the endpoints are the same, @cpp false @ce otherwise.
     *
     * For two endpoints to be equal, they must have the same hostname string
     * and the same port.
     *
     * @note An endpoint with the IP address of a resolved DNS endpoint are *not*
     * considered equal. This method only does a string comparison.
     *
     * @see @ref operator!=()
     */
    bool operator==(const Ipv4Endpoint& other) const;

    /**
     * @brief Checks if two endpoints are different
     * @param other Other endpoint to compare
     * @return @cpp true @ce if the endpoints are different, @cpp false @ce otherwise.
     *
     * For two endpoints to be different, they must have a different hostname string
     * and/or a different port.
     *
     * @note An endpoint with the IP address of a resolved DNS endpoint are
     * considered *different*. This method only does a string comparison.
     *
     * @see @ref operator==()
     */
    bool operator!=(const Ipv4Endpoint& other) const;

private:
    std::string m_hostname;
    uint32_t m_port;
};

} // namespace modbus

#endif //MODBUS_IPV4ENDPOINT_H

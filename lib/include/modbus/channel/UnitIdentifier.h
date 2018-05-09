#ifndef MODBUS_UNITIDENTIFIER_H
#define MODBUS_UNITIDENTIFIER_H

#include <cstdint>
#include <ostream>

/** @file
 * @brief Class @ref modbus::UnitIdentifier
 */

namespace modbus
{

/**
 * @brief Modbus unit identifier
 *
 * Represents the 8-bit unit identifier used in the MBAP messages. When sending a request, this identifier
 * must fit with the one reported by the slave. This feature is mainly used in Modbus+ and Modbus serial lines.
 * However, it must be set properly on TCP/IP only sessions too.
 *
 * @see @ref IChannel::create_session()
 */
class UnitIdentifier
{
    constexpr static uint8_t DefaultUnitIdentifier = 0xFF;

public:
    /**
     * @brief Returns the default unit identifier
     * @returns Default unit identifier instance
     *
     * Its value is @cpp 0xFF @ce.
     */
    static UnitIdentifier default_unit_identifier()
    {
        return UnitIdentifier(DefaultUnitIdentifier);
    }

public:
    /**
     * @brief Constructor
     * @param value Unit identifier 8-bit value
     * @see @ref default_unit_identifier()
     */
    UnitIdentifier(uint8_t value);

    /**
     * @brief Get the 8-bit value of the unit identifier
     * @return 8-bit value of the unit identifier
     */
    uint8_t get_value() const;

    /**
     * @brief Check if two unit identifiers are identical
     * @param other Other unit identifier to compare
     * @return @cpp true @ce if the unit identifiers are the same, @cpp false @ce otherwise
     */
    bool operator==(const UnitIdentifier& other) const;

    /**
     * @brief Check if two unit identifiers are different
     * @param other Other unit identifier to compare
     * @return @cpp true @ce if the unit identifiers are different, @cpp false @ce otherwise
     */
    bool operator!=(const UnitIdentifier& other) const;

private:
    uint8_t m_value;
};

/**
 * @brief Format a unit identifier
 * @param stream    Stream to write to
 * @param unit_id   Unit identifier to write
 * @return Stream passed in for method chaining
 *
 * This function is useful for logging purposes.
 */
std::ostream& operator<<(std::ostream& stream, const UnitIdentifier& unit_id);

} // namespace modbus

#endif //MODBUS_UNITIDENTIFIER_H

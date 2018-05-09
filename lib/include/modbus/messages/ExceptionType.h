#ifndef MODBUS_EXCEPTIONTYPE_H
#define MODBUS_EXCEPTIONTYPE_H

#include <ostream>

namespace modbus
{

/**
 * @brief Modbus exception type
 *
 * See section *7: MODBUS Exception Responses* for more details about the exception.
 */
enum class ExceptionType
{
    /**
     * @cpp 0x01 @ce Illegal function
     *
     * Function is not allowed on the slave, or the slave is not in a state
     * where it can respond to this type of request.
     */
    IllegalFunction = 0x01,

    /**
     * @cpp 0x02 @ce Illegal data address
     *
     * The address range does not fit in the device mapping.
     */
    IllegalDataAddress = 0x02,

    /**
     * @cpp 0x03 @ce Illegal data value
     *
     * A value in the query is not valid (e.g. length not set properly).
     * It does *not* mean the value is outside the expectation of the application program.
     */
    IllegalDataValue = 0x03,

    /**
     * @cpp 0x04 @ce Server device failure
     *
     * An unknown error occured on the slave device.
     */
    ServerDeviceFailure = 0x04,

    /**
     * @cpp 0x05 @ce Acknowledge
     *
     * The request was accepted, but it requires a long processing type.
     *
     * Used with programming commands (not supported by this library)
     */
    Acknowledge = 0x05,

    /**
     * @cpp 0x06 @ce Server device busy
     *
     * The server is already processing a long-duration program.
     *
     * Used with programming commands (not supported by this library)
     */
    ServerDeviceBusy = 0x06,

    /**
     * @cpp 0x07 @ce Memory parity error
     *
     * The extended file area failed to pass a consistency check.
     *
     * Used with function codes @cpp 0x20 @ce and @cpp 0x21 @ce (not supported by this library)
     */
    MemoryParityError = 0x07,

    /**
     * @cpp 0x0A @ce Gateway path unavailable
     *
     * When use with a gateway, the gateway could not map the input port to the output port.
     */
    GatewayPathUnavailable = 0x0A,

    /**
     * @cpp 0x0B @ce Gateway target device failed to respond
     *
     * When use with a gateway, no response was received from the target device.
     */
    GatewayTargetDeviceFailedToRespond = 0x0B,

    /**
     * Unknown Modbus exception.
     *
     * Notify the vendor it should not respond with this message.
     */
    Unknown
};

/**
 * @brief Parse a Modbus exception from an 8-bit value
 * @param data 8-bit value to parse
 * @return Modbus exception (@ref ExceptionType::Unknown if the exception is unknown)
 */
ExceptionType parse_exception_type(uint8_t data);

/**
 * @brief Format a unit identifier
 * @param stream            Stream to write to
 * @param exception_type    Modbus exception type
 * @return Stream passed in for method chaining
 *
 * This function is useful for logging purposes.
 */
std::ostream& operator<<(std::ostream& stream, const ExceptionType& exception_type);

} // namespace modbus

#endif //MODBUS_EXCEPTIONTYPE_H

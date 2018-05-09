#ifndef MODBUS_WRITEMULTIPLEREGISTERSREQUEST_H
#define MODBUS_WRITEMULTIPLEREGISTERSREQUEST_H

/** @file
 * @brief Struct @ref modbus::WriteMultipleRegistersRequest
 */

#include <vector>
#include "modbus/messages/Address.h"

namespace modbus
{

/**
 * @brief Write multiple registers request message
 *
 * Write a block of contiguous registers.
 *
 * This is the function code @cpp 0x10 @ce of the Modbus protocol.
 *
 * @see @ref WriteMultipleRegistersResponse, @ref WriteSingleRegisterRequest
 */
struct WriteMultipleRegistersRequest
{
    /**
     * @brief Starting address to write to
     *
     * The starting address must be in a valid range of the device.
     */
    Address starting_address;

    /**
     * @brief Values to write to the registers
     *
     * Note that the Modbus protocol limits the number of registers you can write in a
     * single request to 123. Therefore, the vector should have a length of 1 to 123 inclusively.
     */
    std::vector<uint16_t> values;
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLEREGISTERSREQUEST_H

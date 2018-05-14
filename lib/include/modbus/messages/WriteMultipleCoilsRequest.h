#ifndef MODBUS_WRITEMULTIPLECOILSREQUEST_H
#define MODBUS_WRITEMULTIPLECOILSREQUEST_H

/** @file
 * @brief Struct @ref modbus::WriteMultipleCoilsRequest
 */

#include <vector>
#include "modbus/messages/Address.h"

namespace modbus
{

/**
 * @brief Write multiple coils request message
 *
 * Write a block of contiguous coils.
 *
 * This is the function code @cpp 0x0F @ce of the Modbus protocol.
 *
 * @see @ref WriteMultipleCoilsResponse, @ref WriteSingleCoilRequest
 */
struct WriteMultipleCoilsRequest
{
    /**
     * @brief Starting address to write to
     *
     * The starting address must be in a valid range of the device.
     */
    Address starting_address;

    /**
     * @brief Values to write to the coils
     *
     * Note that the Modbus protocol limits the number of registers you can write in a
     * single request to 2000. Therefore, the vector should have a length of 1 to 2000 inclusively.
     */
    std::vector<bool> values;
};

} // namespace modbus

#endif //MODBUS_WRITEMULTIPLECOILSREQUEST_H

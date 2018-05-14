#ifndef MODBUS_READCOILSRESPONSE_H
#define MODBUS_READCOILSRESPONSE_H

/** @file
 * @brief Struct @ref modbus::ReadCoilsResponse
 */

#include <vector>
#include "modbus/messages/SingleBitValue.h"

namespace modbus
{

/**
 * @brief Read Coils response message
 *
 * Response to @ref ReadCoilsRequest.
 *
 * This is the response for function code @cpp 0x01 @ce of the Modbus protocol.
 *
 * @see @ref ReadCoilsRequest
 */
struct ReadCoilsResponse
{
    /**
     * @brief Coil values read on the device
     *
     * Each value contains both the address and the value of the coil.
     */
    std::vector<SingleBitValue> values;
};

} // namespace modbus

#endif //MODBUS_READCOILSRESPONSE_H

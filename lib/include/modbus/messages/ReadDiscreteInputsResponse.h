#ifndef MODBUS_READDISCRETEINPUTSRESPONSE_H
#define MODBUS_READDISCRETEINPUTSRESPONSE_H

/** @file
 * @brief Struct @ref modbus::ReadCoilsResponse
 */

#include <vector>
#include "modbus/messages/SingleBitValue.h"

namespace modbus
{

/**
 * @brief Read Discrete Inputs response message
 *
 * Response to @ref ReadDiscreteInputsRequest.
 *
 * This is the response for function code @cpp 0x02 @ce of the Modbus protocol.
 *
 * @see @ref ReadDiscreteInputsRequest
 */
struct ReadDiscreteInputsResponse
{
    /**
     * @brief Discrete input values read on the device
     *
     * Each value contains both the address and the value of the coil.
     */
    std::vector<SingleBitValue> values;
};

} // namespace modbus

#endif //MODBUS_READDISCRETEINPUTSRESPONSE_H

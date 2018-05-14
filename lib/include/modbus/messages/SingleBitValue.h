#ifndef MODBUS_SINGLEBITVALUE_H
#define MODBUS_SINGLEBITVALUE_H

/** @file
 * @brief Struct @ref modbus::SingleBitValue
 */

#include "Address.h"

namespace modbus
{

/**
 * @brief Represent a coil or a discrete input address and its value
 *
 * @see @ref ReadCoilsResponse, @ref ReadDiscreteInputsResponse,
 *      @ref WriteSingleCoilRequest, @ref WriteSingleCoilResponse,
 */
struct SingleBitValue
{
    Address address;
    bool value;
};

} // modbus

#endif //MODBUS_SINGLEBITVALUE_H

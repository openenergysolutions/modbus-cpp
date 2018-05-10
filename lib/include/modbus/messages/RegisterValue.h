#ifndef MODBUS_REGISTERVALUE_H
#define MODBUS_REGISTERVALUE_H

/** @file
 * @brief Struct @ref modbus::RegisterValue
 */

#include "Address.h"

namespace modbus
{

/**
 * @brief Represent a register address and its value
 *
 * @see @ref ReadHoldingRegistersResponse, @ref ReadInputRegistersResponse,
 *      @ref WriteSingleRegisterRequest, @ref WriteSingleRegisterResponse
 */
struct RegisterValue
{
    Address address;
    uint16_t value;
};

} // modbus

#endif //MODBUS_REGISTERVALUE_H

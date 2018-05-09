#ifndef MODBUS_ADDRESS_H
#define MODBUS_ADDRESS_H

/** @file
 * @brief Typedef @ref modbus::Address
 */

#include <cstdint>

namespace modbus
{

/**
 * @brief Address of a register
 *
 * This is a 16-bit value.
 *
 * See section *4.4: MODBUS Addressing model* of the Modbus documentation for more details
 * about the addressing model of Modbus.
 *
 * @see @ref RegisterValue, @ref ReadHoldingRegistersRequest,
 *      @ref ReadInputRegistersRequest, @ref WriteMultipleRegistersRequest,
 *      @ref WriteMultipleRegistersResponse
 */
using Address = uint16_t;

} // namespace modbus

#endif //MODBUS_ADDRESS_H

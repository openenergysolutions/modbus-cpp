#ifndef MODBUS_TIMETYPEDEFS_H
#define MODBUS_TIMETYPEDEFS_H

/** @file
* @brief Typedef @ref modbus::action_t and @ref modbus::duration_t
*/

#include <chrono>
#include <functional>

namespace modbus
{

/**
 * @brief A copyable void function pointer taking no argument
 */
using action_t = std::function<void()>;

/**
 * @brief Represents relative time
 */
using duration_t = std::chrono::steady_clock::duration;

/**
 * @brief Represents an absolute timepoint based on steady_clock
 */
using steady_time_t = std::chrono::steady_clock::time_point;

} // namespace modbus

#endif //MODBUS_TIMETYPEDEFS_H

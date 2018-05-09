#include "modbus/channel/UnitIdentifier.h"

#include <ios>
#include <iomanip>

namespace modbus
{

std::ostream& operator<<(std::ostream& stream, const UnitIdentifier& unit_id)
{
    return stream << "0x"
                  << std::uppercase
                  << std::setw(2) << std::setfill('0')
                  << std::hex << (unsigned int)unit_id.get_value();
}

} // namespace modbus
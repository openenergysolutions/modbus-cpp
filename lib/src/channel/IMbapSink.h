#ifndef MODBUS_IMBAPSINK_H
#define MODBUS_IMBAPSINK_H

#include "channel/MbapMessage.h"

namespace modbus
{

class IMbapSink
{
public:
    virtual ~IMbapSink() = default;

    virtual void on_mbap_message(const MbapMessage& message) = 0;
};

} // namespace modbus

#endif //MODBUS_IMBAPSINK_H

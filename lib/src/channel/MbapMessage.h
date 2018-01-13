#ifndef MODBUS_MBAPMESSAGE_H
#define MODBUS_MBAPMESSAGE_H

#include "openpal/container/SequenceTypes.h"
#include "modbus/channel/UnitIdentifier.h"
#include "channel/TransactionIdentifier.h"

namespace modbus
{

struct MbapMessage
{
    MbapMessage(UnitIdentifier unit_id,
                TransactionIdentifier transaction_id,
                openpal::rseq_t data)
        : unit_id{unit_id},
          transaction_id{transaction_id},
          data{data} {}

    UnitIdentifier unit_id;
    TransactionIdentifier transaction_id;
    openpal::rseq_t data;
};

} // namespace modbus

#endif //MODBUS_MBAPMESSAGE_H

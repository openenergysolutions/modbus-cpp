#include "channel/MbapMessage.h"

#include "openpal/serialization/BigEndian.h"

namespace modbus
{

MbapMessage::MbapMessage(UnitIdentifier unit_id,
                         TransactionIdentifier transaction_id,
                         openpal::rseq_t data)
        : unit_id{unit_id},
          transaction_id{transaction_id},
          data{data}
{

}

openpal::rseq_t MbapMessage::build_message(UnitIdentifier unit_id,
                                           TransactionIdentifier transaction_id,
                                           const IRequest& request,
                                           openpal::wseq_t& buffer)
{
    auto length = request.get_request_length();
    if(length > 253)
    {
        length = 253;
    }

    auto result = buffer.readonly().take(length + 7);

    openpal::UInt16::write_to(buffer, transaction_id.get_value());
    openpal::UInt16::write_to(buffer, 0x0000);
    openpal::UInt16::write_to(buffer, (uint16_t)(length + 1));
    openpal::UInt8::write_to(buffer, unit_id.get_value());
    request.build_request(buffer);

    return result;
}

} // namespace modbus

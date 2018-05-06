#include "channel/MbapMessage.h"

#include "ser4cpp/serialization/BigEndian.h"

namespace modbus
{

MbapMessage::MbapMessage(UnitIdentifier unit_id,
                         TransactionIdentifier transaction_id,
                         ser4cpp::rseq_t data)
        : unit_id{unit_id},
          transaction_id{transaction_id},
          data{data}
{

}

ser4cpp::rseq_t MbapMessage::build_message(UnitIdentifier unit_id,
                                           TransactionIdentifier transaction_id,
                                           const IRequest& request,
                                           ser4cpp::wseq_t& buffer)
{
    auto length = request.get_request_length();
    if(length > 253)
    {
        length = 253;
    }

    auto result = buffer.readonly().take(length + 7);

    ser4cpp::UInt16::write_to(buffer, transaction_id.get_value());
    ser4cpp::UInt16::write_to(buffer, 0x0000);
    ser4cpp::UInt16::write_to(buffer, (uint16_t)(length + 1));
    ser4cpp::UInt8::write_to(buffer, unit_id.get_value());
    request.build_request(buffer);

    return result;
}

} // namespace modbus

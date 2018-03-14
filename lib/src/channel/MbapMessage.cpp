#include "channel/MbapMessage.h"

#include "loopser/serialization/BigEndian.h"

namespace modbus
{

MbapMessage::MbapMessage(UnitIdentifier unit_id,
                         TransactionIdentifier transaction_id,
                         loopser::rseq_t data)
        : unit_id{unit_id},
          transaction_id{transaction_id},
          data{data}
{

}

loopser::rseq_t MbapMessage::build_message(UnitIdentifier unit_id,
                                           TransactionIdentifier transaction_id,
                                           const IRequest& request,
                                           loopser::wseq_t& buffer)
{
    auto length = request.get_request_length();
    if(length > 253)
    {
        length = 253;
    }

    auto result = buffer.readonly().take(length + 7);

    loopser::UInt16::write_to(buffer, transaction_id.get_value());
    loopser::UInt16::write_to(buffer, 0x0000);
    loopser::UInt16::write_to(buffer, (uint16_t)(length + 1));
    loopser::UInt8::write_to(buffer, unit_id.get_value());
    request.build_request(buffer);

    return result;
}

} // namespace modbus

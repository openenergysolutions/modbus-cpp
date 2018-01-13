#include "MbapParser.h"

#include "openpal/serialization/BigEndian.h"
#include "IMbapSink.h"

namespace modbus
{

MbapParser::MbapParser(IMbapSink* sink)
    : m_sink{sink}
{
    reset();
}

void MbapParser::parse(openpal::rseq_t data)
{
    while(data.is_not_empty())
    {
        auto read = data.take(m_num_bytes_to_read);
        data.advance(read.length());
        m_num_bytes_to_read -= read.length();
        m_buffer_view.copy_from(read);

        if(m_num_bytes_to_read == 0)
        {
            if(!m_reading_body)
            {
                find_body_size();
            }
            else
            {
                send_data();
            }
        }
    }
}

void MbapParser::reset()
{
    m_buffer_view = m_buffer.as_wseq();
    m_num_bytes_to_read = MaxMbapHeaderSize;
    m_reading_body = false;
}

void MbapParser::find_body_size()
{
    auto view = m_buffer.as_seq().skip(4);

    uint16_t length;
    openpal::UInt16::read_from(view, length);

    if(length == 0)
    {
        reset();
        return;
    }

    m_num_bytes_to_read = length - 1;
    m_reading_body = true;
}

void MbapParser::send_data()
{
    auto view = m_buffer.as_seq();

    uint16_t transaction_id;
    openpal::UInt16::read_from(view, transaction_id);

    uint16_t protocol_id;
    openpal::UInt16::read_from(view, protocol_id);

    uint16_t length;
    openpal::UInt16::read_from(view, length);

    uint8_t unit_id;
    openpal::UInt8::read_from(view, unit_id);

    MbapMessage message
    {
            UnitIdentifier{unit_id},
            TransactionIdentifier{transaction_id},
            view.take(length - 1)
    };

    if(protocol_id == ModbusProtocolId && length > 0)
    {
        m_sink->on_mbap_message(message);
    }

    reset();
}

} // namespace modbus

/*
 * Copyright 2017-2018 Duke Energy Corporation and Open Energy Solutions, Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "MbapParser.h"

#include "ser4cpp/serialization/BigEndian.h"
#include "IMbapSink.h"

namespace modbus
{

MbapParser::MbapParser(IMbapSink* sink)
    : m_sink{sink}
{
    reset();
}

void MbapParser::parse(ser4cpp::rseq_t data)
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
    ser4cpp::UInt16::read_from(view, length);

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
    ser4cpp::UInt16::read_from(view, transaction_id);

    uint16_t protocol_id;
    ser4cpp::UInt16::read_from(view, protocol_id);

    uint16_t length;
    ser4cpp::UInt16::read_from(view, length);

    uint8_t unit_id;
    ser4cpp::UInt8::read_from(view, unit_id);

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

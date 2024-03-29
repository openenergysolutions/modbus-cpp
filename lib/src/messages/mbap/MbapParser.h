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
#ifndef MODBUS_MBAPPARSER_H
#define MODBUS_MBAPPARSER_H

#include "ser4cpp/container/SequenceTypes.h"
#include "ser4cpp/container/StaticBuffer.h"

namespace modbus
{

class IMbapSink;

class MbapParser
{
public:
    MbapParser(IMbapSink* sink);

    void parse(ser4cpp::rseq_t data);
    void reset();

private:
    void find_body_size();
    void send_data();

    static const std::size_t MaxMbapSize = 260;
    static const std::size_t MaxMbapHeaderSize = 7;
    static const std::size_t ModbusProtocolId = 0x0000;

    IMbapSink* m_sink;
    ser4cpp::StaticBuffer<MaxMbapSize> m_buffer;
    ser4cpp::wseq_t m_buffer_view;
    size_t m_num_bytes_to_read;
    bool m_reading_body;
};

} // namespace modbus

#endif //MODBUS_MBAPPARSER_H

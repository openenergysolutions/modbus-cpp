#ifndef MODBUS_MBAPPARSER_H
#define MODBUS_MBAPPARSER_H

#include "loopser/container/SequenceTypes.h"
#include "loopser/container/StaticBuffer.h"

namespace modbus
{

class IMbapSink;

class MbapParser
{
public:
    MbapParser(IMbapSink* sink);

    void parse(loopser::rseq_t data);
    void reset();

private:
    void find_body_size();
    void send_data();

    static const std::size_t MaxMbapSize = 260;
    static const std::size_t MaxMbapHeaderSize = 7;
    static const std::size_t ModbusProtocolId = 0x0000;

    IMbapSink* m_sink;
    loopser::StaticBuffer<uint32_t, MaxMbapSize> m_buffer;
    loopser::wseq_t m_buffer_view;
    size_t m_num_bytes_to_read;
    bool m_reading_body;
};

} // namespace modbus

#endif //MODBUS_MBAPPARSER_H

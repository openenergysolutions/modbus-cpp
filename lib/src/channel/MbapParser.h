#ifndef MODBUS_MBAPPARSER_H
#define MODBUS_MBAPPARSER_H

#include "openpal/container/SequenceTypes.h"
#include "openpal/container/StaticBuffer.h"

namespace modbus
{

class IMbapSink;

class MbapParser
{
public:
    MbapParser(IMbapSink* sink);

    void parse(openpal::rseq_t data);
    void reset();

private:
    void find_body_size();
    void send_data();

    static const std::size_t MaxMbapSize = 260;
    static const std::size_t MaxMbapHeaderSize = 7;
    static const std::size_t ModbusProtocolId = 0x0000;

    IMbapSink* m_sink;
    openpal::StaticBuffer<uint32_t, MaxMbapSize> m_buffer;
    openpal::wseq_t m_buffer_view;
    size_t m_num_bytes_to_read;
    bool m_reading_body;
};

} // namespace modbus

#endif //MODBUS_MBAPPARSER_H

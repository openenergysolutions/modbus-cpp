#ifndef MODBUS_LOGGINGLEVELCONVERSION_H
#define MODBUS_LOGGINGLEVELCONVERSION_H

#include "spdlog/spdlog.h"
#include "modbus/logging/LoggingLevel.h"

namespace modbus
{

inline spdlog::level::level_enum to_spdlog_level(LoggingLevel level)
{
    switch(level)
    {
        case LoggingLevel::Trace: return spdlog::level::trace;
        case LoggingLevel::Debug: return spdlog::level::debug;
        case LoggingLevel::Info: return spdlog::level::info;
        case LoggingLevel::Warn: return spdlog::level::warn;
        case LoggingLevel::Error: return spdlog::level::err;
        case LoggingLevel::Critical: return spdlog::level::critical;
        default: return spdlog::level::debug;
    }
}

} //namespace modbus

#endif //MODBUS_LOGGINGLEVELCONVERSION_H

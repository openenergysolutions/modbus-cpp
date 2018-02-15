#include "catch.hpp"

#include <sstream>
#include "modbus/messages/ExceptionType.h"

using namespace modbus;

TEST_CASE("ExceptionType")
{
    SECTION("parse_exception_type")
    {
        SECTION("IllegalFunction")
        {
            REQUIRE(parse_exception_type(0x01) == ExceptionType::IllegalFunction);
        }

        SECTION("IllegalDataAddress")
        {
            REQUIRE(parse_exception_type(0x02) == ExceptionType::IllegalDataAddress);
        }

        SECTION("IllegalDataValue")
        {
            REQUIRE(parse_exception_type(0x03) == ExceptionType::IllegalDataValue);
        }

        SECTION("ServerDeviceFailure")
        {
            REQUIRE(parse_exception_type(0x04) == ExceptionType::ServerDeviceFailure);
        }

        SECTION("Acknowledge")
        {
            REQUIRE(parse_exception_type(0x05) == ExceptionType::Acknowledge);
        }

        SECTION("ServerDeviceBusy")
        {
            REQUIRE(parse_exception_type(0x06) == ExceptionType::ServerDeviceBusy);
        }

        SECTION("MemoryParityError")
        {
            REQUIRE(parse_exception_type(0x08) == ExceptionType::MemoryParityError);
        }

        SECTION("GatewayPathUnavailable")
        {
            REQUIRE(parse_exception_type(0x0A) == ExceptionType::GatewayPathUnavailable);
        }

        SECTION("GatewayTargetDeviceFailedToRespond")
        {
            REQUIRE(parse_exception_type(0x0B) == ExceptionType::GatewayTargetDeviceFailedToRespond);
        }

        SECTION("Unknown")
        {
            REQUIRE(parse_exception_type(0xFF) == ExceptionType::Unknown);
        }
    }

    SECTION("ostream<<")
    {
        SECTION("IllegalFunction")
        {
            std::ostringstream oss;
            oss << ExceptionType::IllegalFunction;
            REQUIRE(oss.str() == "Illegal function");
        }

        SECTION("IllegalDataAddress")
        {
            std::ostringstream oss;
            oss << ExceptionType::IllegalDataAddress;
            REQUIRE(oss.str() == "Illegal data address");
        }

        SECTION("IllegalDataValue")
        {
            std::ostringstream oss;
            oss << ExceptionType::IllegalDataValue;
            REQUIRE(oss.str() == "Illegal data value");
        }

        SECTION("ServerDeviceFailure")
        {
            std::ostringstream oss;
            oss << ExceptionType::ServerDeviceFailure;
            REQUIRE(oss.str() == "Server device failure");
        }

        SECTION("Acknowledge")
        {
            std::ostringstream oss;
            oss << ExceptionType::Acknowledge;
            REQUIRE(oss.str() == "Acknowledge");
        }

        SECTION("ServerDeviceBusy")
        {
            std::ostringstream oss;
            oss << ExceptionType::ServerDeviceBusy;
            REQUIRE(oss.str() == "Server device busy");
        }

        SECTION("MemoryParityError")
        {
            std::ostringstream oss;
            oss << ExceptionType::MemoryParityError;
            REQUIRE(oss.str() == "Memory parity error");
        }

        SECTION("GatewayPathUnavailable")
        {
            std::ostringstream oss;
            oss << ExceptionType::GatewayPathUnavailable;
            REQUIRE(oss.str() == "Gateway path unavailable");
        }

        SECTION("GatewayTargetDeviceFailedToRespond")
        {
            std::ostringstream oss;
            oss << ExceptionType::GatewayTargetDeviceFailedToRespond;
            REQUIRE(oss.str() == "Gateway target device failed to respond");
        }

        SECTION("Unknown")
        {
            std::ostringstream oss;
            oss << ExceptionType::Unknown;
            REQUIRE(oss.str() == "Unknown");
        }
    }
}
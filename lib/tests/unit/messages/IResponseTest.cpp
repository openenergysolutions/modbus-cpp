#include "catch.hpp"

#include <array>
#include "modbus/exceptions/MalformedModbusResponseException.h"
#include "modbus/exceptions/ModbusException.h"
#include "mocks/IResponseMock.h"

using namespace modbus;

TEST_CASE("IResponse")
{
    uint8_t function_code = 0x03;

    SECTION("When corresponding function code, then return true")
    {
        std::array<uint8_t, 3> good_function_code{{
            0x03,       // Good function code
            0x42, 0x43 // Data
        }};
        ser4cpp::rseq_t buffer{good_function_code.data(), good_function_code.size()};

        auto result = IResponseMock::parse_function_code(function_code, buffer);

        REQUIRE(result.is_valid() == true);
        REQUIRE(buffer[0] == 0x42);
    }

    SECTION("When buffer is empty, then return malformed exception")
    {
        auto data = ser4cpp::rseq_t::empty();

        auto result = IResponseMock::parse_function_code(function_code, data);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When wrong function code, then return malformed exception")
    {
        std::array<uint8_t, 3> wrong_function_code{{
            0x01,       // Wrong function code
            0x00, 0x00 // Data
        }};
        ser4cpp::rseq_t buffer{wrong_function_code.data(), wrong_function_code.size()};

        auto result = IResponseMock::parse_function_code(function_code, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When wrong exception function code, then return malformed exception")
    {
        std::array<uint8_t, 3> wrong_exception_function_code{{
            0x81,      // Wrong exception function code
            0x00, 0x00 // Data
        }};
        ser4cpp::rseq_t buffer{wrong_exception_function_code.data(), wrong_exception_function_code.size()};

        auto result = IResponseMock::parse_function_code(function_code, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When exception function code with invalid length, then return malformed exception")
    {
        std::array<uint8_t, 3> exception_function_code_invalid_length{{
            0x83,      // Exception function code
            0x02, 0x03 // Data of invalid length
        }};
        ser4cpp::rseq_t buffer{exception_function_code_invalid_length.data(), exception_function_code_invalid_length.size()};

        auto result = IResponseMock::parse_function_code(function_code, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When exception function code without exception code, then return malformed exception")
    {
        std::array<uint8_t, 1> exception_function_without_code{{
            0x83,      // Exception function code
        }};
        ser4cpp::rseq_t buffer{exception_function_without_code.data(), exception_function_without_code.size()};

        auto result = IResponseMock::parse_function_code(function_code, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When exception function code, then return modbus exception")
    {
        std::array<uint8_t, 2> exception_function_code{{
            0x83,      // Exception function code
            0x02       // Illegal data address
        }};
        ser4cpp::rseq_t buffer{exception_function_code.data(), exception_function_code.size()};

        auto result = IResponseMock::parse_function_code(function_code, buffer);

        REQUIRE(result.has_exception<ModbusException>() == true);
        REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
    }
}

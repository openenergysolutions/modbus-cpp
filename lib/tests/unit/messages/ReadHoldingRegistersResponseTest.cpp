#include "catch.hpp"

#include <array>
#include "modbus/exceptions/MalformedModbusResponseException.h"
#include "modbus/exceptions/ModbusException.h"
#include "modbus/messages/ReadHoldingRegistersRequest.h"
#include "modbus/messages/ReadHoldingRegistersResponse.h"

using namespace modbus;

TEST_CASE("ReadHoldingRegistersResponse")
{
    const uint16_t starting_address = 0x0016;
    const uint16_t qty_of_registers = 4;
    ReadHoldingRegistersRequest request{starting_address, qty_of_registers};

    SECTION("When proper response, then parse it properly")
    {
        std::array<uint8_t, 10> proper_response{{
            0x03,       // Function code
            0x08,       // Number of bytes
            0x00, 0x01, // Register 0x0016
            0x00, 0x02, // Register 0x0017
            0x00, 0x03, // Register 0x0018
            0x00, 0x04  // Register 0x0019
        }};
        ser4cpp::rseq_t buffer{proper_response.data(), proper_response.size()};

        auto result = ReadHoldingRegistersResponse::parse(request, buffer);

        REQUIRE(result.is_valid() == true);
        auto response = result.get();
        auto values = response.get_values();
        REQUIRE(values.size() == 4);
        REQUIRE(values[0].address == 0x0016);
        REQUIRE(values[0].value == 0x0001);
        REQUIRE(values[1].address == 0x0017);
        REQUIRE(values[1].value == 0x0002);
        REQUIRE(values[2].address == 0x0018);
        REQUIRE(values[2].value == 0x0003);
        REQUIRE(values[3].address == 0x0019);
        REQUIRE(values[3].value == 0x0004);
    }

    SECTION("When exception response, then parse report exception")
    {
        std::array<uint8_t, 2> exception_response{{
            0x83, // Exception function code
            0x02  // Illegal data address
        }};
        ser4cpp::rseq_t buffer{exception_response.data(), exception_response.size()};

        auto result = ReadHoldingRegistersResponse::parse(request, buffer);

        REQUIRE(result.has_exception<ModbusException>() == true);
        REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
    }

    SECTION("When size not present, then return malformed exception")
    {
        std::array<uint8_t, 1> wrong_size_response{ {
            0x03 // Function code
        }};
        ser4cpp::rseq_t buffer{ wrong_size_response.data(), wrong_size_response.size() };

        auto result = ReadHoldingRegistersResponse::parse(request, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When size is not even, then return malformed exception")
    {
        std::array<uint8_t, 5> wrong_size_response{ {
            0x03,            // Function code
            0x03,            // Size not even
            0x42, 0x43, 0x44 // Appropriate data
        }};
        ser4cpp::rseq_t buffer{ wrong_size_response.data(), wrong_size_response.size() };

        auto result = ReadHoldingRegistersResponse::parse(request, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When wrong size response, then return malformed exception")
    {
        std::array<uint8_t, 3> wrong_size_response{{
            0x03, // Function code
            0x02, // Length should be 2
            0x42  // Only 1 byte is received
        }};
        ser4cpp::rseq_t buffer{wrong_size_response.data(), wrong_size_response.size()};

        auto result = ReadHoldingRegistersResponse::parse(request, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }
}

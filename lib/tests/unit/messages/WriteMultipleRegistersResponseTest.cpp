#include "catch.hpp"

#include <array>
#include "modbus/exceptions/MalformedModbusResponseException.h"
#include "modbus/exceptions/ModbusException.h"
#include "modbus/messages/WriteMultipleRegistersResponse.h"

using namespace modbus;

TEST_CASE("WriteMultpileRegistersResponse")
{
    const uint16_t starting_address = 0x1234;
    const uint16_t qty_of_registers = 3;
    WriteMultipleRegistersRequest request{starting_address};

    SECTION("When proper response, then parse it properly")
    {
        std::array<uint8_t, 5> proper_response{{
            0x10,       // Function code
            0x12, 0x34, // Starting address
            0x00, 0x03  // Qty of registers
        }};
        ser4cpp::rseq_t buffer{proper_response.data(), proper_response.size()};

        auto result = WriteMultipleRegistersResponse::parse(request, buffer);

        REQUIRE(result.is_valid() == true);
        auto response = result.get();
        REQUIRE(response.get_starting_address() == starting_address);
        REQUIRE(response.get_qty_of_registers() == qty_of_registers);
    }

    SECTION("When exception response, then parse report exception")
    {
        std::array<uint8_t, 2> exception_response{{
            0x90,       // Exception function code
            0x02        // Illegal data address
        }};
        ser4cpp::rseq_t buffer{exception_response.data(), exception_response.size()};

        auto result = WriteMultipleRegistersResponse::parse(request, buffer);

        REQUIRE(result.has_exception<ModbusException>() == true);
        REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
    }

    SECTION("When too small, then return malformed exception")
    {
        std::array<uint8_t, 1> too_small_response{{
            0x10 // Function code
        }};
        ser4cpp::rseq_t buffer{ too_small_response.data(), too_small_response.size() };

        auto result = WriteMultipleRegistersResponse::parse(request, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When too big, then return malformed exception")
    {
        std::array<uint8_t, 7> too_big_response{{
            0x10,       // Function code
            0x12, 0x34, // Starting address
            0x00, 0x03, // Qty of registers
            0x42, 0x42  // Junk
        }};
        ser4cpp::rseq_t buffer{ too_big_response.data(), too_big_response.size() };

        auto result = WriteMultipleRegistersResponse::parse(request, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }
}

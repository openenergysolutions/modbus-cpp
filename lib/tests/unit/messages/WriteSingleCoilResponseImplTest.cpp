#include "catch.hpp"

#include <array>
#include "modbus/exceptions/MalformedModbusResponseException.h"
#include "modbus/exceptions/ModbusException.h"
#include "messages/WriteSingleCoilResponseImpl.h"

using namespace modbus;

TEST_CASE("WriteSingleCoilResponse")
{
    const uint16_t address = 0x1234;
    WriteSingleCoilRequest request{{address, true}};
    WriteSingleCoilRequestImpl request_impl{request};

    SECTION("When proper ON response, then parse it properly")
    {
        std::array<uint8_t, 5> proper_response{{
            0x05,       // Function code
            0x12, 0x34, // Address
            0xFF, 0x00  // ON value
        }};
        ser4cpp::rseq_t buffer{proper_response.data(), proper_response.size()};

        auto result = WriteSingleCoilResponseImpl::parse(request_impl, buffer);

        REQUIRE(result.is_valid() == true);
        auto response = result.get();
        REQUIRE(response.value.address == address);
        REQUIRE(response.value.value == true);
    }

    SECTION("When proper OFF response, then parse it properly")
    {
        WriteSingleCoilRequest off_request{{address, false}};
        WriteSingleCoilRequestImpl off_request_impl{off_request};
        std::array<uint8_t, 5> proper_response{{
            0x05,       // Function code
            0x12, 0x34, // Address
            0x00, 0x00  // OFF value
        }};
        ser4cpp::rseq_t buffer{proper_response.data(), proper_response.size()};

        auto result = WriteSingleCoilResponseImpl::parse(off_request_impl, buffer);

        REQUIRE(result.is_valid() == true);
        auto response = result.get();
        REQUIRE(response.value.address == address);
        REQUIRE(response.value.value == false);
    }

    SECTION("When exception response, then parse report exception")
    {
        std::array<uint8_t, 2> exception_response{{
            0x85,       // Exception function code
            0x02        // Illegal data address
        }};
        ser4cpp::rseq_t buffer{exception_response.data(), exception_response.size()};

        auto result = WriteSingleCoilResponseImpl::parse(request_impl, buffer);

        REQUIRE(result.has_exception<ModbusException>() == true);
        REQUIRE(result.get_exception<ModbusException>().get_exception_type() == ExceptionType::IllegalDataAddress);
    }

    SECTION("When too small, then return malformed exception")
    {
        std::array<uint8_t, 1> too_small_response{ {
            0x05 // Function code
        }};
        ser4cpp::rseq_t buffer{ too_small_response.data(), too_small_response.size() };

        auto result = WriteSingleCoilResponseImpl::parse(request_impl, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When too big, then return malformed exception")
    {
        std::array<uint8_t, 7> too_big_response{{
            0x05,       // Function code
            0x12, 0x34, // Address
            0x67, 0x89, // Value
            0x42, 0x42  // Junk
        }};
        ser4cpp::rseq_t buffer{ too_big_response.data(), too_big_response.size() };

        auto result = WriteSingleCoilResponseImpl::parse(request_impl, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }

    SECTION("When wrong value, then return malformed exception")
    {
        std::array<uint8_t, 5> wrong_value_response{{
            0x05,       // Function code
            0x12, 0x34, // Address
            0x42, 0x42  // Wrong value
        }};
        ser4cpp::rseq_t buffer{wrong_value_response.data(), wrong_value_response.size()};

        auto result = WriteSingleCoilResponseImpl::parse(request_impl, buffer);

        REQUIRE(result.has_exception<MalformedModbusResponseException>() == true);
    }
}

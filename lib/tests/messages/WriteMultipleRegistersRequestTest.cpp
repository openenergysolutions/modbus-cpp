#include "catch.hpp"

#include <memory>
#include "openpal/container/Buffer.h"
#include "modbus/messages/WriteMultipleRegistersRequest.h"

using namespace modbus;

TEST_CASE("WriteMultipleRegistersRequest")
{
    const uint16_t starting_address = 0x1234;
    WriteMultipleRegistersRequest request{starting_address};

    SECTION("Normal request")
    {
        request.add_register(0x1111);
        request.add_register(0x1122);
        request.add_register(0x1133);

        SECTION("When get length, then return actual length.")
        {
            auto length = request.get_request_length();

            REQUIRE(length == 6 + 6);
        }

        SECTION("When build request, then write appropriate values to the buffer")
        {
            openpal::Buffer buffer{(uint32_t) request.get_request_length()};
            auto slice = buffer.as_wslice();

            request.build_request(slice);

            REQUIRE(buffer.as_wslice()[0] == 0x10);  // Function code
            REQUIRE(buffer.as_wslice()[1] == 0x12);  // Starting address MSB
            REQUIRE(buffer.as_wslice()[2] == 0x34);  // Starting address LSB
            REQUIRE(buffer.as_wslice()[3] == 0x00);  // Qty of registers value MSB
            REQUIRE(buffer.as_wslice()[4] == 0x03);  // Qty of registers value LSB
            REQUIRE(buffer.as_wslice()[5] == 6);     // Byte count
            REQUIRE(buffer.as_wslice()[6] == 0x11);  // First register MSB
            REQUIRE(buffer.as_wslice()[7] == 0x11);  // First register LSB
            REQUIRE(buffer.as_wslice()[8] == 0x11);  // Second register MSB
            REQUIRE(buffer.as_wslice()[9] == 0x22);  // Second register LSB
            REQUIRE(buffer.as_wslice()[10] == 0x11); // Third register MSB
            REQUIRE(buffer.as_wslice()[11] == 0x33); // Third register LSB
        }

        SECTION("When clone, then effectively creates a copy of the request")
        {
            auto copy = request.clone();
            auto other_request = static_cast<WriteMultipleRegistersRequest*>(copy.get());

            REQUIRE(other_request->get_starting_address() == starting_address);
            REQUIRE(other_request->get_request_length() == 6 + 6);
        }
    }

    SECTION("Empty request")
    {
        SECTION("When get length, then return actual length.")
        {
            auto length = request.get_request_length();

            REQUIRE(length == 6);
        }

        SECTION("When build request, then write appropriate values to the buffer")
        {
            openpal::Buffer buffer{(uint32_t) request.get_request_length()};
            auto slice = buffer.as_wslice();

            request.build_request(slice);

            REQUIRE(buffer.as_wslice()[0] == 0x10);  // Function code
            REQUIRE(buffer.as_wslice()[1] == 0x12);  // Starting address MSB
            REQUIRE(buffer.as_wslice()[2] == 0x34);  // Starting address LSB
            REQUIRE(buffer.as_wslice()[3] == 0x00);  // Qty of registers value MSB
            REQUIRE(buffer.as_wslice()[4] == 0x00);  // Qty of registers value LSB
            REQUIRE(buffer.as_wslice()[5] == 0);     // Byte count
        }
    }
}
#include "catch.hpp"

#include "loopser/container/Buffer.h"
#include "modbus/messages/ReadInputRegistersRequest.h"

using namespace modbus;

TEST_CASE("ReadInputRegistersRequest")
{
    const uint16_t starting_address = 0x1234;
    const uint16_t qty_of_registers = 4;
    ReadInputRegistersRequest request{starting_address, qty_of_registers};

    SECTION("When get length, then always return 5.")
    {
        auto length = request.get_request_length();

        REQUIRE(length == 5);
    }

    SECTION("When build request, then write appropriate values to the buffer")
    {
        loopser::Buffer buffer{(uint32_t)request.get_request_length()};
        auto slice = buffer.as_wslice();

        request.build_request(slice);

        REQUIRE(buffer.as_wslice()[0] == 0x04); // Function code
        REQUIRE(buffer.as_wslice()[1] == 0x12); // Starting address MSB
        REQUIRE(buffer.as_wslice()[2] == 0x34); // Starting address LSB
        REQUIRE(buffer.as_wslice()[3] == 0x00); // Qty of registers MSB
        REQUIRE(buffer.as_wslice()[4] == 0x04); // Qty of registers LSB
    }

    SECTION("When clone, then effectively creates a copy of the request")
    {
        auto copy = request.clone();
        auto other_request = static_cast<ReadInputRegistersRequest*>(copy.get());

        REQUIRE(other_request->get_starting_address() == starting_address);
        REQUIRE(other_request->get_qty_of_registers() == qty_of_registers);
    }
}
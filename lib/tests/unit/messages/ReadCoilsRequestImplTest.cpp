#include "catch.hpp"

#include "ser4cpp/container/Buffer.h"
#include "messages/ReadCoilsRequestImpl.h"

using namespace modbus;

TEST_CASE("ReadCoilsRequestImpl")
{
    const uint16_t starting_address = 0x1234;
    const uint16_t qty_of_bits = 4;
    ReadCoilsRequest request{starting_address, qty_of_bits};
    ReadCoilsRequestImpl request_impl(request);

    SECTION("When get length, then always return 5.")
    {
        auto length = request_impl.get_request_length();

        REQUIRE(length == 5);
    }

    SECTION("When build request, then write appropriate values to the buffer")
    {
        ser4cpp::Buffer buffer{(uint32_t)request_impl.get_request_length()};
        auto slice = buffer.as_wslice();

        request_impl.build_request(slice);

        REQUIRE(buffer.as_wslice()[0] == 0x01); // Function code
        REQUIRE(buffer.as_wslice()[1] == 0x12); // Starting address MSB
        REQUIRE(buffer.as_wslice()[2] == 0x34); // Starting address LSB
        REQUIRE(buffer.as_wslice()[3] == 0x00); // Qty of registers MSB
        REQUIRE(buffer.as_wslice()[4] == 0x04); // Qty of registers LSB
    }

    SECTION("When clone, then effectively creates a copy of the request")
    {
        auto copy = request_impl.clone();
        auto other_request = static_cast<ReadCoilsRequestImpl*>(copy.get());

        REQUIRE(other_request->get_request().starting_address == starting_address);
        REQUIRE(other_request->get_request().qty_of_bits == qty_of_bits);
        REQUIRE(&other_request->get_request() != &request);
    }
}

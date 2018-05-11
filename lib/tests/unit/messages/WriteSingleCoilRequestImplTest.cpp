#include "catch.hpp"

#include "ser4cpp/container/Buffer.h"
#include "messages/WriteSingleCoilRequestImpl.h"

using namespace modbus;

TEST_CASE("WriteSingleCoilRequestImpl")
{
    const uint16_t address = 0x1234;
    WriteSingleCoilRequest request{{address, true}};
    WriteSingleCoilRequestImpl request_impl{request};

    SECTION("When get length, then always return 5.")
    {
        auto length = request_impl.get_request_length();

        REQUIRE(length == 5);
    }

    SECTION("When build ON request, then write appropriate values to the buffer")
    {
        ser4cpp::Buffer buffer{(uint32_t)request_impl.get_request_length()};
        auto slice = buffer.as_wslice();

        request_impl.build_request(slice);

        REQUIRE(buffer.as_wslice()[0] == 0x05); // Function code
        REQUIRE(buffer.as_wslice()[1] == 0x12); // Address MSB
        REQUIRE(buffer.as_wslice()[2] == 0x34); // Address LSB
        REQUIRE(buffer.as_wslice()[3] == 0xFF); // ON value MSB
        REQUIRE(buffer.as_wslice()[4] == 0x00); // ON value LSB
    }

    SECTION("When build OFF request, then write appropriate values to the buffer")
    {
        WriteSingleCoilRequest off_request{{address, false}};
        WriteSingleCoilRequestImpl off_request_impl{off_request};

        ser4cpp::Buffer buffer{(uint32_t)off_request_impl.get_request_length()};
        auto slice = buffer.as_wslice();

        off_request_impl.build_request(slice);

        REQUIRE(buffer.as_wslice()[0] == 0x05); // Function code
        REQUIRE(buffer.as_wslice()[1] == 0x12); // Address MSB
        REQUIRE(buffer.as_wslice()[2] == 0x34); // Address LSB
        REQUIRE(buffer.as_wslice()[3] == 0x00); // OFF value MSB
        REQUIRE(buffer.as_wslice()[4] == 0x00); // OFF value LSB
    }

    SECTION("When clone, then effectively creates a copy of the request")
    {
        auto copy = request_impl.clone();
        auto other_request = static_cast<WriteSingleCoilRequestImpl*>(copy.get());

        REQUIRE(other_request->get_request().value.address == address);
        REQUIRE(other_request->get_request().value.value == true);
        REQUIRE(&other_request->get_request() != &request);
    }
}

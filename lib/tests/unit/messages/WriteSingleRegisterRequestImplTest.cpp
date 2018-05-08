#include "catch.hpp"

#include <memory>
#include "ser4cpp/container/Buffer.h"
#include "messages/WriteSingleRegisterRequestImpl.h"

using namespace modbus;

TEST_CASE("WriteSingleRegisterRequestImpl")
{
    const uint16_t address = 0x1234;
    const uint16_t value = 0x6789;
    WriteSingleRegisterRequest request{{address, value}};
    WriteSingleRegisterRequestImpl request_impl{request};

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

        REQUIRE(buffer.as_wslice()[0] == 0x06); // Function code
        REQUIRE(buffer.as_wslice()[1] == 0x12); // Address MSB
        REQUIRE(buffer.as_wslice()[2] == 0x34); // Address LSB
        REQUIRE(buffer.as_wslice()[3] == 0x67); // Register value MSB
        REQUIRE(buffer.as_wslice()[4] == 0x89); // Register value LSB
    }

    SECTION("When clone, then effectively creates a copy of the request")
    {
        auto copy = request_impl.clone();
        auto other_request = static_cast<WriteSingleRegisterRequestImpl*>(copy.get());

        REQUIRE(other_request->get_request().value.address == address);
        REQUIRE(other_request->get_request().value.value == value);
        REQUIRE(&other_request->get_request() != &request);
    }
}

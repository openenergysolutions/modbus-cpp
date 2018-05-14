#include "catch.hpp"

#include <memory>
#include "ser4cpp/container/Buffer.h"
#include "messages/WriteMultipleCoilsRequestImpl.h"

using namespace modbus;

TEST_CASE("WriteMultipleCoilsRequestImpl")
{
    const uint16_t starting_address = 0x1234;
    WriteMultipleCoilsRequest request{starting_address, {false, true, false, true,
                                                         false, false, true, true}};

    SECTION("Normal request")
    {
        WriteMultipleCoilsRequestImpl request_impl{request};

        SECTION("When get length, then return actual length.")
        {
            auto length = request_impl.get_request_length();

            REQUIRE(length == 6 + 1);
        }

        SECTION("When build request, then write appropriate values to the buffer")
        {
            ser4cpp::Buffer buffer{(uint32_t) request_impl.get_request_length()};
            auto slice = buffer.as_wslice();

            request_impl.build_request(slice);

            REQUIRE(buffer.as_wslice()[0] == 0x0F);  // Function code
            REQUIRE(buffer.as_wslice()[1] == 0x12);  // Starting address MSB
            REQUIRE(buffer.as_wslice()[2] == 0x34);  // Starting address LSB
            REQUIRE(buffer.as_wslice()[3] == 0x00);  // Qty of coils value MSB
            REQUIRE(buffer.as_wslice()[4] == 0x08);  // Qty of coils value LSB
            REQUIRE(buffer.as_wslice()[5] == 0x01);     // Byte count
            REQUIRE(buffer.as_wslice()[6] == 0b11001010);  // 8 coils
        }

        SECTION("When clone, then effectively creates a copy of the request")
        {
            auto copy = request_impl.clone();
            auto other_request = static_cast<WriteMultipleCoilsRequestImpl*>(copy.get());

            REQUIRE(other_request->get_request().starting_address == starting_address);
            REQUIRE(other_request->get_request().values.size() == 8);
            REQUIRE(&other_request->get_request() != &request);
        }
    }

    SECTION("Request with number of coils not a multiple of 8")
    {
        request.values.emplace_back(true);
        request.values.emplace_back(false);
        request.values.emplace_back(true);
        WriteMultipleCoilsRequestImpl request_impl{request};

        SECTION("When get length, then return actual length.")
        {
            auto length = request_impl.get_request_length();

            REQUIRE(length == 6 + 2);
        }

        SECTION("When build request, then write appropriate values to the buffer")
        {
            ser4cpp::Buffer buffer{(uint32_t) request_impl.get_request_length()};
            auto slice = buffer.as_wslice();

            request_impl.build_request(slice);

            REQUIRE(buffer.as_wslice()[0] == 0x0F);  // Function code
            REQUIRE(buffer.as_wslice()[1] == 0x12);  // Starting address MSB
            REQUIRE(buffer.as_wslice()[2] == 0x34);  // Starting address LSB
            REQUIRE(buffer.as_wslice()[3] == 0x00);  // Qty of coils value MSB
            REQUIRE(buffer.as_wslice()[4] == 0x0B);  // Qty of coils value LSB
            REQUIRE(buffer.as_wslice()[5] == 0x02);     // Byte count
            REQUIRE(buffer.as_wslice()[6] == 0b11001010);  // First 8 coils
            REQUIRE(buffer.as_wslice()[7] == 0b00000101);  // Last 3 coils
        }
    }

    SECTION("Empty request")
    {
        WriteMultipleCoilsRequest empty_request{starting_address, {}};
        WriteMultipleCoilsRequestImpl request_impl{empty_request};

        SECTION("When get length, then return actual length.")
        {
            auto length = request_impl.get_request_length();

            REQUIRE(length == 6);
        }

        SECTION("When build request, then write appropriate values to the buffer")
        {
            ser4cpp::Buffer buffer{(uint32_t) request_impl.get_request_length()};
            auto slice = buffer.as_wslice();

            request_impl.build_request(slice);

            REQUIRE(buffer.as_wslice()[0] == 0x0F);  // Function code
            REQUIRE(buffer.as_wslice()[1] == 0x12);  // Starting address MSB
            REQUIRE(buffer.as_wslice()[2] == 0x34);  // Starting address LSB
            REQUIRE(buffer.as_wslice()[3] == 0x00);  // Qty of coils value MSB
            REQUIRE(buffer.as_wslice()[4] == 0x00);  // Qty of coils value LSB
            REQUIRE(buffer.as_wslice()[5] == 0);     // Byte count
        }
    }
}

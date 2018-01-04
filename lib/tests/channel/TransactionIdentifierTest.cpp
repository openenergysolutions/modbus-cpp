#include "catch.hpp"

#include "channel/TransactionIdentifier.h"

using namespace modbus;

TEST_CASE("TransactionIdentifier")
{
    SECTION("Constructor initialize with the right value")
    {
        TransactionIdentifier transaction_identifier{0x1234};

        REQUIRE(transaction_identifier.get_value() == 0x1234);
    }

    SECTION("Equality operator checks if value is equal")
    {
        TransactionIdentifier first_transaction_identifier{0x1234};
        TransactionIdentifier second_transaction_identifier{0x4321};

        REQUIRE(first_transaction_identifier == first_transaction_identifier);
        REQUIRE(first_transaction_identifier != second_transaction_identifier);
    }
}

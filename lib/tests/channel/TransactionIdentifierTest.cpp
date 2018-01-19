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

    SECTION("Printing writes proper hexadecimal format")
    {
        SECTION("Leading zeros are added")
        {
            TransactionIdentifier transaction_id_zero{0x00};
            std::ostringstream oss;
            oss << transaction_id_zero;

            REQUIRE(oss.str() == "0x0000");
        }


        SECTION("Letters are capitalized")
        {
            TransactionIdentifier transaction_id_four_zero_zero_c{0x400C};
            std::ostringstream oss;
            oss << transaction_id_four_zero_zero_c;

            REQUIRE(oss.str() == "0x400C");
        }
    }
}

#define CATCH_CONFIG_MAIN 
#include "catch.hpp"

TEST_CASE("Simple addition test", "[math]") {
    REQUIRE(1 + 1 == 2);  // Passes
    REQUIRE(2 * 2 == 5);  // Fails
}

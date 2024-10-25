#include <doctest/doctest.h>
#include <ssip/ssip.h>
#include <stdexcept>

using namespace ssip;


TEST_CASE("given new RB, put to BR and get all elements, expect empty") {
    RingBuffer rb(5);

    CHECK(rb.size() == 0);
    CHECK(rb.is_empty());
    CHECK(!rb.is_full());
    rb.put(1);
    rb.put(2);
    rb.put(3);
    CHECK(rb.size() == 3);
    CHECK(!rb.is_empty());
    rb.put(4);
    rb.put(5);
    CHECK(rb.size() == 5);
    CHECK(rb.is_full());
    rb.get();
    rb.get();
    rb.get();
    CHECK(rb.size() == 2);
    rb.get();
    CHECK(rb.get() == 5);
    CHECK(rb.is_empty());
}

TEST_CASE("verify exception when get data from empty RB") {
    // Given
    RingBuffer rb(5);
    // When
    bool bflag;
    try {
        rb.get();
    } catch (std::out_of_range& e) {
        bflag = true;
    }
    // Then
    REQUIRE(rb.size() == 0);
    CHECK(bflag);
}

TEST_CASE("verify exception when put data to full RB") {
    // Given
    RingBuffer rb(5);
    rb.put(1);
    rb.put(2);
    rb.put(3);
    rb.put(4);
    rb.put(5);
    // When
    bool bflag;
    try {
        rb.put(6);
    } catch (std::overflow_error& e) {
        bflag = true;
    }
    // Then
    REQUIRE(rb.size() == 5);
    CHECK(bflag);
}
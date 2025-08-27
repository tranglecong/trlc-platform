/**
 * @file test_endianness.cpp
 * @brief Comprehensive tests for byte order detection and utilities
 *
 * Tests all endianness detection functionality including compile-time detection,
 * byte swapping, network byte order conversion, and utility functions.
 */

#include <cassert>
#include <cstdint>
#include <iostream>

#include "trlc/platform/endianness.hpp"

namespace trlc::platform::test {

void testByteOrderDetection() {
    std::cout << "Testing byte order detection..." << std::endl;

    // Test basic byte order detection
    ByteOrder detected_order = getByteOrder();

    std::cout << "  - Detected byte order: " << static_cast<int>(detected_order) << std::endl;

    // Byte order should be one of the known types (not unknown on modern systems)
    assert(detected_order == ByteOrder::little_endian || detected_order == ByteOrder::big_endian ||
           detected_order == ByteOrder::mixed_endian);

    // Test compile-time detection
    constexpr ByteOrder compile_time_order = getByteOrder();
    static_cast<void>(compile_time_order);  // Use to prevent optimization

    // Test convenience functions
    bool is_little = isLittleEndian();
    bool is_big = isBigEndian();

    std::cout << "  - Is little endian: " << (is_little ? "yes" : "no") << std::endl;
    std::cout << "  - Is big endian: " << (is_big ? "yes" : "no") << std::endl;

    // Exactly one should be true for most systems
    if (detected_order != ByteOrder::mixed_endian) {
        assert(is_little != is_big);  // One should be true, one false
    }

    // Test that function results match detected order
    if (detected_order == ByteOrder::little_endian) {
        assert(is_little && !is_big);
    } else if (detected_order == ByteOrder::big_endian) {
        assert(!is_little && is_big);
    }

    std::cout << "  ✓ Byte order detection works correctly" << std::endl;
}

void testEndiannessInfo() {
    std::cout << "Testing EndiannessInfo structure..." << std::endl;

    EndiannessInfo info = getEndiannessInfo();

    std::cout << "  - Byte order: " << static_cast<int>(info.byte_order) << std::endl;
    std::cout << "  - Is little endian: " << (info.is_little_endian ? "yes" : "no") << std::endl;
    std::cout << "  - Is big endian: " << (info.is_big_endian ? "yes" : "no") << std::endl;

    // Test consistency with direct functions
    assert(info.byte_order == getByteOrder());
    assert(info.is_little_endian == isLittleEndian());
    assert(info.is_big_endian == isBigEndian());

    // Test member functions
    assert(info.isNativeOrder(info.byte_order));
    assert(!info.isNativeOrder(getOppositeByteOrder(info.byte_order)));

    // Test byte swap detection
    if (info.byte_order != ByteOrder::unknown) {
        assert(!info.needsByteSwap(info.byte_order));

        ByteOrder opposite = getOppositeByteOrder(info.byte_order);
        if (opposite != ByteOrder::unknown) {
            assert(info.needsByteSwap(opposite));
        }
    }

    std::cout << "  ✓ EndiannessInfo structure works correctly" << std::endl;
}

void testByteSwapping() {
    std::cout << "Testing byte swapping functions..." << std::endl;

    // Test 16-bit byte swapping
    uint16_t test16 = 0x1234;
    uint16_t swapped16 = byteSwap16(test16);
    uint16_t expected16 = 0x3412;

    assert(swapped16 == expected16);
    assert(byteSwap16(swapped16) == test16);  // Double swap should restore

    std::cout << "  - 16-bit: 0x" << std::hex << test16 << " -> 0x" << swapped16 << std::dec
              << std::endl;

    // Test 32-bit byte swapping
    uint32_t test32 = 0x12345678;
    uint32_t swapped32 = byteSwap32(test32);
    uint32_t expected32 = 0x78563412;

    assert(swapped32 == expected32);
    assert(byteSwap32(swapped32) == test32);  // Double swap should restore

    std::cout << "  - 32-bit: 0x" << std::hex << test32 << " -> 0x" << swapped32 << std::dec
              << std::endl;

    // Test 64-bit byte swapping
    uint64_t test64 = 0x123456789ABCDEF0ULL;
    uint64_t swapped64 = byteSwap64(test64);
    uint64_t expected64 = 0xF0DEBC9A78563412ULL;

    assert(swapped64 == expected64);
    assert(byteSwap64(swapped64) == test64);  // Double swap should restore

    std::cout << "  - 64-bit: 0x" << std::hex << test64 << " -> 0x" << swapped64 << std::dec
              << std::endl;

    std::cout << "  ✓ Byte swapping functions work correctly" << std::endl;
}

void testGenericByteSwap() {
    std::cout << "Testing generic byte swap template..." << std::endl;

    // Test 8-bit (should be no-op)
    uint8_t test8 = 0xAB;
    uint8_t swapped8 = byteSwap(test8);
    assert(swapped8 == test8);

    // Test signed 8-bit
    int8_t test_int8 = -5;
    int8_t swapped_int8 = byteSwap(test_int8);
    assert(swapped_int8 == test_int8);

    // Test 16-bit with template
    uint16_t test16 = 0x1234;
    uint16_t swapped16 = byteSwap(test16);
    assert(swapped16 == 0x3412);

    // Test signed 16-bit
    int16_t test_int16 = 0x1234;
    int16_t swapped_int16 = byteSwap(test_int16);
    assert(swapped_int16 == static_cast<int16_t>(0x3412));

    // Test 32-bit with template
    uint32_t test32 = 0x12345678;
    uint32_t swapped32 = byteSwap(test32);
    assert(swapped32 == 0x78563412);

    // Test signed 32-bit
    int32_t test_int32 = 0x12345678;
    int32_t swapped_int32 = byteSwap(test_int32);
    assert(swapped_int32 == static_cast<int32_t>(0x78563412));

    // Test 64-bit with template
    uint64_t test64 = 0x123456789ABCDEF0ULL;
    uint64_t swapped64 = byteSwap(test64);
    assert(swapped64 == 0xF0DEBC9A78563412ULL);

    // Test signed 64-bit
    int64_t test_int64 = 0x123456789ABCDEF0LL;
    int64_t swapped_int64 = byteSwap(test_int64);
    assert(swapped_int64 == static_cast<int64_t>(0xF0DEBC9A78563412ULL));

    std::cout << "  ✓ Generic byte swap template works correctly" << std::endl;
}

void testNetworkByteOrder() {
    std::cout << "Testing network byte order conversion..." << std::endl;

    // Test 16-bit conversions
    uint16_t host16 = 0x1234;
    uint16_t net16 = hostToNetwork(host16);
    uint16_t back16 = networkToHost(net16);

    assert(back16 == host16);  // Round trip should restore original

    // On little-endian systems, network order should be swapped
    if (isLittleEndian()) {
        assert(net16 == byteSwap(host16));
    } else {
        assert(net16 == host16);
    }

    std::cout << "  - 16-bit host: 0x" << std::hex << host16 << " -> network: 0x" << net16
              << std::dec << std::endl;

    // Test 32-bit conversions
    uint32_t host32 = 0x12345678;
    uint32_t net32 = hostToNetwork(host32);
    uint32_t back32 = networkToHost(net32);

    assert(back32 == host32);  // Round trip should restore original

    if (isLittleEndian()) {
        assert(net32 == byteSwap(host32));
    } else {
        assert(net32 == host32);
    }

    std::cout << "  - 32-bit host: 0x" << std::hex << host32 << " -> network: 0x" << net32
              << std::dec << std::endl;

    // Test specialized functions
    uint16_t htons_result = hostToNetworkShort(host16);
    uint32_t htonl_result = hostToNetworkLong(host32);
    uint16_t ntohs_result = networkToHostShort(net16);
    uint32_t ntohl_result = networkToHostLong(net32);

    assert(htons_result == net16);
    assert(htonl_result == net32);
    assert(ntohs_result == host16);
    assert(ntohl_result == host32);

    std::cout << "  ✓ Network byte order conversion works correctly" << std::endl;
}

void testUtilityFunctions() {
    std::cout << "Testing utility functions..." << std::endl;

    // Test byte order compatibility
    assert(areByteOrdersCompatible(ByteOrder::little_endian, ByteOrder::little_endian));
    assert(areByteOrdersCompatible(ByteOrder::big_endian, ByteOrder::big_endian));
    assert(!areByteOrdersCompatible(ByteOrder::little_endian, ByteOrder::big_endian));
    assert(!areByteOrdersCompatible(ByteOrder::big_endian, ByteOrder::little_endian));

    // Unknown orders should be considered compatible (conservative)
    assert(areByteOrdersCompatible(ByteOrder::unknown, ByteOrder::little_endian));
    assert(areByteOrdersCompatible(ByteOrder::big_endian, ByteOrder::unknown));
    assert(areByteOrdersCompatible(ByteOrder::unknown, ByteOrder::unknown));

    // Test opposite byte order
    assert(getOppositeByteOrder(ByteOrder::little_endian) == ByteOrder::big_endian);
    assert(getOppositeByteOrder(ByteOrder::big_endian) == ByteOrder::little_endian);
    assert(getOppositeByteOrder(ByteOrder::unknown) == ByteOrder::unknown);
    assert(getOppositeByteOrder(ByteOrder::mixed_endian) == ByteOrder::unknown);

    // Test byte order conversion
    uint32_t test_value = 0x12345678;

    // Same order should not change value
    uint32_t same_order =
        convertByteOrder(test_value, ByteOrder::little_endian, ByteOrder::little_endian);
    assert(same_order == test_value);

    // Different order should swap bytes
    uint32_t diff_order =
        convertByteOrder(test_value, ByteOrder::little_endian, ByteOrder::big_endian);
    assert(diff_order == byteSwap(test_value));

    // Unknown order should not change value (conservative)
    uint32_t unknown_order =
        convertByteOrder(test_value, ByteOrder::unknown, ByteOrder::little_endian);
    assert(unknown_order == test_value);

    std::cout << "  ✓ Utility functions work correctly" << std::endl;
}

void testMacros() {
    std::cout << "Testing endianness macros..." << std::endl;

    // Test endianness detection macros
    bool little_macro = TRLC_ENDIAN_LITTLE;
    bool big_macro = TRLC_ENDIAN_BIG;

    assert(little_macro == isLittleEndian());
    assert(big_macro == isBigEndian());

    std::cout << "  - TRLC_ENDIAN_LITTLE: " << (little_macro ? "true" : "false") << std::endl;
    std::cout << "  - TRLC_ENDIAN_BIG: " << (big_macro ? "true" : "false") << std::endl;

    // Test byte swap macro
    uint32_t test_value = 0x12345678;
    uint32_t swapped_macro = TRLC_BYTE_SWAP(test_value);
    assert(swapped_macro == byteSwap(test_value));

    // Test network conversion macros
    uint16_t host16 = 0x1234;
    uint32_t host32 = 0x12345678;

    uint16_t net16_macro = TRLC_HTONS(host16);
    uint32_t net32_macro = TRLC_HTONL(host32);
    uint16_t back16_macro = TRLC_NTOHS(net16_macro);
    uint32_t back32_macro = TRLC_NTOHL(net32_macro);

    assert(net16_macro == hostToNetworkShort(host16));
    assert(net32_macro == hostToNetworkLong(host32));
    assert(back16_macro == host16);
    assert(back32_macro == host32);

    // Test generic network conversion macros
    uint16_t net16_generic = TRLC_HTON(host16);
    uint32_t net32_generic = TRLC_HTON(host32);
    uint16_t back16_generic = TRLC_NTOH(net16_generic);
    uint32_t back32_generic = TRLC_NTOH(net32_generic);

    assert(net16_generic == hostToNetwork(host16));
    assert(net32_generic == hostToNetwork(host32));
    assert(back16_generic == host16);
    assert(back32_generic == host32);

    std::cout << "  ✓ Endianness macros work correctly" << std::endl;
}

void testCompileTimeExecution() {
    std::cout << "Testing compile-time execution..." << std::endl;

    // Test that functions are constexpr
    static_assert(getByteOrder() != ByteOrder::unknown || true, "getByteOrder should be constexpr");
    static_assert(isLittleEndian() || isBigEndian() || true,
                  "endianness functions should be constexpr");

    // Test compile-time byte swapping
    constexpr uint16_t test16 = 0x1234;
    constexpr uint16_t swapped16 = byteSwap16(test16);
    static_assert(swapped16 == 0x3412, "byteSwap16 should work at compile time");

    constexpr uint32_t test32 = 0x12345678;
    constexpr uint32_t swapped32 = byteSwap32(test32);
    static_assert(swapped32 == 0x78563412, "byteSwap32 should work at compile time");

    constexpr uint64_t test64 = 0x123456789ABCDEF0ULL;
    constexpr uint64_t swapped64 = byteSwap64(test64);
    static_assert(swapped64 == 0xF0DEBC9A78563412ULL, "byteSwap64 should work at compile time");

    // Test compile-time generic byte swap
    constexpr uint16_t generic_swap16 = byteSwap(test16);
    static_assert(generic_swap16 == swapped16, "generic byteSwap should work at compile time");

    // Test compile-time network conversion
    constexpr uint16_t net16 = hostToNetwork(test16);
    constexpr uint16_t back16 = networkToHost(net16);
    static_assert(back16 == test16, "network conversion should work at compile time");

    // Test compile-time endianness info (may not be fully constexpr due to runtime fallback)
    auto info = getEndiannessInfo();
    static_cast<void>(info);  // Use to prevent optimization

    std::cout << "  ✓ All functions execute at compile time" << std::endl;
}

void testEdgeCases() {
    std::cout << "Testing edge cases..." << std::endl;

    // Test single byte values (should not change)
    uint8_t byte_val = 0xAB;
    assert(byteSwap(byte_val) == byte_val);

    // Test zero values
    assert(byteSwap16(0) == 0);
    assert(byteSwap32(0) == 0);
    assert(byteSwap64(0) == 0);

    // Test maximum values
    assert(byteSwap16(0xFFFF) == 0xFFFF);
    assert(byteSwap32(0xFFFFFFFF) == 0xFFFFFFFF);
    assert(byteSwap64(0xFFFFFFFFFFFFFFFFULL) == 0xFFFFFFFFFFFFFFFFULL);

    // Test alternating patterns
    uint16_t pattern16 = 0xAAAA;
    assert(byteSwap16(pattern16) == 0xAAAA);

    uint32_t pattern32 = 0xAAAAAAAA;
    assert(byteSwap32(pattern32) == 0xAAAAAAAA);

    // Test mixed endian handling (should be treated as incompatible)
    assert(!areByteOrdersCompatible(ByteOrder::mixed_endian, ByteOrder::little_endian));
    assert(getOppositeByteOrder(ByteOrder::mixed_endian) == ByteOrder::unknown);

    std::cout << "  ✓ Edge cases handled correctly" << std::endl;
}

void testPerformance() {
    std::cout << "Testing performance characteristics..." << std::endl;

    // All detection should be compile-time where possible
    constexpr bool little = isLittleEndian();
    constexpr bool big = isBigEndian();
    constexpr ByteOrder order = getByteOrder();
    auto info = getEndiannessInfo();  // May not be fully constexpr

    // Test that byte swapping is efficient
    uint32_t test_values[] = {0x12345678, 0x87654321, 0xAAAABBBB, 0x11223344};

    for (uint32_t value : test_values) {
        uint32_t swapped = byteSwap(value);
        uint32_t double_swapped = byteSwap(swapped);
        assert(double_swapped == value);

        // Test network conversion round trip
        uint32_t to_net = hostToNetwork(value);
        uint32_t from_net = networkToHost(to_net);
        assert(from_net == value);
    }

    // Use compile-time constants to prevent optimization
    static_cast<void>(little);
    static_cast<void>(big);
    static_cast<void>(order);
    static_cast<void>(info);

    std::cout << "  ✓ Performance characteristics verified" << std::endl;
}

void testHeaderInclusion() {
    std::cout << "Testing header inclusion..." << std::endl;

// Test that header inclusion guard is defined
#ifdef TRLC_ENDIANNESS_INCLUDED
    std::cout << "  - TRLC_ENDIANNESS_INCLUDED is defined" << std::endl;
#else
    assert(false && "TRLC_ENDIANNESS_INCLUDED should be defined");
#endif

    std::cout << "  ✓ Header inclusion works correctly" << std::endl;
}

}  // namespace trlc::platform::test

int main() {
    using namespace trlc::platform::test;

    std::cout << "=== TRLC Platform Endianness Tests ===" << std::endl;

    try {
        testByteOrderDetection();
        testEndiannessInfo();
        testByteSwapping();
        testGenericByteSwap();
        testNetworkByteOrder();
        testUtilityFunctions();
        testMacros();
        testCompileTimeExecution();
        testEdgeCases();
        testPerformance();
        testHeaderInclusion();

        std::cout << "\n✅ All endianness tests passed!" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}

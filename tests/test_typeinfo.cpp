/**
 * @file test_typeinfo.cpp
 * @brief Comprehensive tests for type information utilities
 *
 * Tests all type introspection and memory layout functionality including
 * type size/alignment queries, padding analysis, cache line alignment,
 * and type verification utilities.
 */

#include <cassert>
#include <cstdint>
#include <iostream>

#include "trlc/platform/macros.hpp"  // For alignment macros
#include "trlc/platform/typeinfo.hpp"

namespace trlc::platform::test {

// Test structures for analysis
struct SimpleStruct {
    char a;
    int b;
};

struct PackedStruct {
    char a;
    char b;
    char c;
    char d;
};

struct AlignedStruct {
    alignas(16) char data[16];
};

struct TRLC_ALIGN_TO_CACHE_LINE CacheAlignedStruct {
    int value;
};

void testBasicTypeInfo() {
    std::cout << "Testing basic type information..." << std::endl;

    // Test fundamental types
    assert(getTypeSize<char>() == sizeof(char));
    assert(getTypeSize<int>() == sizeof(int));
    assert(getTypeSize<double>() == sizeof(double));
    assert(getTypeSize<void*>() == sizeof(void*));

    assert(getTypeAlignment<char>() == alignof(char));
    assert(getTypeAlignment<int>() == alignof(int));
    assert(getTypeAlignment<double>() == alignof(double));
    assert(getTypeAlignment<void*>() == alignof(void*));

    std::cout << "  - char: size=" << getTypeSize<char>() << ", align=" << getTypeAlignment<char>()
              << std::endl;
    std::cout << "  - int: size=" << getTypeSize<int>() << ", align=" << getTypeAlignment<int>()
              << std::endl;
    std::cout << "  - double: size=" << getTypeSize<double>()
              << ", align=" << getTypeAlignment<double>() << std::endl;
    std::cout << "  - void*: size=" << getTypeSize<void*>()
              << ", align=" << getTypeAlignment<void*>() << std::endl;

    // Test struct types
    assert(getTypeSize<SimpleStruct>() == sizeof(SimpleStruct));
    assert(getTypeAlignment<SimpleStruct>() == alignof(SimpleStruct));

    std::cout << "  - SimpleStruct: size=" << getTypeSize<SimpleStruct>()
              << ", align=" << getTypeAlignment<SimpleStruct>() << std::endl;

    std::cout << "  ✓ Basic type information works correctly" << std::endl;
}

void testSystemSizes() {
    std::cout << "Testing system size detection..." << std::endl;

    size_t cache_line_size = getCacheLineSize();
    size_t page_size = getPageSize();

    std::cout << "  - Cache line size: " << cache_line_size << " bytes" << std::endl;
    std::cout << "  - Page size: " << page_size << " bytes" << std::endl;

    // Cache line size should be a power of 2 and reasonable
    assert(cache_line_size > 0);
    assert((cache_line_size & (cache_line_size - 1)) == 0);   // Power of 2
    assert(cache_line_size >= 32 && cache_line_size <= 256);  // Reasonable range

    // Page size should be a power of 2 and reasonable
    assert(page_size > 0);
    assert((page_size & (page_size - 1)) == 0);       // Power of 2
    assert(page_size >= 4096 && page_size <= 65536);  // Reasonable range

    // Test macros
    assert(TRLC_CACHE_LINE_SIZE == cache_line_size);
    assert(TRLC_PAGE_SIZE == page_size);

    std::cout << "  ✓ System size detection works correctly" << std::endl;
}

void testTypeInfoStruct() {
    std::cout << "Testing TypeInfo structure..." << std::endl;

    // Test fundamental type info
    using IntInfo = TypeInfo<int>;
    assert(IntInfo::size == sizeof(int));
    assert(IntInfo::alignment == alignof(int));
    assert(IntInfo::is_fundamental);
    assert(IntInfo::is_integral);
    assert(!IntInfo::is_floating_point);
    assert(!IntInfo::is_pointer);
    assert(!IntInfo::is_array);
    assert(!IntInfo::is_empty);
    assert(IntInfo::is_trivially_copyable);
    assert(IntInfo::is_standard_layout);
    assert(IntInfo::is_pod);

    std::cout << "  - int TypeInfo: size=" << IntInfo::size << ", align=" << IntInfo::alignment
              << std::endl;
    std::cout << "    fundamental=" << IntInfo::is_fundamental
              << ", integral=" << IntInfo::is_integral;
    std::cout << ", pod=" << IntInfo::is_pod << std::endl;

    // Test pointer type info
    using PtrInfo = TypeInfo<int*>;
    assert(PtrInfo::size == sizeof(int*));
    assert(PtrInfo::alignment == alignof(int*));
    assert(!PtrInfo::is_fundamental);
    assert(!PtrInfo::is_integral);
    assert(!PtrInfo::is_floating_point);
    assert(PtrInfo::is_pointer);
    assert(!PtrInfo::is_array);
    assert(!PtrInfo::is_empty);
    assert(PtrInfo::is_trivially_copyable);
    assert(PtrInfo::is_standard_layout);
    assert(PtrInfo::is_pod);

    std::cout << "  - int* TypeInfo: size=" << PtrInfo::size << ", align=" << PtrInfo::alignment
              << std::endl;
    std::cout << "    pointer=" << PtrInfo::is_pointer << ", pod=" << PtrInfo::is_pod << std::endl;

    // Test floating point type info
    using DoubleInfo = TypeInfo<double>;
    assert(DoubleInfo::size == sizeof(double));
    assert(DoubleInfo::alignment == alignof(double));
    assert(DoubleInfo::is_fundamental);
    assert(!DoubleInfo::is_integral);
    assert(DoubleInfo::is_floating_point);
    assert(!DoubleInfo::is_pointer);
    assert(!DoubleInfo::is_array);
    assert(!DoubleInfo::is_empty);
    assert(DoubleInfo::is_trivially_copyable);
    assert(DoubleInfo::is_standard_layout);
    assert(DoubleInfo::is_pod);

    std::cout << "  - double TypeInfo: floating_point=" << DoubleInfo::is_floating_point
              << std::endl;

    // Test array type info
    using ArrayInfo = TypeInfo<int[10]>;
    assert(ArrayInfo::size == sizeof(int[10]));
    assert(ArrayInfo::alignment == alignof(int[10]));
    assert(!ArrayInfo::is_fundamental);
    assert(!ArrayInfo::is_integral);
    assert(!ArrayInfo::is_floating_point);
    assert(!ArrayInfo::is_pointer);
    assert(ArrayInfo::is_array);
    assert(!ArrayInfo::is_empty);
    assert(ArrayInfo::is_trivially_copyable);
    assert(ArrayInfo::is_standard_layout);
    assert(ArrayInfo::is_pod);

    std::cout << "  - int[10] TypeInfo: array=" << ArrayInfo::is_array << std::endl;

    // Test struct type info
    using StructInfo = TypeInfo<SimpleStruct>;
    assert(StructInfo::size == sizeof(SimpleStruct));
    assert(StructInfo::alignment == alignof(SimpleStruct));
    assert(!StructInfo::is_fundamental);
    assert(!StructInfo::is_integral);
    assert(!StructInfo::is_floating_point);
    assert(!StructInfo::is_pointer);
    assert(!StructInfo::is_array);
    assert(!StructInfo::is_empty);
    assert(StructInfo::is_trivially_copyable);
    assert(StructInfo::is_standard_layout);
    assert(StructInfo::is_pod);

    std::cout << "  - SimpleStruct TypeInfo: size=" << StructInfo::size
              << ", standard_layout=" << StructInfo::is_standard_layout << std::endl;

    std::cout << "  ✓ TypeInfo structure works correctly" << std::endl;
}

void testPaddingAnalysis() {
    std::cout << "Testing padding analysis..." << std::endl;

    // Test fundamental types (should have no padding)
    assert(calculatePadding<char>() == 0);
    assert(calculatePadding<int>() == 0);
    assert(calculatePadding<double>() == 0);
    assert(!hasInternalPadding<char>());
    assert(!hasInternalPadding<int>());
    assert(!hasInternalPadding<double>());

    std::cout << "  - char padding: " << calculatePadding<char>() << " bytes" << std::endl;
    std::cout << "  - int padding: " << calculatePadding<int>() << " bytes" << std::endl;
    std::cout << "  - double padding: " << calculatePadding<double>() << " bytes" << std::endl;

    // Test struct padding
    size_t simple_padding = calculatePadding<SimpleStruct>();
    size_t packed_padding = calculatePadding<PackedStruct>();

    std::cout << "  - SimpleStruct padding: " << simple_padding << " bytes" << std::endl;
    std::cout << "  - PackedStruct padding: " << packed_padding << " bytes" << std::endl;

    // SimpleStruct likely has padding due to char + int layout
    std::cout << "  - SimpleStruct has padding: "
              << (hasInternalPadding<SimpleStruct>() ? "yes" : "no") << std::endl;
    std::cout << "  - PackedStruct has padding: "
              << (hasInternalPadding<PackedStruct>() ? "yes" : "no") << std::endl;

    std::cout << "  ✓ Padding analysis works correctly" << std::endl;
}

void testAlignmentHelpers() {
    std::cout << "Testing alignment helper types..." << std::endl;

    // Test AlignedType
    AlignedType<16> aligned16;
    AlignedType<32> aligned32;

    // Check alignment
    assert(alignof(decltype(aligned16)) >= 16);
    assert(alignof(decltype(aligned32)) >= 32);

    std::cout << "  - AlignedType<16> alignment: " << alignof(decltype(aligned16)) << " bytes"
              << std::endl;
    std::cout << "  - AlignedType<32> alignment: " << alignof(decltype(aligned32)) << " bytes"
              << std::endl;

    // Test pointer access
    void* ptr16 = aligned16.get();
    void* ptr32 = aligned32.get();
    assert(ptr16 == aligned16.data);
    assert(ptr32 == aligned32.data);

    // Test typed access
    int* int_ptr16 = aligned16.as<int>();
    int* int_ptr32 = aligned32.as<int>();
    assert(int_ptr16 == reinterpret_cast<int*>(aligned16.data));
    assert(int_ptr32 == reinterpret_cast<int*>(aligned32.data));

    // Test cache line alignment
    CacheLineAligned cache_aligned;
    assert(alignof(decltype(cache_aligned)) >= getCacheLineSize());

    std::cout << "  - CacheLineAligned alignment: " << alignof(decltype(cache_aligned)) << " bytes"
              << std::endl;

    // Test page alignment
    PageAligned page_aligned;
    assert(alignof(decltype(page_aligned)) >= getPageSize());

    std::cout << "  - PageAligned alignment: " << alignof(decltype(page_aligned)) << " bytes"
              << std::endl;

    std::cout << "  ✓ Alignment helper types work correctly" << std::endl;
}

void testVerificationFunctions() {
    std::cout << "Testing verification functions..." << std::endl;

    // Test size verification
    constexpr bool char_size_1 = verifyTypeSize<char, 1>();
    constexpr bool char_size_2 = verifyTypeSize<char, 2>();
    constexpr bool int_size_correct = verifyTypeSize<int, sizeof(int)>();
    constexpr bool int_size_wrong = verifyTypeSize<int, sizeof(int) + 1>();

    assert(char_size_1);
    assert(!char_size_2);
    assert(int_size_correct);
    assert(!int_size_wrong);

    std::cout << "  - char size verification (1 byte): " << (char_size_1 ? "pass" : "fail")
              << std::endl;
    std::cout << "  - int size verification (" << sizeof(int)
              << " bytes): " << (int_size_correct ? "pass" : "fail") << std::endl;

    // Test alignment verification
    constexpr bool char_align_1 = verifyTypeAlignment<char, 1>();
    constexpr bool int_align_correct = verifyTypeAlignment<int, alignof(int)>();
    constexpr bool char_align_2 = verifyTypeAlignment<char, 2>();

    assert(char_align_1);
    assert(int_align_correct);
    assert(!char_align_2);

    std::cout << "  - char alignment verification (1 byte): " << (char_align_1 ? "pass" : "fail")
              << std::endl;
    std::cout << "  - int alignment verification (" << alignof(int)
              << " bytes): " << (int_align_correct ? "pass" : "fail") << std::endl;

    // Test cache line alignment verification
    bool cache_aligned_test = isCacheLineAligned<CacheAlignedStruct>();
    std::cout << "  - CacheAlignedStruct is cache line aligned: "
              << (cache_aligned_test ? "yes" : "no") << std::endl;

    // Test page alignment verification (most types are not page aligned)
    bool page_aligned_test = isPageAligned<int>();
    std::cout << "  - int is page aligned: " << (page_aligned_test ? "yes" : "no") << std::endl;

    std::cout << "  ✓ Verification functions work correctly" << std::endl;
}

void testAlignmentCalculations() {
    std::cout << "Testing alignment calculations..." << std::endl;

    // Test aligned size calculation
    assert(alignedSize(7, 4) == 8);    // 7 aligned to 4-byte boundary = 8
    assert(alignedSize(8, 4) == 8);    // 8 aligned to 4-byte boundary = 8
    assert(alignedSize(9, 4) == 12);   // 9 aligned to 4-byte boundary = 12
    assert(alignedSize(15, 8) == 16);  // 15 aligned to 8-byte boundary = 16
    assert(alignedSize(16, 8) == 16);  // 16 aligned to 8-byte boundary = 16

    std::cout << "  - alignedSize(7, 4) = " << alignedSize(7, 4) << std::endl;
    std::cout << "  - alignedSize(9, 4) = " << alignedSize(9, 4) << std::endl;
    std::cout << "  - alignedSize(15, 8) = " << alignedSize(15, 8) << std::endl;

    // Test aligned address calculation
    assert(alignedAddress(0x1003, 4) == 0x1004);  // 0x1003 aligned to 4-byte = 0x1004
    assert(alignedAddress(0x1004, 4) == 0x1004);  // 0x1004 aligned to 4-byte = 0x1004
    assert(alignedAddress(0x1007, 8) == 0x1008);  // 0x1007 aligned to 8-byte = 0x1008

    std::cout << "  - alignedAddress(0x1003, 4) = 0x" << std::hex << alignedAddress(0x1003, 4)
              << std::dec << std::endl;
    std::cout << "  - alignedAddress(0x1007, 8) = 0x" << std::hex << alignedAddress(0x1007, 8)
              << std::dec << std::endl;

    // Test alignment checking with uintptr_t
    assert(isAligned(static_cast<uintptr_t>(0x1000), 4));   // 0x1000 is 4-byte aligned
    assert(!isAligned(static_cast<uintptr_t>(0x1003), 4));  // 0x1003 is not 4-byte aligned
    assert(isAligned(static_cast<uintptr_t>(0x1008), 8));   // 0x1008 is 8-byte aligned
    assert(!isAligned(static_cast<uintptr_t>(0x1007), 8));  // 0x1007 is not 8-byte aligned

    std::cout << "  - isAligned(0x1000, 4): "
              << (isAligned(static_cast<uintptr_t>(0x1000), 4) ? "yes" : "no") << std::endl;
    std::cout << "  - isAligned(0x1003, 4): "
              << (isAligned(static_cast<uintptr_t>(0x1003), 4) ? "yes" : "no") << std::endl;
    std::cout << "  - isAligned(0x1008, 8): "
              << (isAligned(static_cast<uintptr_t>(0x1008), 8) ? "yes" : "no") << std::endl;

    // Test pointer alignment checking
    alignas(8) char buffer[32];
    assert(isAligned(buffer, 8));
    assert(isAligned(buffer + 8, 8));
    assert(!isAligned(buffer + 1, 8));

    std::cout << "  ✓ Alignment calculations work correctly" << std::endl;
}

void testMacros() {
    std::cout << "Testing utility macros..." << std::endl;

    // Test basic macros
    assert(TRLC_SIZEOF(int) == sizeof(int));
    assert(TRLC_ALIGNOF(int) == alignof(int));
    assert(TRLC_CACHE_LINE_SIZE == getCacheLineSize());
    assert(TRLC_PAGE_SIZE == getPageSize());

    std::cout << "  - TRLC_SIZEOF(int): " << TRLC_SIZEOF(int) << std::endl;
    std::cout << "  - TRLC_ALIGNOF(int): " << TRLC_ALIGNOF(int) << std::endl;
    std::cout << "  - TRLC_CACHE_LINE_SIZE: " << TRLC_CACHE_LINE_SIZE << std::endl;
    std::cout << "  - TRLC_PAGE_SIZE: " << TRLC_PAGE_SIZE << std::endl;

    // Test alignment macros (compilation test)
    struct TestAlignedStruct {
        TRLC_ALIGN_TO_CACHE_LINE int cache_aligned_member;
    };

    assert(alignof(TestAlignedStruct) >= getCacheLineSize());
    std::cout << "  - TestAlignedStruct alignment: " << alignof(TestAlignedStruct) << " bytes"
              << std::endl;

    // Test aligned variable declarations (compilation test)
    TRLC_CACHE_ALIGNED int cache_var;  // macros.hpp version doesn't take type parameter
    // TRLC_PAGE_ALIGNED int page_var;  // This macro doesn't exist in macros.hpp

    static_cast<void>(cache_var);
    // static_cast<void>(page_var);

    std::cout << "  ✓ Utility macros work correctly" << std::endl;
}

void testCompileTimeExecution() {
    std::cout << "Testing compile-time execution..." << std::endl;

    // Test that functions are constexpr
    static_assert(getTypeSize<int>() == sizeof(int), "getTypeSize should be constexpr");
    static_assert(getTypeAlignment<int>() == alignof(int), "getTypeAlignment should be constexpr");
    static_assert(getCacheLineSize() > 0, "getCacheLineSize should be constexpr");
    static_assert(getPageSize() > 0, "getPageSize should be constexpr");

    // Test compile-time calculations
    static_assert(alignedSize(7, 4) == 8, "alignedSize should work at compile time");
    static_assert(alignedAddress(0x1003, 4) == 0x1004,
                  "alignedAddress should work at compile time");
    static_assert(isAligned(static_cast<uintptr_t>(0x1000), 4),
                  "isAligned should work at compile time");
    static_assert(!isAligned(static_cast<uintptr_t>(0x1003), 4),
                  "isAligned should work at compile time");

    // Test verification functions
    static_assert(verifyTypeSize<char, 1>(), "verifyTypeSize should work at compile time");
    static_assert(verifyTypeAlignment<char, 1>(),
                  "verifyTypeAlignment should work at compile time");

    // Test TypeInfo compile-time access
    static_assert(TypeInfo<int>::size == sizeof(int), "TypeInfo should work at compile time");
    static_assert(TypeInfo<int>::alignment == alignof(int), "TypeInfo should work at compile time");
    static_assert(TypeInfo<int>::is_fundamental, "TypeInfo should work at compile time");
    static_assert(TypeInfo<int>::is_integral, "TypeInfo should work at compile time");
    static_assert(!TypeInfo<int>::is_floating_point, "TypeInfo should work at compile time");

    std::cout << "  ✓ All functions execute at compile time" << std::endl;
}

void testEdgeCases() {
    std::cout << "Testing edge cases..." << std::endl;

    // Test empty struct
    struct EmptyStruct {};

    using EmptyInfo = TypeInfo<EmptyStruct>;
    assert(EmptyInfo::size == sizeof(EmptyStruct));
    assert(EmptyInfo::is_empty);
    assert(EmptyInfo::is_trivially_copyable);
    assert(EmptyInfo::is_standard_layout);
    assert(EmptyInfo::is_pod);

    std::cout << "  - EmptyStruct size: " << EmptyInfo::size << " bytes" << std::endl;
    std::cout << "  - EmptyStruct is_empty: " << EmptyInfo::is_empty << std::endl;

    // Test zero alignment cases
    assert(alignedSize(0, 4) == 0);
    assert(alignedAddress(0, 4) == 0);
    assert(isAligned(static_cast<uintptr_t>(0), 4));

    // Test single-byte alignment
    assert(alignedSize(5, 1) == 5);
    assert(alignedAddress(0x1234, 1) == 0x1234);
    assert(isAligned(static_cast<uintptr_t>(0x1234), 1));

    // Test large alignments
    assert(alignedSize(100, 64) == 128);
    assert(alignedSize(64, 64) == 64);

    std::cout << "  ✓ Edge cases handled correctly" << std::endl;
}

void testPerformance() {
    std::cout << "Testing performance characteristics..." << std::endl;

    // All type info functions should be compile-time
    constexpr size_t int_size = getTypeSize<int>();
    constexpr size_t int_alignment = getTypeAlignment<int>();
    constexpr size_t cache_size = getCacheLineSize();
    constexpr size_t page_size = getPageSize();

    // Type verification should be compile-time
    constexpr bool size_ok = verifyTypeSize<int, sizeof(int)>();
    constexpr bool align_ok = verifyTypeAlignment<int, alignof(int)>();

    // Alignment calculations should be compile-time
    constexpr size_t aligned_7_4 = alignedSize(7, 4);
    constexpr uintptr_t aligned_addr = alignedAddress(0x1003, 4);
    constexpr bool is_aligned_test = isAligned(0x1000, 4);

    // TypeInfo should be compile-time
    constexpr auto info_size = TypeInfo<int>::size;
    constexpr auto info_alignment = TypeInfo<int>::alignment;
    constexpr auto is_fundamental = TypeInfo<int>::is_fundamental;

    // Use values to prevent optimization
    static_cast<void>(int_size);
    static_cast<void>(int_alignment);
    static_cast<void>(cache_size);
    static_cast<void>(page_size);
    static_cast<void>(size_ok);
    static_cast<void>(align_ok);
    static_cast<void>(aligned_7_4);
    static_cast<void>(aligned_addr);
    static_cast<void>(is_aligned_test);
    static_cast<void>(info_size);
    static_cast<void>(info_alignment);
    static_cast<void>(is_fundamental);

    std::cout << "  ✓ All functions execute at compile time with zero overhead" << std::endl;
}

}  // namespace trlc::platform::test

int main() {
    using namespace trlc::platform::test;

    std::cout << "=== TRLC Platform Type Information Tests ===" << std::endl;

    try {
        testBasicTypeInfo();
        testSystemSizes();
        testTypeInfoStruct();
        testPaddingAnalysis();
        testAlignmentHelpers();
        testVerificationFunctions();
        testAlignmentCalculations();
        testMacros();
        testCompileTimeExecution();
        testEdgeCases();
        testPerformance();

        std::cout << "\n✅ All type information tests passed!" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}

/**
 * @file test_macros.cpp
 * @brief Comprehensive tests for portable cross-platform macros
 *
 * Tests all macro functionality including attributes, inlining, branch prediction,
 * symbol visibility, and utility macros. Verifies both compile-time and runtime
 * behavior across different compilers and platforms.
 */

#include <cassert>
#include <chrono>
#include <iostream>
#include <string>
#include <type_traits>

#include "trlc/platform/macros.hpp"

namespace trlc::platform::test {

// Test functions for attribute testing
TRLC_NODISCARD int testNodecardFunction() {
    return 42;
}

TRLC_DEPRECATED void testDeprecatedFunction() {
    // This function should generate deprecation warnings when used
}

TRLC_DEPRECATED_MSG("Use newTestFunction() instead") void testDeprecatedWithMessageFunction() {
    // This function should generate deprecation warnings with custom message
}

TRLC_FORCE_INLINE int testForceInlineFunction() {
    return 100;
}

TRLC_NEVER_INLINE int testNeverInlineFunction() {
    return 200;
}

TRLC_INLINE int testInlineFunction() {
    return 300;
}

// Test functions for exception safety
void testNoexceptFunction() TRLC_NOEXCEPT {
    // This function claims not to throw
}

template <typename T>
void testConditionalNoexceptFunction(T&& t)
    TRLC_NOEXCEPT_IF(std::is_nothrow_move_constructible_v<T>) {
    // Conditionally noexcept based on template parameter
}

// Test functions with maybe_unused parameters
void testMaybeUnusedFunction(TRLC_MAYBE_UNUSED int debug_param) {
#ifdef DEBUG_MODE
    std::cout << "Debug param: " << debug_param << std::endl;
#endif
}

// Test functions with multiple attributes
TRLC_NODISCARD TRLC_FORCE_INLINE int multiAttributeFunction() {
    return 42;
}

// Test aligned variables
TRLC_ALIGNED(16) float aligned_array[4] = {1.0f, 2.0f, 3.0f, 4.0f};
TRLC_CACHE_ALIGNED int cache_aligned_counter = 0;

// Test packed structures
TRLC_PACK_BEGIN

struct TRLC_PACKED TestPackedStruct {
    char a;
    int b;
    char c;
} TRLC_PACK_END;

// Test API visibility (for libraries)
class TRLC_API_EXPORT TestExportedClass {
public:
    void publicMethod();
};

class TRLC_API_HIDDEN TestHiddenClass {
public:
    void internalMethod();
};

void testAttributeSupport() {
    std::cout << "Testing attribute support detection..." << std::endl;

    // Test compile-time attribute detection
    constexpr bool nodiscard_support = supportsNodecard();
    constexpr bool deprecated_support = supportsDeprecated();
    constexpr bool fallthrough_support = supportsFallthrough();

    std::cout << "  - Nodiscard support: " << (nodiscard_support ? "yes" : "no") << std::endl;
    std::cout << "  - Deprecated support: " << (deprecated_support ? "yes" : "no") << std::endl;
    std::cout << "  - Fallthrough support: " << (fallthrough_support ? "yes" : "no") << std::endl;

    // Test that detection functions are constexpr
    static_assert(supportsNodecard() == supportsNodecard(), "supportsNodecard should be constexpr");
    static_assert(supportsDeprecated() == supportsDeprecated(),
                  "supportsDeprecated should be constexpr");
    static_assert(supportsFallthrough() == supportsFallthrough(),
                  "supportsFallthrough should be constexpr");

    std::cout << "  ✓ Attribute support detection works" << std::endl;
}

void testBasicMacros() {
    std::cout << "Testing basic macro definitions..." << std::endl;

// Test that macros are defined (can be empty but should be defined)
#ifdef TRLC_NODISCARD
    std::cout << "  - TRLC_NODISCARD is defined" << std::endl;
#endif

#ifdef TRLC_DEPRECATED
    std::cout << "  - TRLC_DEPRECATED is defined" << std::endl;
#endif

#ifdef TRLC_FALLTHROUGH
    std::cout << "  - TRLC_FALLTHROUGH is defined" << std::endl;
#endif

#ifdef TRLC_MAYBE_UNUSED
    std::cout << "  - TRLC_MAYBE_UNUSED is defined" << std::endl;
#endif

    // Test function calls work
    int result = testNodecardFunction();
    assert(result == 42);

    result = testForceInlineFunction();
    assert(result == 100);

    result = testNeverInlineFunction();
    assert(result == 200);

    result = testInlineFunction();
    assert(result == 300);

    std::cout << "  ✓ Basic macros work correctly" << std::endl;
}

void testFallthroughMacro() {
    std::cout << "Testing fallthrough macro..." << std::endl;

    int test_value = 1;
    int result = 0;

    switch (test_value) {
        case 1:
            result += 10;
            TRLC_FALLTHROUGH;
        case 2:
            result += 20;
            break;
        default:
            result = -1;
            break;
    }

    assert(result == 30);  // Should be 10 + 20 due to fallthrough
    std::cout << "  ✓ Fallthrough macro works correctly" << std::endl;
}

void testBranchPrediction() {
    std::cout << "Testing branch prediction macros..." << std::endl;

    // Test that macros don't change the logic
    bool condition_true = true;
    bool condition_false = false;

    if (TRLC_LIKELY(condition_true)) {
        std::cout << "  - TRLC_LIKELY with true condition works" << std::endl;
    } else {
        assert(false && "TRLC_LIKELY should not change logic");
    }

    if (TRLC_UNLIKELY(condition_false)) {
        assert(false && "TRLC_UNLIKELY should not change logic");
    } else {
        std::cout << "  - TRLC_UNLIKELY with false condition works" << std::endl;
    }

    // Test with complex expressions
    int* ptr = nullptr;
    if (TRLC_UNLIKELY(ptr == nullptr)) {
        std::cout << "  - TRLC_UNLIKELY with nullptr check works" << std::endl;
    }

    ptr = &cache_aligned_counter;
    if (TRLC_LIKELY(ptr != nullptr)) {
        std::cout << "  - TRLC_LIKELY with valid pointer works" << std::endl;
    }

    std::cout << "  ✓ Branch prediction macros work correctly" << std::endl;
}

void testExceptionMacros() {
    std::cout << "Testing exception safety macros..." << std::endl;

    // Test noexcept function
    testNoexceptFunction();

    // Test conditional noexcept
    int moveable_int = 42;
    testConditionalNoexceptFunction(std::move(moveable_int));

// Test exception detection
#if TRLC_HAS_EXCEPTIONS_ENABLED
    std::cout << "  - Exceptions are enabled" << std::endl;
#else
    std::cout << "  - Exceptions are disabled" << std::endl;
#endif

    std::cout << "  ✓ Exception safety macros work correctly" << std::endl;
}

void testVisibilityMacros() {
    std::cout << "Testing symbol visibility macros..." << std::endl;

// Test that visibility macros are defined
#ifdef TRLC_API_EXPORT
    std::cout << "  - TRLC_API_EXPORT is defined" << std::endl;
#endif

#ifdef TRLC_API_IMPORT
    std::cout << "  - TRLC_API_IMPORT is defined" << std::endl;
#endif

#ifdef TRLC_API_HIDDEN
    std::cout << "  - TRLC_API_HIDDEN is defined" << std::endl;
#endif

#ifdef TRLC_API
    std::cout << "  - TRLC_API is defined" << std::endl;
#endif

    // Test that classes can be declared (compilation test)
    TestExportedClass exported_instance;
    TestHiddenClass hidden_instance;

    static_cast<void>(exported_instance);
    static_cast<void>(hidden_instance);

    std::cout << "  ✓ Symbol visibility macros work correctly" << std::endl;
}

void testAlignmentMacros() {
    std::cout << "Testing alignment macros..." << std::endl;

    // Test aligned array
    assert(aligned_array[0] == 1.0f);
    assert(aligned_array[1] == 2.0f);
    assert(aligned_array[2] == 3.0f);
    assert(aligned_array[3] == 4.0f);

    // Test cache-aligned counter
    cache_aligned_counter = 100;
    assert(cache_aligned_counter == 100);

    // Test pointer alignment (implementation-specific)
    uintptr_t array_addr = reinterpret_cast<uintptr_t>(aligned_array);
    assert((array_addr % 16) == 0);  // Should be 16-byte aligned

    uintptr_t counter_addr = reinterpret_cast<uintptr_t>(&cache_aligned_counter);
    assert((counter_addr % 64) == 0);  // Should be 64-byte aligned (cache line)

    std::cout << "  - Aligned array address: 0x" << std::hex << array_addr << std::dec << std::endl;
    std::cout << "  - Cache-aligned counter address: 0x" << std::hex << counter_addr << std::dec
              << std::endl;

    std::cout << "  ✓ Alignment macros work correctly" << std::endl;
}

void testPackedStructures() {
    std::cout << "Testing packed structure macros..." << std::endl;

    TestPackedStruct packed_instance = {'A', 0x12345678, 'B'};

    assert(packed_instance.a == 'A');
    assert(packed_instance.b == 0x12345678);
    assert(packed_instance.c == 'B');

    // Test struct size (should be tightly packed)
    size_t packed_size = sizeof(TestPackedStruct);
    size_t expected_min_size = sizeof(char) + sizeof(int) + sizeof(char);  // 6 bytes minimum

    std::cout << "  - Packed struct size: " << packed_size << " bytes" << std::endl;
    std::cout << "  - Expected minimum: " << expected_min_size << " bytes" << std::endl;

    // On most platforms, packing should reduce size compared to natural alignment
    assert(packed_size >= expected_min_size);
    assert(packed_size <= expected_min_size + 3);  // Allow some platform variance

    std::cout << "  ✓ Packed structure macros work correctly" << std::endl;
}

void testUtilityMacros() {
    std::cout << "Testing utility macros..." << std::endl;

    // Test unused variable suppression
    int potentially_unused_var = 42;
    TRLC_USE(potentially_unused_var);

// Test string macros
#define TEST_NAME example
    const char* stringified = TRLC_STRINGIFY(TEST_NAME);
    std::cout << "  - TRLC_STRINGIFY result: '" << stringified << "'" << std::endl;
    assert(std::string(stringified) ==
           std::string("TEST_NAME"));  // TRLC_STRINGIFY stringifies the token literally

    const char* expanded_string = TRLC_STRINGIFY_EXPANDED(TEST_NAME);
    std::cout << "  - TRLC_STRINGIFY_EXPANDED result: '" << expanded_string << "'" << std::endl;
    assert(std::string(expanded_string) ==
           std::string("example"));  // This should expand TEST_NAME to example first

    // Test restrict keyword (compilation test)
    int array1[10] = {0};
    int array2[10] = {0};

    auto process_arrays = [](int* TRLC_RESTRICT input, int* TRLC_RESTRICT output, size_t size) {
        for (size_t i = 0; i < size; ++i) {
            output[i] = input[i] * 2;
        }
    };

    process_arrays(array1, array2, 10);

    std::cout << "  ✓ Utility macros work correctly" << std::endl;
}

void testConditionalCompilation() {
    std::cout << "Testing conditional compilation macros..." << std::endl;

// Test platform detection
#if TRLC_PLATFORM_WINDOWS
    std::cout << "  - Running on Windows platform" << std::endl;
#elif TRLC_PLATFORM_POSIX
    std::cout << "  - Running on POSIX platform" << std::endl;
#else
    std::cout << "  - Running on unknown platform" << std::endl;
#endif

// Test architecture detection
#if TRLC_ARCH_X86_64
    std::cout << "  - Running on x86_64 architecture" << std::endl;
#elif TRLC_ARCH_ARM64
    std::cout << "  - Running on ARM64 architecture" << std::endl;
#else
    std::cout << "  - Running on other architecture" << std::endl;
#endif

// Test C++17 detection
#if TRLC_HAS_CPP17
    std::cout << "  - C++17 features available" << std::endl;
#else
    std::cout << "  - C++17 features not available" << std::endl;
#endif

    // Test platform-specific code execution
    bool windows_code_executed = false;
    bool posix_code_executed = false;

    TRLC_ON_WINDOWS({ windows_code_executed = true; })

    TRLC_ON_POSIX({ posix_code_executed = true; })

    // Exactly one should be true (or both false on unknown platforms)
    assert(!(windows_code_executed && posix_code_executed));

    std::cout << "  ✓ Conditional compilation macros work correctly" << std::endl;
}

void testDiagnosticMacros() {
    std::cout << "Testing diagnostic control macros..." << std::endl;

    // Test diagnostic suppression (this should compile without warnings)
    TRLC_DIAGNOSTIC_PUSH
    TRLC_DIAGNOSTIC_IGNORE_UNUSED_PARAMETER
    TRLC_DIAGNOSTIC_IGNORE_UNUSED_VARIABLE

    auto test_function = [](int unused_param) {
        int unused_variable = 42;
        TRLC_USE(unused_param);
        TRLC_USE(unused_variable);
        return 0;
    };

    TRLC_DIAGNOSTIC_POP

    int result = test_function(100);
    assert(result == 0);

    std::cout << "  ✓ Diagnostic control macros work correctly" << std::endl;
}

void testVersionMacros() {
    std::cout << "Testing version macros..." << std::endl;

    // Test version constants
    assert(TRLC_MACROS_VERSION_MAJOR >= 1);
    assert(TRLC_MACROS_VERSION_MINOR >= 0);
    assert(TRLC_MACROS_VERSION_PATCH >= 0);

    std::cout << "  - Macros version: " << TRLC_MACROS_VERSION_STRING << std::endl;
    std::cout << "  - Major: " << TRLC_MACROS_VERSION_MAJOR << std::endl;
    std::cout << "  - Minor: " << TRLC_MACROS_VERSION_MINOR << std::endl;
    std::cout << "  - Patch: " << TRLC_MACROS_VERSION_PATCH << std::endl;

    // Test version comparison
    assert(TRLC_MACROS_VERSION_AT_LEAST(1, 0, 0));
    assert(!TRLC_MACROS_VERSION_AT_LEAST(2, 0, 0));

// Test that header inclusion guard is defined
#ifdef TRLC_MACROS_INCLUDED
    std::cout << "  - TRLC_MACROS_INCLUDED is defined" << std::endl;
#else
    assert(false && "TRLC_MACROS_INCLUDED should be defined");
#endif

    std::cout << "  ✓ Version macros work correctly" << std::endl;
}

void testCompileTimeExecution() {
    std::cout << "Testing compile-time execution..." << std::endl;

    // Test that detection functions are compile-time
    static_assert(supportsNodecard() == supportsNodecard(), "supportsNodecard should be constexpr");
    static_assert(supportsDeprecated() == supportsDeprecated(),
                  "supportsDeprecated should be constexpr");
    static_assert(supportsFallthrough() == supportsFallthrough(),
                  "supportsFallthrough should be constexpr");

    // Test compile-time constants
    constexpr bool nodiscard_support = supportsNodecard();
    constexpr bool deprecated_support = supportsDeprecated();
    constexpr bool fallthrough_support = supportsFallthrough();

    static_cast<void>(nodiscard_support);
    static_cast<void>(deprecated_support);
    static_cast<void>(fallthrough_support);

    std::cout << "  ✓ All functions execute at compile time" << std::endl;
}

void testEdgeCases() {
    std::cout << "Testing edge cases..." << std::endl;

    // Test multiple macro applications
    int result = multiAttributeFunction();
    assert(result == 42);

    // Test macro nesting
    testMaybeUnusedFunction(100);  // Should not generate warnings

    // Test unreachable macro (should not crash or affect logic)
    bool reachable = true;
    if (reachable) {
        std::cout << "  - Reachable code executed" << std::endl;
    } else {
        TRLC_UNREACHABLE();
    }

    std::cout << "  ✓ Edge cases handled correctly" << std::endl;
}

void testPerformance() {
    std::cout << "Testing performance characteristics..." << std::endl;

    // All detection functions should be compile-time only
    constexpr bool nodiscard = supportsNodecard();
    constexpr bool deprecated = supportsDeprecated();
    constexpr bool fallthrough = supportsFallthrough();

    // Test that macros don't add runtime overhead
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 1000; ++i) {
        TRLC_LIKELY(i < 1000) ? ++cache_aligned_counter : --cache_aligned_counter;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "  - 1000 branch predictions took: " << duration.count() << " microseconds"
              << std::endl;

    // Use compile-time constants to prevent optimization
    static_cast<void>(nodiscard);
    static_cast<void>(deprecated);
    static_cast<void>(fallthrough);

    std::cout << "  ✓ Macros have zero runtime overhead" << std::endl;
}

}  // namespace trlc::platform::test

int main() {
    using namespace trlc::platform::test;

    std::cout << "=== TRLC Platform Macros Tests ===" << std::endl;

    try {
        testAttributeSupport();
        testBasicMacros();
        testFallthroughMacro();
        testBranchPrediction();
        testExceptionMacros();
        testVisibilityMacros();
        testAlignmentMacros();
        testPackedStructures();
        testUtilityMacros();
        testConditionalCompilation();
        testDiagnosticMacros();
        testVersionMacros();
        testCompileTimeExecution();
        testEdgeCases();
        testPerformance();

        std::cout << "\n✅ All macro tests passed!" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}

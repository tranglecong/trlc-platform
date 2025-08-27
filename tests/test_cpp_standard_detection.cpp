/**
 * @file test_cpp_standard_detection.cpp
 * @brief Comprehensive tests for C++ standard detection functionality
 *
 * Tests all C++ standard detection macros, functions, and feature detection
 * including compile-time detection accuracy and runtime verification.
 */

#include <cassert>
#include <iostream>
#include <string>

#include "trlc/platform/cpp_standard.hpp"

namespace trlc::platform::test {

// Test basic C++ standard detection macros
void testCppStandardDetectionMacros() {
    std::cout << "Testing C++ standard detection macros..." << std::endl;

    // Check which standards are detected
    bool cpp17_detected = false, cpp20_detected = false, cpp23_detected = false;

#if TRLC_CPP17
    cpp17_detected = true;
    std::cout << "  - C++17 detected" << std::endl;
#endif

#if TRLC_CPP20
    cpp20_detected = true;
    std::cout << "  - C++20 detected" << std::endl;
#endif

#if TRLC_CPP23
    cpp23_detected = true;
    std::cout << "  - C++23 detected" << std::endl;
#endif

    // At least C++17 should be detected for builds
    assert(cpp17_detected);
    std::cout << "  ✓ At least C++17 detected" << std::endl;

    // Test logical ordering
    if (cpp20_detected)
        assert(cpp17_detected);
    if (cpp23_detected)
        assert(cpp20_detected);

    std::cout << "  ✓ Standard detection hierarchy is correct" << std::endl;
}

// Test C++ standard type detection
void testCppStandardTypeDetection() {
    std::cout << "Testing C++ standard type detection..." << std::endl;

    // Test that a standard is detected (not unknown)
    auto standard = getCppStandard();
    assert(standard != CppStandard::cpp_unknown);
    std::cout << "  ✓ C++ standard detected: " << static_cast<long>(standard) << std::endl;

    // Test version macro
    auto version_macro = getCppVersionMacro();
    assert(version_macro >= 201703L);  // At least C++17
    std::cout << "  ✓ Version macro: " << version_macro << std::endl;
}

// Test standard feature detection
void testStandardFeatureDetection() {
    std::cout << "Testing standard feature detection..." << std::endl;

    auto standard = getCppStandard();

    // Test feature functions based on current standard
    if (standard >= CppStandard::cpp_17) {
        assert(hasStructuredBindings());
        assert(hasIfConstexpr());
        assert(hasFoldExpressions());
        assert(hasInlineVariables());
        std::cout << "  ✓ C++17 features confirmed" << std::endl;
    }

    if (standard >= CppStandard::cpp_20) {
        assert(hasConcepts());
        assert(hasCoroutines());
        assert(hasConsteval());
        assert(hasConstinit());
        assert(hasDesignatedInitializers());
        assert(hasThreeWayComparison());
        std::cout << "  ✓ C++20 features confirmed" << std::endl;
    }

    std::cout << "  ✓ Standard features tested" << std::endl;
}

// Test library feature detection using macros
void testLibraryFeatureDetection() {
    std::cout << "Testing library feature detection..." << std::endl;

    // Test standard library features using macros
    bool has_string_view = (TRLC_HAS_STRING_VIEW == 1);
    bool has_optional = (TRLC_HAS_OPTIONAL == 1);
    bool has_variant = (TRLC_HAS_VARIANT == 1);
    bool has_filesystem = (TRLC_HAS_FILESYSTEM == 1);
    bool has_span = (TRLC_HAS_SPAN == 1);
    bool has_format = (TRLC_HAS_FORMAT == 1);

    std::cout << "  - string_view: " << (has_string_view ? "yes" : "no") << std::endl;
    std::cout << "  - optional: " << (has_optional ? "yes" : "no") << std::endl;
    std::cout << "  - variant: " << (has_variant ? "yes" : "no") << std::endl;
    std::cout << "  - filesystem: " << (has_filesystem ? "yes" : "no") << std::endl;
    std::cout << "  - span: " << (has_span ? "yes" : "no") << std::endl;
    std::cout << "  - format: " << (has_format ? "yes" : "no") << std::endl;

    // Test feature dependencies
    auto standard = getCppStandard();
    if (standard >= CppStandard::cpp_17) {
        assert(has_string_view);
        assert(has_optional);
        assert(has_variant);
        assert(has_filesystem);
    }

    std::cout << "  ✓ Library features tested" << std::endl;
}

// Test CppStandardInfo struct
void testCppStandardInfoStruct() {
    std::cout << "Testing CppStandardInfo struct..." << std::endl;

    CppStandardInfo info = getCppStandardInfo();

    // Test that fields are populated
    assert(info.standard_name != nullptr);
    assert(std::string(info.standard_name).length() > 0);
    assert(info.standard != CppStandard::cpp_unknown);
    assert(info.version_macro >= 201703L);

    std::cout << "  - Name: " << info.standard_name << std::endl;
    std::cout << "  - Standard: " << static_cast<long>(info.standard) << std::endl;
    std::cout << "  - Version macro: " << info.version_macro << std::endl;

    // Test methods
    std::cout << "  - Is at least C++17: " << (info.isAtLeast(CppStandard::cpp_17) ? "yes" : "no")
              << std::endl;
    std::cout << "  - Is at least C++20: " << (info.isAtLeast(CppStandard::cpp_20) ? "yes" : "no")
              << std::endl;

    // Basic sanity checks
    assert(info.isAtLeast(CppStandard::cpp_17));  // Should be true for modern builds

    std::cout << "  ✓ CppStandardInfo struct works correctly" << std::endl;
}

// Test compile-time detection
void testCompileTimeDetection() {
    std::cout << "Testing compile-time detection..." << std::endl;

    // These should all be compile-time constant expressions
    static_assert(getCppStandard() != CppStandard::cpp_unknown,
                  "C++ standard should be detected at compile time");

    static_assert(getCppVersionMacro() >= 201703L, "Standard version should be at least C++17");

    constexpr auto info = getCppStandardInfo();
    static_assert(info.standard_name != nullptr, "Standard name should not be null");

    // Test feature detection at compile time
    static_assert(hasStructuredBindings(), "structured bindings should be available in C++17+");

    std::cout << "  ✓ All compile-time detections work" << std::endl;
}

// Test standard-specific features
void testStandardSpecificFeatures() {
    std::cout << "Testing standard-specific features..." << std::endl;

    auto standard = getCppStandard();

    if (standard == CppStandard::cpp_17) {
        std::cout << "  - Testing C++17-specific features" << std::endl;
        assert(hasStructuredBindings());
        assert(hasIfConstexpr());
        assert(!hasConcepts());  // Should not have C++20 features

    } else if (standard == CppStandard::cpp_20) {
        std::cout << "  - Testing C++20-specific features" << std::endl;
        assert(hasConcepts());
        assert(hasCoroutines());
        assert(hasConsteval());

    } else if (standard == CppStandard::cpp_23) {
        std::cout << "  - Testing C++23-specific features" << std::endl;
        // C++23 features would be tested here
    }

    std::cout << "  ✓ Standard-specific features tested" << std::endl;
}

// Test version comparison
void testVersionComparison() {
    std::cout << "Testing version comparison..." << std::endl;

    auto current = getCppStandard();

    // Test comparison operators
    assert(isCpp17OrLater());

    if (current >= CppStandard::cpp_20) {
        assert(isCpp20OrLater());
        assert(isCpp17OrLater());
    }

    // Test specific comparisons
    bool supports_concepts = isCpp20OrLater();
    bool supports_constexpr_if = isCpp17OrLater();

    std::cout << "  - Supports concepts: " << (supports_concepts ? "yes" : "no") << std::endl;
    std::cout << "  - Supports constexpr if: " << (supports_constexpr_if ? "yes" : "no")
              << std::endl;

    assert(supports_constexpr_if);  // Should always be true for C++17+ builds

    std::cout << "  ✓ Version comparison works correctly" << std::endl;
}

// Test edge cases
void testEdgeCases() {
    std::cout << "Testing edge cases..." << std::endl;

    // Test string lengths
    auto info = getCppStandardInfo();
    auto name_len = std::string(info.standard_name).length();
    assert(name_len > 0 && name_len < 50);

    // Test enum bounds
    auto standard = getCppStandard();
    assert(static_cast<long>(standard) >= 0 || standard == CppStandard::cpp_unknown);

    // Test version bounds
    auto version = getCppVersionMacro();
    assert(version >= 201703L && version <= 999999L);

    std::cout << "  ✓ Edge cases handled correctly" << std::endl;
}

// Performance test - verify compile-time execution
void testPerformance() {
    std::cout << "Testing performance (compile-time execution)..." << std::endl;

    // All these should be resolved at compile time
    constexpr auto standard = getCppStandard();
    constexpr auto version_macro = getCppVersionMacro();
    constexpr auto has_structured_bindings = hasStructuredBindings();
    constexpr auto has_if_constexpr = hasIfConstexpr();
    constexpr auto info = getCppStandardInfo();

    // Use the values to prevent optimization
    static_cast<void>(standard);
    static_cast<void>(version_macro);
    static_cast<void>(has_structured_bindings);
    static_cast<void>(has_if_constexpr);
    static_cast<void>(info);

    std::cout << "  ✓ All functions execute at compile time" << std::endl;
}

}  // namespace trlc::platform::test

int main() {
    using namespace trlc::platform::test;

    std::cout << "=== TRLC Platform C++ Standard Detection Tests ===" << std::endl;

    try {
        testCppStandardDetectionMacros();
        testCppStandardTypeDetection();
        testStandardFeatureDetection();
        testLibraryFeatureDetection();
        testCppStandardInfoStruct();
        testCompileTimeDetection();
        testStandardSpecificFeatures();
        testVersionComparison();
        testEdgeCases();
        testPerformance();

        std::cout << "\n✅ All C++ standard detection tests passed!" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}

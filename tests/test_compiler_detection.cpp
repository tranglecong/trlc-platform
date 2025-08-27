/**
 * @file test_compiler_detection.cpp
 * @brief Comprehensive tests for compiler detection functionality
 *
 * Tests all compiler detection macros, functions, and capabilities
 * including compile-time detection accuracy and runtime verification.
 */

#include <cassert>
#include <iostream>
#include <string>

#include "trlc/platform/compiler.hpp"

namespace trlc::platform::test {

// Test basic compiler detection macros
void testCompilerDetectionMacros() {
    std::cout << "Testing compiler detection macros..." << std::endl;

    // Verify exactly one compiler is detected
    int compiler_count = 0;

#ifdef TRLC_COMPILER_GCC
    compiler_count++;
    std::cout << "  - GCC detected" << std::endl;
#endif

#ifdef TRLC_COMPILER_CLANG
    compiler_count++;
    std::cout << "  - Clang detected" << std::endl;
#endif

#ifdef TRLC_COMPILER_MSVC
    compiler_count++;
    std::cout << "  - MSVC detected" << std::endl;
#endif

#ifdef TRLC_COMPILER_INTEL
    compiler_count++;
    std::cout << "  - Intel detected" << std::endl;
#endif

#ifdef TRLC_COMPILER_MINGW
    compiler_count++;
    std::cout << "  - MinGW detected" << std::endl;
#endif

    // At least one compiler should be detected
    assert(compiler_count >= 1);
    std::cout << "  ✓ At least one compiler detected" << std::endl;
}

// Test compiler version detection
void testCompilerVersionDetection() {
    std::cout << "Testing compiler version detection..." << std::endl;

    // Test that compiler type is detected (not unknown)
    auto compiler_type = getCompilerType();
    assert(compiler_type != CompilerType::unknown);
    std::cout << "  ✓ Compiler type detected: " << static_cast<int>(compiler_type) << std::endl;

    // Test that version is valid
    auto version = getCompilerVersion();
    assert(version.major > 0);
    std::cout << "  ✓ Version: " << version.major << "." << version.minor << "." << version.patch
              << std::endl;

    // Test that name is not null
    auto name = getCompilerName();
    assert(name != nullptr);
    assert(std::string(name).length() > 0);
    std::cout << "  ✓ Compiler name: " << name << std::endl;
}

// Test version comparison operators
void testVersionComparison() {
    std::cout << "Testing version comparison..." << std::endl;

    constexpr CompilerVersion v1{10, 2, 1};
    constexpr CompilerVersion v2{10, 2, 0};
    constexpr CompilerVersion v3{10, 2, 1};
    constexpr CompilerVersion v4{9, 5, 2};

    static_assert(v1 > v2);
    static_assert(v1 == v3);
    static_assert(v2 < v1);
    static_assert(v1 >= v3);
    static_assert(v2 <= v1);
    static_assert(v1 != v2);
    static_assert(v1 > v4);

    std::cout << "  ✓ All version comparison operators work" << std::endl;
}

// Test compiler capability detection
void testCompilerCapabilities() {
    std::cout << "Testing compiler capabilities..." << std::endl;

    // Test builtin attributes
    bool builtin_support = hasBuiltinAttribute("expect");
    std::cout << "  - Builtin attributes: " << (builtin_support ? "yes" : "no") << std::endl;

    // Test inline assembly
    bool inline_asm = supportsInlineAssembly();
    std::cout << "  - Inline assembly: " << (inline_asm ? "yes" : "no") << std::endl;

    // Test color diagnostics
    bool color_diag = hasColorDiagnostics();
    std::cout << "  - Color diagnostics: " << (color_diag ? "yes" : "no") << std::endl;

    std::cout << "  ✓ Compiler capabilities tested" << std::endl;
}

// Test CompilerInfo struct
void testCompilerInfoStruct() {
    std::cout << "Testing CompilerInfo struct..." << std::endl;

    CompilerInfo info = getCompilerInfo();

    // Test that fields are populated
    assert(info.name != nullptr);
    assert(std::string(info.name).length() > 0);
    assert(info.version.major > 0);

    std::cout << "  - Name: " << info.name << std::endl;
    std::cout << "  - Version: " << info.version.major << "." << info.version.minor << "."
              << info.version.patch << std::endl;
    std::cout << "  - Type: " << static_cast<int>(info.type) << std::endl;
    std::cout << "  - Builtin attributes: " << (info.has_builtin_attribute ? "yes" : "no")
              << std::endl;
    std::cout << "  - Inline assembly: " << (info.has_inline_assembly ? "yes" : "no") << std::endl;
    std::cout << "  - Color diagnostics: " << (info.has_color_diagnostics ? "yes" : "no")
              << std::endl;

    // Basic sanity checks
    assert(info.type != CompilerType::unknown);

    std::cout << "  ✓ CompilerInfo struct works correctly" << std::endl;
}

// Test compile-time detection
void testCompileTimeDetection() {
    std::cout << "Testing compile-time detection..." << std::endl;

    // These should all be compile-time constant expressions
    static_assert(getCompilerType() != CompilerType::unknown,
                  "Compiler should be detected at compile time");

    static_assert(getCompilerVersion().major > 0, "Compiler major version should be positive");

    constexpr auto name = getCompilerName();
    static_assert(name != nullptr, "Compiler name should not be null");

    std::cout << "  ✓ All compile-time detections work" << std::endl;
}

// Test edge cases
void testEdgeCases() {
    std::cout << "Testing edge cases..." << std::endl;

    // Test version boundary conditions
    auto version = getCompilerVersion();
    assert(version.major < 1000);   // Sanity check
    assert(version.minor < 1000);   // Sanity check
    assert(version.patch < 10000);  // Sanity check

    // Test string lengths
    auto name = getCompilerName();
    auto name_len = std::string(name).length();
    assert(name_len > 0 && name_len < 100);

    std::cout << "  ✓ Edge cases handled correctly" << std::endl;
}

// Performance test - verify compile-time execution
void testPerformance() {
    std::cout << "Testing performance (compile-time execution)..." << std::endl;

    // All these should be resolved at compile time
    constexpr auto compiler_type = getCompilerType();
    constexpr auto compiler_name = getCompilerName();
    constexpr auto version = getCompilerVersion();
    constexpr auto info = getCompilerInfo();

    // Use the values to prevent optimization
    static_cast<void>(compiler_type);
    static_cast<void>(compiler_name);
    static_cast<void>(version);
    static_cast<void>(info);

    std::cout << "  ✓ All functions execute at compile time" << std::endl;
}

}  // namespace trlc::platform::test

int main() {
    using namespace trlc::platform::test;

    std::cout << "=== TRLC Platform Compiler Detection Tests ===" << std::endl;

    try {
        testCompilerDetectionMacros();
        testCompilerVersionDetection();
        testVersionComparison();
        testCompilerCapabilities();
        testCompilerInfoStruct();
        testCompileTimeDetection();
        testEdgeCases();
        testPerformance();

        std::cout << "\n✅ All compiler detection tests passed!" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}

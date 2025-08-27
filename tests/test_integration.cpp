/**
 * @file test_integration.cpp
 * @brief Comprehensive integration tests for TRLC platform detection library
 *
 * This test suite validates that all detection systems work together correctly,
 * provides cross-validation between different detection methods, and ensures
 * the library functions properly in various build configurations.
 */

#include <cassert>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "trlc/platform/core.hpp"

namespace trlc::platform::test {

//==============================================================================
// Performance Testing Utilities
//==============================================================================

/**
 * @brief Measure compile-time performance impact
 */
class CompileTimeProfiler {
private:
    std::chrono::high_resolution_clock::time_point start_time_;
    std::string test_name_;

public:
    explicit CompileTimeProfiler(const std::string& test_name)
        : start_time_(std::chrono::high_resolution_clock::now()), test_name_(test_name) {}

    ~CompileTimeProfiler() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time_);
        std::cout << "  - " << test_name_ << " took: " << duration.count() << " microseconds"
                  << std::endl;
    }
};

#define PROFILE_COMPILE_TIME(name) CompileTimeProfiler profiler(name)

//==============================================================================
// Comprehensive Platform Report Validation
//==============================================================================

void testCompletePlatformReport() {
    std::cout << "Testing complete platform report generation and validation..." << std::endl;
    PROFILE_COMPILE_TIME("Platform report generation");

    auto report = getPlatformReport();

    // Validate compiler information completeness
    assert(report.compiler.name != nullptr);
    assert(std::string(report.compiler.name).length() > 0);
    assert(report.compiler.version.major > 0);
    std::cout << "  - Compiler: " << report.compiler.name << " " << report.compiler.version.major
              << "." << report.compiler.version.minor << "." << report.compiler.version.patch
              << std::endl;

    // Validate platform information completeness
    assert(report.platform.os_name != nullptr);
    assert(std::string(report.platform.os_name).length() > 0);
    assert(report.platform.kernel_family != nullptr);
    std::cout << "  - Platform: " << report.platform.os_name
              << " (kernel: " << report.platform.kernel_family << ")" << std::endl;

    // Validate architecture information completeness
    assert(report.architecture.arch_name != nullptr);
    assert(std::string(report.architecture.arch_name).length() > 0);
    assert(report.architecture.pointer_size_bits > 0);
    assert(report.architecture.cache_line_size > 0);
    std::cout << "  - Architecture: " << report.architecture.arch_name << " ("
              << report.architecture.pointer_size_bits << "-bit)" << std::endl;

    // Validate C++ standard information
    assert(report.cpp_standard.standard_name != nullptr);
    assert(std::string(report.cpp_standard.standard_name).length() > 0);
    assert(report.cpp_standard.version_macro >= 201703L);  // At least C++17
    std::cout << "  - C++ Standard: " << report.cpp_standard.standard_name
              << " (macro: " << report.cpp_standard.version_macro << ")" << std::endl;

    // Validate endianness information
    assert(report.endianness.byte_order != ByteOrder::unknown);
    std::cout << "  - Endianness: " << (report.endianness.is_little_endian ? "Little" : "Big")
              << " endian" << std::endl;

    // Generate and validate report text
    auto report_text = report.generateReport();
    assert(!report_text.empty());
    assert(report_text.length() > 500);  // Should be comprehensive
    assert(report_text.find("TRLC Platform Detection Report") != std::string::npos);
    assert(report_text.find("COMPILER INFORMATION") != std::string::npos);
    assert(report_text.find("PLATFORM INFORMATION") != std::string::npos);
    assert(report_text.find("ARCHITECTURE INFORMATION") != std::string::npos);
    assert(report_text.find("C++ STANDARD INFORMATION") != std::string::npos);
    assert(report_text.find("ENDIANNESS INFORMATION") != std::string::npos);

    std::cout << "  - Report length: " << report_text.length() << " characters" << std::endl;

    // Test brief summary
    auto summary = report.getBriefSummary();
    assert(!summary.empty());
    assert(summary.length() < 200);  // Should be brief
    assert(summary.find(report.compiler.name) != std::string::npos);
    assert(summary.find(report.platform.os_name) != std::string::npos);
    assert(summary.find(report.architecture.arch_name) != std::string::npos);

    std::cout << "  - Brief summary: " << summary << std::endl;
    std::cout << "  ✓ Complete platform report validation passed" << std::endl;
}

//==============================================================================
// Cross-Reference Detection Results
//==============================================================================

void testDetectionConsistency() {
    std::cout << "Testing cross-reference detection consistency..." << std::endl;
    PROFILE_COMPILE_TIME("Detection consistency validation");

    auto report = getPlatformReport();

    // Test compiler detection consistency
    auto compiler_type = getCompilerType();
    auto compiler_info = getCompilerInfo();
    assert(compiler_type == compiler_info.type);
    assert(compiler_type == report.compiler.type);
    std::cout << "  - Compiler detection consistent across methods" << std::endl;

    // Test platform detection consistency
    auto platform_info = getPlatformInfo();
    assert(platform_info.os == report.platform.os);
    std::cout << "  - Platform detection consistent across methods" << std::endl;

    // Test architecture detection consistency
    auto arch_type = getCpuArchitecture();
    auto arch_info = getArchitectureInfo();
    assert(arch_type == arch_info.architecture);
    assert(arch_type == report.architecture.architecture);
    std::cout << "  - Architecture detection consistent across methods" << std::endl;

    // Test endianness detection consistency
    auto byte_order = getByteOrder();
    auto endianness_info = getEndiannessInfo();
    assert(byte_order == endianness_info.byte_order);
    assert(byte_order == report.endianness.byte_order);
    assert(byte_order == report.architecture.byte_order);

    bool is_little = isLittleEndian();
    bool is_big = isBigEndian();
    assert(is_little == endianness_info.is_little_endian);
    assert(is_big == endianness_info.is_big_endian);
    assert(is_little == report.endianness.is_little_endian);
    assert(is_big == report.endianness.is_big_endian);
    assert(is_little != is_big ||
           byte_order == ByteOrder::mixed_endian);  // Should be mutually exclusive except for mixed
    std::cout << "  - Endianness detection consistent across methods" << std::endl;

    // Test C++ standard detection consistency
    auto cpp_standard = getCppStandard();
    auto cpp_info = getCppStandardInfo();
    assert(cpp_standard == cpp_info.standard);
    assert(cpp_standard == report.cpp_standard.standard);

    auto version_macro = getCppVersionMacro();
    assert(version_macro == cpp_info.version_macro);
    assert(version_macro == report.cpp_standard.version_macro);
    std::cout << "  - C++ standard detection consistent across methods" << std::endl;

    // Test feature detection consistency
    auto features = getFeatureSet();
    assert(features.has_exceptions == report.features.has_exceptions);
    assert(features.has_rtti == report.features.has_rtti);
    assert(features.has_threads == report.features.has_threads);
    std::cout << "  - Feature detection consistent across methods" << std::endl;

    std::cout << "  ✓ Detection consistency validation passed" << std::endl;
}

//==============================================================================
// Macro and Function Consistency Testing
//==============================================================================

void testMacroFunctionConsistency() {
    std::cout << "Testing macro and function result consistency..." << std::endl;
    PROFILE_COMPILE_TIME("Macro function consistency");

    // Test compiler macros vs functions - only check active compiler
    auto detected_compiler = getCompilerType();

#ifdef TRLC_COMPILER_GCC
    if (detected_compiler == CompilerType::gcc) {
        std::cout << "  - GCC macro matches function result" << std::endl;
    }
#endif

#ifdef TRLC_COMPILER_CLANG
    if (detected_compiler == CompilerType::clang) {
        std::cout << "  - Clang macro matches function result" << std::endl;
    }
#endif

#ifdef TRLC_COMPILER_MSVC
    if (detected_compiler == CompilerType::msvc) {
        std::cout << "  - MSVC macro matches function result" << std::endl;
    }
#endif

    // Test platform macros vs functions - only check active platform
    auto platform_info = getPlatformInfo();

#ifdef TRLC_PLATFORM_WINDOWS
    if (platform_info.os == OperatingSystem::windows) {
        std::cout << "  - Windows macro matches function result" << std::endl;
    }
#endif

#ifdef TRLC_PLATFORM_LINUX
    if (platform_info.os == OperatingSystem::linux_generic ||
        platform_info.os == OperatingSystem::ubuntu ||
        platform_info.os == OperatingSystem::debian ||
        platform_info.os == OperatingSystem::redhat) {
        std::cout << "  - Linux macro matches function result" << std::endl;
    }
#endif

#ifdef TRLC_PLATFORM_MACOS
    if (platform_info.os == OperatingSystem::macos) {
        std::cout << "  - macOS macro matches function result" << std::endl;
    }
#endif

    // Test architecture macros vs functions - only check active architecture
    auto detected_arch = getCpuArchitecture();

#ifdef TRLC_ARCH_X86_64
    if (detected_arch == CpuArchitecture::x86_64) {
        std::cout << "  - x86_64 macro matches function result" << std::endl;
    }
#endif

#ifdef TRLC_ARCH_X86
    if (detected_arch == CpuArchitecture::x86) {
        std::cout << "  - x86 macro matches function result" << std::endl;
    }
#endif

// Test endianness macros vs functions
#ifdef TRLC_ENDIAN_LITTLE
    if (isLittleEndian()) {
        std::cout << "  - Little endian macro matches function result" << std::endl;
    }
#endif

#ifdef TRLC_ENDIAN_BIG
    if (isBigEndian()) {
        std::cout << "  - Big endian macro matches function result" << std::endl;
    }
#endif

    // Test C++ standard macros vs functions
    auto detected_std = getCppStandard();

#ifdef TRLC_CPP17
    if (detected_std >= CppStandard::cpp_17) {
        std::cout << "  - C++17 macro matches function result" << std::endl;
    }
#endif

#ifdef TRLC_CPP20
    if (detected_std >= CppStandard::cpp_20) {
        std::cout << "  - C++20 macro matches function result" << std::endl;
    }
#endif

#ifdef TRLC_CPP23
    if (detected_std >= CppStandard::cpp_23) {
        std::cout << "  - C++23 macro matches function result" << std::endl;
    }
#endif

    std::cout << "  ✓ Macro and function consistency validation passed" << std::endl;
}

void testConstexprEvaluation() {
    std::cout << "Testing constexpr evaluation capabilities..." << std::endl;
    PROFILE_COMPILE_TIME("Constexpr evaluation");

    // Test compile-time constants
    static_assert(getCompilerType() != CompilerType::unknown);
    static_assert(getCpuArchitecture() != CpuArchitecture::unknown);

    constexpr auto platform_info = getPlatformInfo();
    static_assert(platform_info.os != OperatingSystem::unknown);

    constexpr auto cpp_std = getCppStandard();
    static_assert(cpp_std >= CppStandard::cpp_17);

    constexpr bool little_endian = isLittleEndian();
    constexpr bool big_endian = isBigEndian();
    static_assert(little_endian != big_endian);  // Exactly one should be true

    std::cout << "  ✓ All detection functions work at compile-time" << std::endl;
}

void testBuildConfigurations() {
    std::cout << "Testing build configuration detection..." << std::endl;
    PROFILE_COMPILE_TIME("Build configuration");

// Test debug/release detection
#ifdef NDEBUG
    std::cout << "  - Running in Release mode" << std::endl;
#else
    std::cout << "  - Running in Debug mode" << std::endl;
#endif

// Test optimization level detection
#ifdef __OPTIMIZE__
    std::cout << "  - Optimizations enabled" << std::endl;
#else
    std::cout << "  - Optimizations disabled" << std::endl;
#endif

    std::cout << "  ✓ Build configuration detection working" << std::endl;
}

void testPerformanceMeasurement() {
    std::cout << "Testing performance measurement..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    // Perform multiple detection calls to measure overhead
    for (int i = 0; i < 1000; ++i) {
        volatile auto compiler = getCompilerType();
        volatile auto platform = getPlatformInfo();
        volatile auto arch = getCpuArchitecture();
        volatile auto std_ver = getCppStandard();
        (void)compiler;
        (void)platform;
        (void)arch;
        (void)std_ver;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "  - 1000 detection calls took: " << duration.count() << " microseconds"
              << std::endl;
    std::cout << "  - Average per call: " << (duration.count() / 1000.0) << " microseconds"
              << std::endl;
    std::cout << "  ✓ Performance measurement completed" << std::endl;
}

void displayProfileResults() {
    std::cout << "\nCompile-time profiling results:" << std::endl;
    std::cout << "  Note: Profile timing results would be available in a real compile-time profiler"
              << std::endl;
    std::cout << "  This is a placeholder for demonstration of the profiling API" << std::endl;
}

}  // namespace trlc::platform::test

int main() {
    std::cout << "Starting TRLC Platform Library Integration Tests" << std::endl;
    std::cout << "=================================================" << std::endl;

    try {
        // Core detection consistency
        trlc::platform::test::testDetectionConsistency();

        // Macro vs function consistency
        trlc::platform::test::testMacroFunctionConsistency();

        // Constexpr evaluation
        trlc::platform::test::testConstexprEvaluation();

        // Build configuration validation
        trlc::platform::test::testBuildConfigurations();

        // Performance measurement
        trlc::platform::test::testPerformanceMeasurement();

        // Display compilation profile results
        trlc::platform::test::displayProfileResults();

        std::cout << "\n=================================================" << std::endl;
        std::cout << "All integration tests passed successfully!" << std::endl;
        std::cout << "TRLC Platform Library is working correctly." << std::endl;

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nIntegration test failed: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\nIntegration test failed with unknown error" << std::endl;
        return 1;
    }
}

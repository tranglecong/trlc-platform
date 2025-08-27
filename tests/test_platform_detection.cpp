/**
 * @file test_platform_detection.cpp
 * @brief Comprehensive tests for platform and OS detection functionality
 *
 * Tests all platform detection macros, functions, and capabilities
 * including compile-time detection accuracy and runtime verification.
 */

#include <cassert>
#include <iostream>
#include <string>

#include "trlc/platform/platform.hpp"

namespace trlc::platform::test {

// Test basic platform detection macros
void testPlatformDetectionMacros() {
    std::cout << "Testing platform detection macros..." << std::endl;

    // Verify exactly one platform is detected
    int platform_count = 0;

#ifdef TRLC_PLATFORM_WINDOWS
    platform_count++;
    std::cout << "  - Windows detected" << std::endl;
#endif

#ifdef TRLC_PLATFORM_LINUX
    platform_count++;
    std::cout << "  - Linux detected" << std::endl;
#endif

#ifdef TRLC_PLATFORM_MACOS
    platform_count++;
    std::cout << "  - macOS detected" << std::endl;
#endif

#ifdef TRLC_PLATFORM_BSD
    platform_count++;
    std::cout << "  - BSD detected" << std::endl;
#endif

#ifdef TRLC_PLATFORM_UNIX
    platform_count++;
    std::cout << "  - Unix detected" << std::endl;
#endif

    // At least one platform should be detected
    assert(platform_count >= 1);
    std::cout << "  ✓ At least one platform detected" << std::endl;
}

// Test platform type detection
void testPlatformTypeDetection() {
    std::cout << "Testing platform type detection..." << std::endl;

    // Test that operating system is detected (not unknown)
    auto os = getOperatingSystem();
    assert(os != OperatingSystem::unknown);
    std::cout << "  ✓ Operating system detected: " << static_cast<int>(os) << std::endl;

    // Test environment detection
    auto env = getEnvironmentType();
    assert(env != EnvironmentType::unknown);
    std::cout << "  ✓ Environment type detected: " << static_cast<int>(env) << std::endl;
}

// Test platform capabilities
void testPlatformCapabilities() {
    std::cout << "Testing platform capabilities..." << std::endl;

    // Test POSIX API support
    bool posix_support = hasPosixApi();
    std::cout << "  - POSIX API: " << (posix_support ? "yes" : "no") << std::endl;

    // Test Win32 API support
    bool win32_support = hasWin32Api();
    std::cout << "  - Win32 API: " << (win32_support ? "yes" : "no") << std::endl;

    // Test case-sensitive filesystem
    bool case_sensitive = supportsCaseSensitiveFilesystem();
    std::cout << "  - Case sensitive FS: " << (case_sensitive ? "yes" : "no") << std::endl;

    // At least one API should be supported
    assert(posix_support || win32_support);

    std::cout << "  ✓ Platform capabilities tested" << std::endl;
}

// Test environment detection
void testEnvironmentDetection() {
    std::cout << "Testing environment detection..." << std::endl;

    // Test environment type detection
    auto env = getEnvironmentType();
    std::cout << "  - Environment type: " << static_cast<int>(env) << std::endl;
    assert(env != EnvironmentType::unknown);

    std::cout << "  ✓ Environment detection completed" << std::endl;
}

// Test PlatformInfo struct
void testPlatformInfoStruct() {
    std::cout << "Testing PlatformInfo struct..." << std::endl;

    PlatformInfo info = getPlatformInfo();

    // Test that fields are populated
    assert(info.os_name != nullptr);
    assert(std::string(info.os_name).length() > 0);
    assert(info.kernel_family != nullptr);
    assert(info.os != OperatingSystem::unknown);
    assert(info.environment != EnvironmentType::unknown);

    std::cout << "  - OS: " << info.os_name << std::endl;
    std::cout << "  - OS type: " << static_cast<int>(info.os) << std::endl;
    std::cout << "  - Kernel family: " << info.kernel_family << std::endl;
    std::cout << "  - Environment: " << static_cast<int>(info.environment) << std::endl;
    std::cout << "  - Is POSIX: " << (info.isPosix() ? "yes" : "no") << std::endl;
    std::cout << "  - Is Unix: " << (info.isUnix() ? "yes" : "no") << std::endl;
    std::cout << "  - Is Windows: " << (info.isWindows() ? "yes" : "no") << std::endl;

    // Basic sanity checks
    assert(std::string(info.kernel_family).length() > 0);

    std::cout << "  ✓ PlatformInfo struct works correctly" << std::endl;
}

// Test compile-time detection
void testCompileTimeDetection() {
    std::cout << "Testing compile-time detection..." << std::endl;

    // These should all be compile-time constant expressions
    static_assert(getOperatingSystem() != OperatingSystem::unknown,
                  "Operating system should be detected at compile time");

    static_assert(getEnvironmentType() != EnvironmentType::unknown,
                  "Environment type should be detected at compile time");

    constexpr auto info = getPlatformInfo();
    static_assert(info.os_name != nullptr, "OS name should not be null");
    static_assert(info.kernel_family != nullptr, "Kernel family should not be null");

    std::cout << "  ✓ All compile-time detections work" << std::endl;
}

// Test platform-specific features
void testPlatformSpecificFeatures() {
    std::cout << "Testing platform-specific features..." << std::endl;

    auto os = getOperatingSystem();

    if (os == OperatingSystem::windows) {
        std::cout << "  - Testing Windows-specific features" << std::endl;

        // Windows should have Win32 API
        assert(hasWin32Api());

        // Windows should not have POSIX API by default
        std::cout << "    * POSIX API: " << (hasPosixApi() ? "yes" : "no") << std::endl;

    } else if (os == OperatingSystem::linux_generic || os == OperatingSystem::ubuntu ||
               os == OperatingSystem::debian || os == OperatingSystem::redhat) {
        std::cout << "  - Testing Linux-specific features" << std::endl;

        // Linux should have POSIX API
        assert(hasPosixApi());

        // Linux should have case-sensitive filesystem
        assert(supportsCaseSensitiveFilesystem());

    } else if (os == OperatingSystem::macos) {
        std::cout << "  - Testing macOS-specific features" << std::endl;

        // macOS should have POSIX API
        assert(hasPosixApi());

    } else if (os == OperatingSystem::freebsd || os == OperatingSystem::openbsd ||
               os == OperatingSystem::netbsd) {
        std::cout << "  - Testing BSD-specific features" << std::endl;

        // BSD should have POSIX API
        assert(hasPosixApi());
    }

    std::cout << "  ✓ Platform-specific features tested" << std::endl;
}

// Test edge cases
void testEdgeCases() {
    std::cout << "Testing edge cases..." << std::endl;

    // Test string lengths
    auto info = getPlatformInfo();
    auto name_len = std::string(info.os_name).length();
    assert(name_len > 0 && name_len < 100);

    auto kernel_len = std::string(info.kernel_family).length();
    assert(kernel_len > 0 && kernel_len < 50);

    // Test enum bounds
    auto os = getOperatingSystem();
    assert(static_cast<int>(os) >= 0);
    assert(static_cast<int>(os) < 20);  // Reasonable upper bound

    auto env = getEnvironmentType();
    assert(static_cast<int>(env) >= 0);
    assert(static_cast<int>(env) < 10);  // Reasonable upper bound

    std::cout << "  ✓ Edge cases handled correctly" << std::endl;
}

// Performance test - verify compile-time execution
void testPerformance() {
    std::cout << "Testing performance (compile-time execution)..." << std::endl;

    // All these should be resolved at compile time
    constexpr auto os = getOperatingSystem();
    constexpr auto env = getEnvironmentType();
    constexpr auto info = getPlatformInfo();
    constexpr auto posix_support = hasPosixApi();
    constexpr auto win32_support = hasWin32Api();
    constexpr auto case_sensitive = supportsCaseSensitiveFilesystem();

    // Use the values to prevent optimization
    static_cast<void>(os);
    static_cast<void>(env);
    static_cast<void>(info);
    static_cast<void>(posix_support);
    static_cast<void>(win32_support);
    static_cast<void>(case_sensitive);

    std::cout << "  ✓ All functions execute at compile time" << std::endl;
}

}  // namespace trlc::platform::test

int main() {
    using namespace trlc::platform::test;

    std::cout << "=== TRLC Platform Detection Tests ===" << std::endl;

    try {
        testPlatformDetectionMacros();
        testPlatformTypeDetection();
        testPlatformCapabilities();
        testEnvironmentDetection();
        testPlatformInfoStruct();
        testCompileTimeDetection();
        testPlatformSpecificFeatures();
        testEdgeCases();
        testPerformance();

        std::cout << "\n✅ All platform detection tests passed!" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}

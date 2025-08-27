/**
 * @file test_architecture_detection.cpp
 * @brief Comprehensive tests for CPU architecture detection functionality
 *
 * Tests all architecture detection macros, functions, and capabilities
 * including compile-time detection accuracy and runtime verification.
 */

#include <cassert>
#include <iostream>
#include <string>

#include "trlc/platform/architecture.hpp"

namespace trlc::platform::test {

// Test basic architecture detection macros
void testArchitectureDetectionMacros() {
    std::cout << "Testing architecture detection macros..." << std::endl;

    // Verify exactly one architecture is detected
    int arch_count = 0;

#ifdef TRLC_ARCH_X86
    arch_count++;
    std::cout << "  - x86 detected" << std::endl;
#endif

#ifdef TRLC_ARCH_X86_64
    arch_count++;
    std::cout << "  - x86_64 detected" << std::endl;
#endif

#ifdef TRLC_ARCH_ARM
    arch_count++;
    std::cout << "  - ARM detected" << std::endl;
#endif

#ifdef TRLC_ARCH_ARM64
    arch_count++;
    std::cout << "  - ARM64 detected" << std::endl;
#endif

#ifdef TRLC_ARCH_MIPS
    arch_count++;
    std::cout << "  - MIPS detected" << std::endl;
#endif

#ifdef TRLC_ARCH_POWERPC
    arch_count++;
    std::cout << "  - PowerPC detected" << std::endl;
#endif

    // At least one architecture should be detected
    assert(arch_count >= 1);
    std::cout << "  ✓ At least one architecture detected" << std::endl;
}

// Test architecture type detection
void testArchitectureTypeDetection() {
    std::cout << "Testing architecture type detection..." << std::endl;

    // Test that architecture is detected (not unknown)
    auto arch = getCpuArchitecture();
    assert(arch != CpuArchitecture::unknown);
    std::cout << "  ✓ Architecture detected: " << static_cast<int>(arch) << std::endl;

    // Test architecture name
    auto name = detail::getArchitectureName();
    assert(name != nullptr);
    assert(std::string(name).length() > 0);
    std::cout << "  ✓ Architecture name: " << name << std::endl;
}

// Test pointer size detection
void testPointerSizeDetection() {
    std::cout << "Testing pointer size detection..." << std::endl;

    auto pointer_size = getPointerSize();
    std::cout << "  - Pointer size: " << pointer_size << " bits" << std::endl;

    // Should be 32 or 64 bits on most platforms
    assert(pointer_size == 32 || pointer_size == 64);

    // Test 64-bit detection
    bool is_64bit = is64BitArchitecture();
    std::cout << "  - Is 64-bit: " << (is_64bit ? "yes" : "no") << std::endl;

    // Pointer size should match 64-bit detection
    if (is_64bit) {
        assert(pointer_size == 64);
    } else {
        assert(pointer_size == 32);
    }

    std::cout << "  ✓ Pointer size detection works correctly" << std::endl;
}

// Test endianness detection
void testEndiannessDetection() {
    std::cout << "Testing endianness detection..." << std::endl;

    auto byte_order = getByteOrder();
    std::cout << "  - Byte order: " << static_cast<int>(byte_order) << std::endl;
    assert(byte_order != ByteOrder::unknown);

    bool is_little = isLittleEndian();
    bool is_big = isBigEndian();

    std::cout << "  - Little endian: " << (is_little ? "yes" : "no") << std::endl;
    std::cout << "  - Big endian: " << (is_big ? "yes" : "no") << std::endl;

    // Exactly one should be true (excluding mixed endian)
    if (byte_order != ByteOrder::mixed_endian) {
        assert(is_little != is_big);
    }

    // Should match byte order
    if (is_little) {
        assert(byte_order == ByteOrder::little_endian);
    } else if (is_big) {
        assert(byte_order == ByteOrder::big_endian);
    }

    std::cout << "  ✓ Endianness detection works correctly" << std::endl;
}

// Test SIMD capabilities
void testSimdCapabilities() {
    std::cout << "Testing SIMD capabilities..." << std::endl;

    bool has_simd = hasSimdSupport();
    bool has_vector = hasVectorInstructions();
    bool cache_align = supportsCacheLineAlignment();

    std::cout << "  - SIMD support: " << (has_simd ? "yes" : "no") << std::endl;
    std::cout << "  - Vector instructions: " << (has_vector ? "yes" : "no") << std::endl;
    std::cout << "  - Cache line alignment: " << (cache_align ? "yes" : "no") << std::endl;

    // Vector instructions imply SIMD support
    if (has_vector) {
        assert(has_simd);
    }

    std::cout << "  ✓ SIMD capabilities tested" << std::endl;
}

// Test ArchitectureInfo struct
void testArchitectureInfoStruct() {
    std::cout << "Testing ArchitectureInfo struct..." << std::endl;

    ArchitectureInfo info = getArchitectureInfo();

    // Test that fields are populated
    assert(info.arch_name != nullptr);
    assert(std::string(info.arch_name).length() > 0);
    assert(info.architecture != CpuArchitecture::unknown);
    assert(info.pointer_size_bits > 0);
    assert(info.byte_order != ByteOrder::unknown);
    assert(info.cache_line_size > 0);

    std::cout << "  - Name: " << info.arch_name << std::endl;
    std::cout << "  - Architecture: " << static_cast<int>(info.architecture) << std::endl;
    std::cout << "  - Pointer size: " << info.pointer_size_bits << " bits" << std::endl;
    std::cout << "  - Byte order: " << static_cast<int>(info.byte_order) << std::endl;
    std::cout << "  - Cache line size: " << info.cache_line_size << " bytes" << std::endl;

    // Test methods
    std::cout << "  - Is 64-bit: " << (info.is64Bit() ? "yes" : "no") << std::endl;
    std::cout << "  - Is 32-bit: " << (info.is32Bit() ? "yes" : "no") << std::endl;
    std::cout << "  - Is little endian: " << (info.isLittleEndian() ? "yes" : "no") << std::endl;
    std::cout << "  - Is X86: " << (info.isX86() ? "yes" : "no") << std::endl;
    std::cout << "  - Is ARM: " << (info.isARM() ? "yes" : "no") << std::endl;
    std::cout << "  - Supports unaligned access: "
              << (info.supportsUnalignedAccess() ? "yes" : "no") << std::endl;
    std::cout << "  - Has SIMD: " << (info.hasSimdSupport() ? "yes" : "no") << std::endl;

    // Basic sanity checks
    assert(info.pointer_size_bits == 32 || info.pointer_size_bits == 64);
    assert(info.is64Bit() != info.is32Bit());
    assert(info.cache_line_size >= 16 && info.cache_line_size <= 256);

    std::cout << "  ✓ ArchitectureInfo struct works correctly" << std::endl;
}

// Test compile-time detection
void testCompileTimeDetection() {
    std::cout << "Testing compile-time detection..." << std::endl;

    // These should all be compile-time constant expressions
    static_assert(getCpuArchitecture() != CpuArchitecture::unknown,
                  "Architecture should be detected at compile time");

    static_assert(getPointerSize() > 0, "Pointer size should be positive");

    static_assert(getByteOrder() != ByteOrder::unknown, "Byte order should be detected");

    constexpr auto name = detail::getArchitectureName();
    static_assert(name != nullptr, "Architecture name should not be null");

    std::cout << "  ✓ All compile-time detections work" << std::endl;
}

// Test architecture-specific features
void testArchitectureSpecificFeatures() {
    std::cout << "Testing architecture-specific features..." << std::endl;

    auto arch = getCpuArchitecture();

    if (arch == CpuArchitecture::x86 || arch == CpuArchitecture::x86_64) {
        std::cout << "  - Testing x86/x64-specific features" << std::endl;

        // x86/x64 should be little endian
        assert(isLittleEndian());

        // x64 should be 64-bit, x86 should be 32-bit
        if (arch == CpuArchitecture::x86_64) {
            assert(is64BitArchitecture());
            assert(getPointerSize() == 64);
        } else {
            assert(is32BitArchitecture());
            assert(getPointerSize() == 32);
        }

        // Should support SIMD
        assert(hasSimdSupport());

    } else if (arch == CpuArchitecture::arm_v6 || arch == CpuArchitecture::arm_v7 ||
               arch == CpuArchitecture::arm_v8_32 || arch == CpuArchitecture::arm_v8_64) {
        std::cout << "  - Testing ARM-specific features" << std::endl;

        // ARM64 should be 64-bit, others should be 32-bit
        if (arch == CpuArchitecture::arm_v8_64) {
            assert(is64BitArchitecture());
            assert(getPointerSize() == 64);
        } else {
            assert(is32BitArchitecture());
            assert(getPointerSize() == 32);
        }

        // Modern ARM is typically little endian
        std::cout << "    * Endianness: " << (isLittleEndian() ? "little" : "big") << std::endl;

    } else {
        std::cout << "  - Testing other architecture: " << detail::getArchitectureName()
                  << std::endl;
    }

    std::cout << "  ✓ Architecture-specific features tested" << std::endl;
}

// Test edge cases
void testEdgeCases() {
    std::cout << "Testing edge cases..." << std::endl;

    // Test string lengths
    auto name = detail::getArchitectureName();
    auto name_len = std::string(name).length();
    assert(name_len > 0 && name_len < 50);

    // Test enum bounds
    auto arch = getCpuArchitecture();
    assert(static_cast<int>(arch) >= 0);
    assert(static_cast<int>(arch) < 20);  // Reasonable upper bound

    auto byte_order = getByteOrder();
    assert(static_cast<int>(byte_order) >= 0);
    assert(static_cast<int>(byte_order) < 10);  // Reasonable upper bound

    // Test pointer size bounds
    auto pointer_size = getPointerSize();
    assert(pointer_size >= 8 && pointer_size <= 128);

    std::cout << "  ✓ Edge cases handled correctly" << std::endl;
}

// Performance test - verify compile-time execution
void testPerformance() {
    std::cout << "Testing performance (compile-time execution)..." << std::endl;

    // All these should be resolved at compile time
    constexpr auto arch = getCpuArchitecture();
    constexpr auto name = detail::getArchitectureName();
    constexpr auto pointer_size = getPointerSize();
    constexpr auto byte_order = getByteOrder();
    constexpr auto is_64bit = is64BitArchitecture();
    constexpr auto is_little = isLittleEndian();
    constexpr auto is_big = isBigEndian();
    constexpr auto has_simd = hasSimdSupport();
    constexpr auto has_vector = hasVectorInstructions();
    constexpr auto info = getArchitectureInfo();

    // Use the values to prevent optimization
    static_cast<void>(arch);
    static_cast<void>(name);
    static_cast<void>(pointer_size);
    static_cast<void>(byte_order);
    static_cast<void>(is_64bit);
    static_cast<void>(is_little);
    static_cast<void>(is_big);
    static_cast<void>(has_simd);
    static_cast<void>(has_vector);
    static_cast<void>(info);

    std::cout << "  ✓ All functions execute at compile time" << std::endl;
}

}  // namespace trlc::platform::test

int main() {
    using namespace trlc::platform::test;

    std::cout << "=== TRLC Platform Architecture Detection Tests ===" << std::endl;

    try {
        testArchitectureDetectionMacros();
        testArchitectureTypeDetection();
        testPointerSizeDetection();
        testEndiannessDetection();
        testSimdCapabilities();
        testArchitectureInfoStruct();
        testCompileTimeDetection();
        testArchitectureSpecificFeatures();
        testEdgeCases();
        testPerformance();

        std::cout << "\n✅ All architecture detection tests passed!" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}

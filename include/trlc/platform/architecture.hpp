/**
 * @file architecture.hpp
 * @brief CPU architecture and byte order detection for trlc-platform
 *
 * @details This header provides comprehensive compile-time detection of CPU
 * architecture, byte order, and architecture-specific capabilities. All functions
 * are constexpr and evaluate at compile time for zero runtime overhead.
 *
 * ## Features
 * - Compile-time CPU architecture detection (x86, ARM, MIPS, PowerPC, RISC-V, SPARC)
 * - Pointer size detection (32-bit vs 64-bit)
 * - Cache line size estimation for optimization
 * - SIMD and vector instruction capability detection
 * - Architecture family classification (ARM vs x86 vs others)
 * - Preprocessor macros for conditional compilation
 *
 * ## Supported Architectures
 * - **x86 Family**: x86 (32-bit), x86_64 (64-bit)
 * - **ARM Family**: ARMv6, ARMv7, ARMv8 (32/64-bit)
 * - **MIPS Family**: MIPS (32/64-bit)
 * - **PowerPC Family**: PowerPC (32/64-bit)
 * - **RISC-V Family**: RISC-V (32/64-bit)
 * - **SPARC Family**: SPARC (32/64-bit)
 *
 * ## Performance Characteristics
 * - **Zero Runtime Overhead**: All detection occurs at compile time
 * - **Header-Only**: No linking required
 * - **Thread-Safe**: Purely compile-time evaluation
 * - **Optimizable**: Enables compiler optimizations through constexpr
 *
 * ## Thread Safety
 * All functions in this header are thread-safe as they perform only compile-time
 * evaluation and access no mutable state.
 *
 * @example Basic architecture detection
 * @code
 * #include "trlc/platform/architecture.hpp"
 *
 * constexpr auto arch = trlc::platform::getCpuArchitecture();
 * if constexpr (arch == trlc::platform::CpuArchitecture::x86_64) {
 *     // x86_64 specific optimizations
 * } else if constexpr (trlc::platform::is64BitArchitecture()) {
 *     // Generic 64-bit optimizations
 * }
 * @endcode
 *
 * @example SIMD capability detection
 * @code
 * if constexpr (trlc::platform::hasSimdSupport()) {
 *     // Use SIMD optimized algorithms
 *     process_with_simd(data);
 * } else {
 *     // Fallback to scalar implementation
 *     process_scalar(data);
 * }
 * @endcode
 *
 * @author TRLC Platform Team
 * @version 1.0.0
 * @since 1.0.0
 * @see endianness.hpp for byte order detection functions
 * @see features.hpp for runtime feature detection
 */
#pragma once

#include <cstddef>  // For size_t

#include "endianness.hpp"  // For ByteOrder enum and endianness functions

namespace trlc {
namespace platform {

/**
 * @brief CPU architecture enumeration
 *
 * @details Represents different CPU architectures that can be detected at compile time.
 * This enumeration covers all major CPU architecture families used in modern computing
 * from embedded systems to high-performance servers.
 *
 * ## Architecture Families
 *
 * ### x86 Family
 * - **x86**: Legacy 32-bit Intel/AMD processors
 * - **x86_64**: Modern 64-bit Intel/AMD processors (also known as AMD64)
 *
 * ### ARM Family
 * - **arm_v6**: ARM Architecture v6 (older embedded systems)
 * - **arm_v7**: ARM Architecture v7 (common in mobile devices)
 * - **arm_v8_32**: AArch32 mode of ARM v8 (32-bit execution)
 * - **arm_v8_64**: AArch64 mode of ARM v8 (64-bit execution, ARM64)
 *
 * ### Other Architectures
 * - **MIPS**: Popular in embedded systems and older workstations
 * - **PowerPC**: Used in older Apple computers and embedded systems
 * - **RISC-V**: Open-source architecture gaining popularity
 * - **SPARC**: Oracle/Sun workstations and servers
 *
 * @note The architecture detection is performed at compile time using compiler
 * predefined macros, ensuring zero runtime overhead.
 *
 * @see getCpuArchitecture() for detection function
 * @see ArchitectureInfo for comprehensive architecture information
 * @since 1.0.0
 */
enum class CpuArchitecture : int {
    unknown = 0,  ///< Unknown or unsupported architecture
    x86,          ///< Intel/AMD x86 (32-bit) - Legacy desktop/server processors
    x86_64,       ///< Intel/AMD x86-64 (64-bit) - Modern desktop/server processors
    arm_v6,       ///< ARM Architecture v6 - Older embedded systems (e.g., Raspberry Pi 1)
    arm_v7,       ///< ARM Architecture v7 - Mobile devices, embedded systems
    arm_v8_32,    ///< AArch32 (ARM v8 32-bit mode) - Modern ARM in 32-bit mode
    arm_v8_64,    ///< AArch64 (ARM v8 64-bit mode / ARM64) - Modern mobile, Apple Silicon
    mips,         ///< MIPS (32-bit) - Embedded systems, older workstations
    mips_64,      ///< MIPS64 (64-bit) - High-performance MIPS systems
    powerpc,      ///< PowerPC (32-bit) - Legacy Apple computers, embedded systems
    powerpc_64,   ///< PowerPC64 (64-bit) - High-performance PowerPC systems
    risc_v_32,    ///< RISC-V (32-bit) - Open-source architecture, embedded systems
    risc_v_64,    ///< RISC-V (64-bit) - Open-source architecture, future systems
    sparc,        ///< SPARC (32-bit) - Legacy Sun/Oracle workstations
    sparc_64      ///< SPARC64 (64-bit) - Oracle/Sun servers and workstations
};

/**
 * @brief Architecture information structure
 *
 * @details Contains comprehensive information about the detected CPU architecture,
 * including architecture type, byte order, and capability information. This structure
 * provides a unified view of the system's architectural characteristics and includes
 * convenient methods for common architectural queries.
 *
 * ## Key Information
 * - **Architecture Type**: Specific CPU architecture family and variant
 * - **Byte Order**: System endianness (little/big endian)
 * - **Pointer Size**: Address space size (32-bit vs 64-bit)
 * - **Cache Line Size**: Typical cache line size for alignment optimization
 * - **Architecture Name**: Human-readable architecture identifier
 *
 * ## Performance Implications
 * - **Cache Line Alignment**: Use `cache_line_size` for optimal memory layout
 * - **SIMD Support**: Check `hasSimdSupport()` for vectorization opportunities
 * - **Unaligned Access**: Use `supportsUnalignedAccess()` for memory access patterns
 *
 * @example Architecture-specific optimization
 * @code
 * constexpr auto arch_info = trlc::platform::getArchitectureInfo();
 *
 * // Optimize based on architecture capabilities
 * if constexpr (arch_info.hasSimdSupport()) {
 *     // Use SIMD instructions
 *     processWithSimd(data);
 * } else {
 *     // Use scalar fallback
 *     processScalar(data);
 * }
 *
 * // Align data to cache line boundary
 * alignas(arch_info.cache_line_size) char buffer[1024];
 * @endcode
 *
 * @note All member functions are constexpr and evaluate at compile time
 * @see getCpuArchitecture() for simple architecture detection
 * @see getArchitectureInfo() for creating instances of this structure
 * @since 1.0.0
 */
struct ArchitectureInfo {
    CpuArchitecture architecture;  ///< Detected CPU architecture type
    ByteOrder byte_order;          ///< Detected byte order (endianness)
    int pointer_size_bits;         ///< Pointer size in bits (32 or 64)
    size_t cache_line_size;        ///< Typical cache line size in bytes
    const char* arch_name;         ///< Human-readable architecture name

    /**
     * @brief Checks if the architecture is 64-bit
     *
     * @details Determines whether the target architecture uses 64-bit pointers
     * and address space. This is useful for selecting architecture-specific
     * optimizations and determining memory layout constraints.
     *
     * @return true if architecture uses 64-bit pointers, false otherwise
     * @note This function is constexpr and evaluates at compile time
     * @see is32Bit() for the complementary check
     * @see getPointerSize() for the actual pointer size in bits
     * @since 1.0.0
     */
    constexpr bool is64Bit() const noexcept { return pointer_size_bits == 64; }

    /**
     * @brief Checks if the architecture is 32-bit
     *
     * @details Determines whether the target architecture uses 32-bit pointers
     * and address space. This is important for embedded systems and legacy
     * platform compatibility.
     *
     * @return true if architecture uses 32-bit pointers, false otherwise
     * @note This function is constexpr and evaluates at compile time
     * @see is64Bit() for the complementary check
     * @see getPointerSize() for the actual pointer size in bits
     * @since 1.0.0
     */
    constexpr bool is32Bit() const noexcept { return pointer_size_bits == 32; }

    /**
     * @brief Checks if the architecture uses little-endian byte order
     *
     * @details Determines the byte ordering used by the target architecture.
     * Little-endian systems store the least significant byte first, which is
     * common on x86/x64 and most ARM systems.
     *
     * @return true if architecture uses little-endian byte order, false otherwise
     * @note This function is constexpr and evaluates at compile time
     * @see endianness.hpp for comprehensive endianness utilities
     * @since 1.0.0
     */
    constexpr bool isLittleEndian() const noexcept {
        return byte_order == ByteOrder::little_endian;
    }

    /**
     * @brief Checks if the architecture supports unaligned memory access
     *
     * @details Determines whether the target architecture can efficiently handle
     * memory accesses that are not aligned to natural boundaries. This affects
     * memory access patterns and data structure layout decisions.
     *
     * ## Architectures with Efficient Unaligned Access
     * - x86/x64: Hardware support for unaligned access
     * - ARMv7+: Modern ARM cores handle unaligned access efficiently
     *
     * ## Architectures Requiring Alignment
     * - ARMv6 and earlier: May trap or perform poorly on unaligned access
     * - Some RISC architectures: May require alignment for optimal performance
     *
     * @return true if unaligned memory access is efficiently supported
     * @note This function is constexpr and evaluates at compile time
     * @see cache_line_size for optimal alignment boundaries
     * @since 1.0.0
     */
    constexpr bool supportsUnalignedAccess() const noexcept {
        // x86/x64 and most modern ARM cores support unaligned access efficiently
        return architecture == CpuArchitecture::x86 || architecture == CpuArchitecture::x86_64 ||
               architecture == CpuArchitecture::arm_v7 ||
               architecture == CpuArchitecture::arm_v8_32 ||
               architecture == CpuArchitecture::arm_v8_64;
    }

    /**
     * @brief Checks if the architecture supports SIMD instructions
     *
     * @details Determines whether the target architecture has Single Instruction,
     * Multiple Data (SIMD) capabilities for parallel processing. This enables
     * vectorized algorithms and optimizations.
     *
     * ## SIMD Instruction Sets by Architecture
     * - **x86**: MMX, SSE family
     * - **x86_64**: SSE, AVX family
     * - **ARM v7+**: NEON
     * - **ARM v8**: Advanced NEON, SVE (on some implementations)
     *
     * @return true if SIMD instruction support is available
     * @note This function is constexpr and evaluates at compile time
     * @see hasVectorInstructions() for advanced vector processing capabilities
     * @see features.hpp for runtime SIMD feature detection
     * @since 1.0.0
     */
    constexpr bool hasSimdSupport() const noexcept {
        return architecture == CpuArchitecture::x86 || architecture == CpuArchitecture::x86_64 ||
               architecture == CpuArchitecture::arm_v7 ||
               architecture == CpuArchitecture::arm_v8_32 ||
               architecture == CpuArchitecture::arm_v8_64;
    }

    /**
     * @brief Checks if the architecture is ARM-based
     *
     * @details Determines whether the target architecture belongs to the ARM
     * family. This is useful for ARM-specific optimizations and feature detection.
     *
     * ## ARM Architecture Variants Detected
     * - ARMv6: Older embedded systems (e.g., original Raspberry Pi)
     * - ARMv7: Common in mobile devices and embedded systems
     * - ARMv8 (32-bit): Modern ARM in 32-bit execution mode
     * - ARMv8 (64-bit): Modern ARM in 64-bit execution mode (ARM64/AArch64)
     *
     * @return true if architecture is any ARM variant
     * @note This function is constexpr and evaluates at compile time
     * @see isX86() for x86 family detection
     * @since 1.0.0
     */
    constexpr bool isARM() const noexcept {
        return architecture == CpuArchitecture::arm_v6 || architecture == CpuArchitecture::arm_v7 ||
               architecture == CpuArchitecture::arm_v8_32 ||
               architecture == CpuArchitecture::arm_v8_64;
    }

    /**
     * @brief Checks if the architecture is x86-based
     *
     * @details Determines whether the target architecture belongs to the Intel x86
     * family. This includes both 32-bit x86 and 64-bit x86_64 (AMD64) architectures.
     *
     * ## x86 Architecture Variants Detected
     * - **x86**: 32-bit Intel/AMD processors (legacy)
     * - **x86_64**: 64-bit Intel/AMD processors (modern desktop/server)
     *
     * @return true if architecture is x86 or x86_64
     * @note This function is constexpr and evaluates at compile time
     * @see isARM() for ARM family detection
     * @since 1.0.0
     */
    constexpr bool isX86() const noexcept {
        return architecture == CpuArchitecture::x86 || architecture == CpuArchitecture::x86_64;
    }
};

namespace detail {

/**
 * @brief Detects CPU architecture using compiler predefined macros
 * @return CpuArchitecture enum value
 */
constexpr CpuArchitecture detectCpuArchitecture() noexcept {
// x86/x64 detection
#if defined(__x86_64__) || defined(__x86_64) || defined(__amd64__) || defined(__amd64) || \
    defined(_M_X64)
    return CpuArchitecture::x86_64;
#elif defined(__i386__) || defined(__i386) || defined(__i486__) || defined(__i586__) || \
    defined(__i686__) || defined(_M_IX86)
    return CpuArchitecture::x86;

// ARM detection
#elif defined(__aarch64__) || defined(_M_ARM64)
    return CpuArchitecture::arm_v8_64;
#elif defined(__arm__) || defined(_M_ARM)
    #if defined(__ARM_ARCH_8__) || defined(__ARM_ARCH_8A__)
    return CpuArchitecture::arm_v8_32;
    #elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || \
        defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
    return CpuArchitecture::arm_v7;
    #elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || \
        defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__)
    return CpuArchitecture::arm_v6;
    #else
    return CpuArchitecture::arm_v7;  // Default to v7 for unknown ARM
    #endif

// MIPS detection
#elif defined(__mips__) || defined(__mips) || defined(__MIPS__)
    #if defined(__mips64__) || defined(__mips64) || (_MIPS_SIM == _ABI64)
    return CpuArchitecture::mips_64;
    #else
    return CpuArchitecture::mips;
    #endif

// PowerPC detection
#elif defined(__powerpc__) || defined(__powerpc) || defined(__ppc__) || defined(__ppc) || \
    defined(_M_PPC)
    #if defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) || defined(_ARCH_PPC64)
    return CpuArchitecture::powerpc_64;
    #else
    return CpuArchitecture::powerpc;
    #endif

// RISC-V detection
#elif defined(__riscv)
    #if defined(__riscv_xlen) && (__riscv_xlen == 64)
    return CpuArchitecture::risc_v_64;
    #elif defined(__riscv_xlen) && (__riscv_xlen == 32)
    return CpuArchitecture::risc_v_32;
    #else
    return CpuArchitecture::risc_v_64;  // Default to 64-bit RISC-V
    #endif

// SPARC detection
#elif defined(__sparc__) || defined(__sparc)
    #if defined(__sparc64__) || defined(__sparcv9)
    return CpuArchitecture::sparc_64;
    #else
    return CpuArchitecture::sparc;
    #endif

#else
    return CpuArchitecture::unknown;
#endif
}

// Note: detectByteOrder() moved to endianness.hpp as getByteOrder()

/**
 * @brief Determines pointer size based on architecture
 * @return Pointer size in bits (32 or 64)
 */
constexpr int detectPointerSize() noexcept {
    constexpr auto arch = detectCpuArchitecture();
    switch (arch) {
        case CpuArchitecture::x86_64:
        case CpuArchitecture::arm_v8_64:
        case CpuArchitecture::mips_64:
        case CpuArchitecture::powerpc_64:
        case CpuArchitecture::risc_v_64:
        case CpuArchitecture::sparc_64:
            return 64;

        case CpuArchitecture::x86:
        case CpuArchitecture::arm_v6:
        case CpuArchitecture::arm_v7:
        case CpuArchitecture::arm_v8_32:
        case CpuArchitecture::mips:
        case CpuArchitecture::powerpc:
        case CpuArchitecture::risc_v_32:
        case CpuArchitecture::sparc:
            return 32;

        default:
            return sizeof(void*) * 8;  // Fallback to actual pointer size
    }
}

/**
 * @brief Determines typical cache line size for architecture
 * @return Cache line size in bytes
 */
constexpr size_t detectCacheLineSize() noexcept {
    constexpr auto arch = detectCpuArchitecture();
    switch (arch) {
        case CpuArchitecture::x86:
        case CpuArchitecture::x86_64:
            return 64;  // Intel/AMD typical cache line size

        case CpuArchitecture::arm_v6:
        case CpuArchitecture::arm_v7:
            return 32;  // Older ARM cores

        case CpuArchitecture::arm_v8_32:
        case CpuArchitecture::arm_v8_64:
            return 64;  // Modern ARM cores

        case CpuArchitecture::powerpc:
        case CpuArchitecture::powerpc_64:
            return 128;  // PowerPC typical cache line size

        case CpuArchitecture::mips:
        case CpuArchitecture::mips_64:
        case CpuArchitecture::risc_v_32:
        case CpuArchitecture::risc_v_64:
        case CpuArchitecture::sparc:
        case CpuArchitecture::sparc_64:
            return 64;  // Common default

        default:
            return 64;  // Safe default for unknown architectures
    }
}

/**
 * @brief Gets human-readable architecture name
 * @return Architecture name string
 */
constexpr const char* getArchitectureName() noexcept {
    constexpr auto arch = detectCpuArchitecture();
    switch (arch) {
        case CpuArchitecture::x86:
            return "x86";
        case CpuArchitecture::x86_64:
            return "x86_64";
        case CpuArchitecture::arm_v6:
            return "ARM v6";
        case CpuArchitecture::arm_v7:
            return "ARM v7";
        case CpuArchitecture::arm_v8_32:
            return "ARM v8 (32-bit)";
        case CpuArchitecture::arm_v8_64:
            return "ARM v8 (64-bit)";
        case CpuArchitecture::mips:
            return "MIPS";
        case CpuArchitecture::mips_64:
            return "MIPS64";
        case CpuArchitecture::powerpc:
            return "PowerPC";
        case CpuArchitecture::powerpc_64:
            return "PowerPC64";
        case CpuArchitecture::risc_v_32:
            return "RISC-V 32";
        case CpuArchitecture::risc_v_64:
            return "RISC-V 64";
        case CpuArchitecture::sparc:
            return "SPARC";
        case CpuArchitecture::sparc_64:
            return "SPARC64";
        default:
            return "Unknown";
    }
}

}  // namespace detail

/**
 * @brief Gets the current CPU architecture
 *
 * @details Detects the target CPU architecture at compile time using compiler
 * predefined macros. This function provides a simple way to determine the
 * architecture family and make architecture-specific decisions.
 *
 * ## Detection Method
 * Uses compiler-specific predefined macros to identify the target architecture:
 * - **GCC/Clang**: __x86_64__, __i386__, __arm__, __aarch64__, etc.
 * - **MSVC**: _M_X64, _M_IX86, _M_ARM, _M_ARM64, etc.
 * - **Other compilers**: Standard architecture detection macros
 *
 * ## Performance
 * - **Zero Runtime Cost**: All detection occurs at compile time
 * - **Optimizable**: Enables compiler optimizations through constexpr evaluation
 * - **Branch Elimination**: if constexpr patterns eliminate unused code paths
 *
 * @return CpuArchitecture enum value representing the detected architecture
 * @note This function is constexpr and evaluates at compile time
 * @see getArchitectureInfo() for comprehensive architecture information
 * @see ArchitectureInfo for detailed architecture capabilities
 *
 * @example Architecture-specific code selection
 * @code
 * constexpr auto arch = trlc::platform::getCpuArchitecture();
 *
 * if constexpr (arch == trlc::platform::CpuArchitecture::x86_64) {
 *     // x86_64 specific optimizations
 *     return process_x86_64(data);
 * } else if constexpr (arch == trlc::platform::CpuArchitecture::arm_v8_64) {
 *     // ARM64 specific optimizations
 *     return process_arm64(data);
 * } else {
 *     // Generic fallback
 *     return process_generic(data);
 * }
 * @endcode
 *
 * @since 1.0.0
 */
constexpr CpuArchitecture getCpuArchitecture() noexcept {
    return detail::detectCpuArchitecture();
}

// Note: getByteOrder() is provided by endianness.hpp

/**
 * @brief Gets comprehensive architecture information
 *
 * @details Creates a complete ArchitectureInfo structure containing all detected
 * architectural characteristics. This provides a unified view of the system's
 * capabilities including architecture type, endianness, pointer size, and
 * performance-related information.
 *
 * ## Information Provided
 * - **Architecture Type**: Specific CPU architecture family and variant
 * - **Byte Order**: System endianness for data serialization
 * - **Pointer Size**: Address space size (32/64-bit) for memory management
 * - **Cache Line Size**: Typical cache line size for alignment optimization
 * - **Architecture Name**: Human-readable identifier for debugging/logging
 *
 * ## Use Cases
 * - **Performance Optimization**: Cache-line alignment, SIMD detection
 * - **Memory Management**: Pointer size awareness, alignment requirements
 * - **Cross-Platform Code**: Architecture-specific algorithm selection
 * - **Debug/Logging**: Human-readable architecture identification
 *
 * @return ArchitectureInfo structure with all detected architectural information
 * @note This function is constexpr and evaluates at compile time
 * @see getCpuArchitecture() for simple architecture detection
 * @see ArchitectureInfo for structure member documentation
 *
 * @example Comprehensive architecture analysis
 * @code
 * constexpr auto info = trlc::platform::getArchitectureInfo();
 *
 * // Architecture-specific optimizations
 * if constexpr (info.isX86() && info.is64Bit()) {
 *     // x86_64 specific code
 * } else if constexpr (info.isARM() && info.hasSimdSupport()) {
 *     // ARM with NEON optimizations
 * }
 *
 * // Cache-line aligned allocation
 * alignas(info.cache_line_size) char buffer[1024];
 *
 * // Debug output
 * std::cout << "Running on " << info.arch_name
 *           << " (" << info.pointer_size_bits << "-bit)" << std::endl;
 * @endcode
 *
 * @since 1.0.0
 */
constexpr ArchitectureInfo getArchitectureInfo() noexcept {
    return ArchitectureInfo{detail::detectCpuArchitecture(),
                            getByteOrder(),  // Use getByteOrder() from endianness.hpp
                            detail::detectPointerSize(),
                            detail::detectCacheLineSize(),
                            detail::getArchitectureName()};
}

/**
 * @brief Gets the pointer size in bits
 *
 * @details Determines the size of pointers on the target architecture, which
 * indicates the address space size and affects memory management decisions.
 *
 * ## Pointer Sizes by Architecture
 * - **32-bit Architectures**: 32-bit pointers, 4GB address space
 *   - x86, ARMv6/v7, 32-bit MIPS/PowerPC/RISC-V/SPARC
 * - **64-bit Architectures**: 64-bit pointers, large address space
 *   - x86_64, ARM64, 64-bit MIPS/PowerPC/RISC-V/SPARC
 *
 * ## Applications
 * - **Memory Management**: Size calculations, pointer arithmetic
 * - **Serialization**: Platform-aware data format selection
 * - **Performance**: Cache efficiency, data structure layout
 *
 * @return Pointer size in bits (32 or 64)
 * @note This function is constexpr and evaluates at compile time
 * @see is32BitArchitecture() for boolean 32-bit check
 * @see is64BitArchitecture() for boolean 64-bit check
 * @see ArchitectureInfo::is32Bit() and ArchitectureInfo::is64Bit()
 *
 * @example Pointer size-aware code
 * @code
 * constexpr int ptr_size = trlc::platform::getPointerSize();
 *
 * if constexpr (ptr_size == 64) {
 *     // 64-bit optimizations
 *     using hash_type = std::uint64_t;
 * } else {
 *     // 32-bit compatibility
 *     using hash_type = std::uint32_t;
 * }
 * @endcode
 *
 * @since 1.0.0
 */
constexpr int getPointerSize() noexcept {
    return detail::detectPointerSize();
}

/**
 * @brief Checks if SIMD instructions are supported
 *
 * @details Determines whether the target architecture has Single Instruction,
 * Multiple Data (SIMD) capabilities for parallel processing. This is a
 * compile-time architectural capability check, not a runtime feature detection.
 *
 * ## SIMD Support by Architecture
 * - **x86**: MMX, SSE family instructions
 * - **x86_64**: SSE, AVX family instructions
 * - **ARMv7**: NEON SIMD instructions
 * - **ARMv8**: Advanced NEON, optional SVE
 * - **Other architectures**: May have limited or no SIMD support
 *
 * ## Performance Implications
 * - **Vectorization**: Enables parallel processing of data arrays
 * - **Mathematical Operations**: Accelerated floating-point calculations
 * - **Media Processing**: Efficient image/audio/video processing
 *
 * @return true if SIMD instruction support is architecturally available
 * @note This function is constexpr and evaluates at compile time
 * @note For runtime SIMD feature detection, see features.hpp
 * @see hasVectorInstructions() for advanced vector processing
 * @see ArchitectureInfo::hasSimdSupport() for member function version
 *
 * @example SIMD-aware algorithm selection
 * @code
 * template<typename T>
 * void process_array(const T* input, T* output, size_t size) {
 *     if constexpr (trlc::platform::hasSimdSupport()) {
 *         // Use SIMD optimized version
 *         process_array_simd(input, output, size);
 *     } else {
 *         // Use scalar fallback
 *         process_array_scalar(input, output, size);
 *     }
 * }
 * @endcode
 *
 * @since 1.0.0
 */
constexpr bool hasSimdSupport() noexcept {
    constexpr auto arch = getCpuArchitecture();
    return arch == CpuArchitecture::x86 || arch == CpuArchitecture::x86_64 ||
           arch == CpuArchitecture::arm_v7 || arch == CpuArchitecture::arm_v8_32 ||
           arch == CpuArchitecture::arm_v8_64;
}

/**
 * @brief Checks if vector instructions are supported
 *
 * @details Determines whether the target architecture supports advanced vector
 * processing capabilities beyond basic SIMD. Vector instructions typically
 * provide more sophisticated parallel processing features than basic SIMD.
 *
 * ## Vector Instruction Support
 * - **x86_64**: Advanced vector extensions (AVX, AVX2, AVX-512)
 * - **ARM64**: Advanced NEON, Scalable Vector Extensions (SVE)
 * - **RISC-V 64**: RISC-V Vector Extension (RVV)
 * - **Other architectures**: Limited or experimental vector support
 *
 * ## Capabilities
 * - **Variable-length vectors**: Adapt to hardware capabilities
 * - **Advanced operations**: Predication, gather/scatter, reduction
 * - **Scalable performance**: Efficient across different vector widths
 *
 * @return true if advanced vector instruction support is available
 * @note This function is constexpr and evaluates at compile time
 * @note This checks architectural capability, not runtime availability
 * @see hasSimdSupport() for basic SIMD capability check
 * @see features.hpp for runtime vector feature detection
 *
 * @example Vector-aware processing
 * @code
 * if constexpr (trlc::platform::hasVectorInstructions()) {
 *     // Use advanced vector algorithms
 *     return process_with_vectors(data);
 * } else if constexpr (trlc::platform::hasSimdSupport()) {
 *     // Use basic SIMD fallback
 *     return process_with_simd(data);
 * } else {
 *     // Use scalar implementation
 *     return process_scalar(data);
 * }
 * @endcode
 *
 * @since 1.0.0
 */
constexpr bool hasVectorInstructions() noexcept {
    // Vector instructions include SIMD and more advanced vector processing
    constexpr auto arch = getCpuArchitecture();
    return arch == CpuArchitecture::x86_64 ||     // SSE/AVX
           arch == CpuArchitecture::arm_v8_64 ||  // NEON/SVE
           arch == CpuArchitecture::risc_v_64;    // RISC-V Vector Extension
}

/**
 * @brief Checks if cache line alignment is supported/beneficial
 *
 * @details Determines whether cache line alignment provides performance benefits
 * on the target architecture. Most modern architectures benefit from proper
 * cache line alignment to avoid false sharing and optimize memory bandwidth.
 *
 * ## Cache Line Alignment Benefits
 * - **False Sharing Avoidance**: Prevent cache line ping-ponging between cores
 * - **Memory Bandwidth**: Optimize cache utilization and prefetching
 * - **Atomic Operations**: Improve performance of concurrent data structures
 *
 * ## Cache Line Sizes
 * - **x86/x64**: Typically 64 bytes
 * - **ARM**: 32-64 bytes depending on implementation
 * - **PowerPC**: Often 128 bytes
 * - **Other architectures**: Variable, typically 32-128 bytes
 *
 * @return true if cache line alignment provides performance benefits
 * @note This function is constexpr and evaluates at compile time
 * @see ArchitectureInfo::cache_line_size for actual cache line size
 * @see getArchitectureInfo() for comprehensive cache information
 *
 * @example Cache-aligned data structure
 * @code
 * if constexpr (trlc::platform::supportsCacheLineAlignment()) {
 *     constexpr auto info = trlc::platform::getArchitectureInfo();
 *
 *     // Align to cache line boundary
 *     alignas(info.cache_line_size) struct {
 *         std::atomic<int> counter;
 *         char padding[info.cache_line_size - sizeof(std::atomic<int>)];
 *     } cache_aligned_counter;
 * }
 * @endcode
 *
 * @since 1.0.0
 */
constexpr bool supportsCacheLineAlignment() noexcept {
    // Most modern architectures benefit from cache line alignment
    constexpr auto arch = getCpuArchitecture();
    return arch != CpuArchitecture::unknown;
}

/**
 * @brief Checks if the architecture is 64-bit
 *
 * @details Convenience function to determine if the target architecture uses
 * 64-bit pointers and address space. This is equivalent to checking if the
 * pointer size is 64 bits.
 *
 * ## 64-bit Architecture Benefits
 * - **Large Address Space**: Access to more than 4GB of memory
 * - **Performance**: Often better register utilization and instruction sets
 * - **Modern Features**: Support for advanced CPU features and optimizations
 *
 * @return true if target architecture is 64-bit
 * @note This function is constexpr and evaluates at compile time
 * @see is32BitArchitecture() for the complementary check
 * @see getPointerSize() for the actual pointer size in bits
 * @see ArchitectureInfo::is64Bit() for member function version
 *
 * @example 64-bit optimizations
 * @code
 * if constexpr (trlc::platform::is64BitArchitecture()) {
 *     // Use 64-bit optimized data structures
 *     using size_type = std::uint64_t;
 *     using hash_type = std::uint64_t;
 * } else {
 *     // Use 32-bit compatible types
 *     using size_type = std::uint32_t;
 *     using hash_type = std::uint32_t;
 * }
 * @endcode
 *
 * @since 1.0.0
 */
constexpr bool is64BitArchitecture() noexcept {
    return getPointerSize() == 64;
}

/**
 * @brief Checks if the architecture is 32-bit
 *
 * @details Convenience function to determine if the target architecture uses
 * 32-bit pointers and address space. This is important for embedded systems
 * and legacy platform compatibility.
 *
 * ## 32-bit Architecture Considerations
 * - **Memory Limitations**: 4GB address space limit
 * - **Embedded Systems**: Common in resource-constrained environments
 * - **Legacy Compatibility**: Support for older platforms and systems
 *
 * @return true if target architecture is 32-bit
 * @note This function is constexpr and evaluates at compile time
 * @see is64BitArchitecture() for the complementary check
 * @see getPointerSize() for the actual pointer size in bits
 * @see ArchitectureInfo::is32Bit() for member function version
 *
 * @example 32-bit aware code
 * @code
 * if constexpr (trlc::platform::is32BitArchitecture()) {
 *     // Use memory-efficient data structures
 *     static_assert(sizeof(void*) == 4);
 *     using offset_type = std::uint32_t;
 * } else {
 *     // 64-bit architecture, can use larger types
 *     using offset_type = std::uint64_t;
 * }
 * @endcode
 *
 * @since 1.0.0
 */
constexpr bool is32BitArchitecture() noexcept {
    return getPointerSize() == 32;
}

}  // namespace platform
}  // namespace trlc

// Preprocessor-only architecture detection (for use in #if directives)
#if defined(__x86_64__) || defined(__x86_64) || defined(__amd64__) || defined(__amd64) || \
    defined(_M_X64)
    #define TRLC_ARCH_X86_64_PP 1
    #define TRLC_ARCH_64BIT_PP 1
    #define TRLC_ARCH_X86_64 1
    #define TRLC_ARCH_64BIT 1
#else
    #define TRLC_ARCH_X86_64_PP 0
    #define TRLC_ARCH_X86_64 0
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
    #define TRLC_ARCH_ARM64_PP 1
    #define TRLC_ARCH_64BIT_PP 1
    #define TRLC_ARCH_ARM64 1
    #define TRLC_ARCH_64BIT 1
#else
    #define TRLC_ARCH_ARM64_PP 0
    #define TRLC_ARCH_ARM64 0
#endif

#if defined(__i386__) || defined(__i386) || defined(_M_IX86)
    #define TRLC_ARCH_X86_PP 1
    #define TRLC_ARCH_32BIT_PP 1
    #define TRLC_ARCH_X86 1
    #define TRLC_ARCH_32BIT 1
#else
    #define TRLC_ARCH_X86_PP 0
    #define TRLC_ARCH_X86 0
#endif

#if defined(__arm__) || defined(_M_ARM)
    #define TRLC_ARCH_ARM_PP 1
    #define TRLC_ARCH_32BIT_PP 1
    #define TRLC_ARCH_ARM 1
    #define TRLC_ARCH_32BIT 1
#else
    #define TRLC_ARCH_ARM_PP 0
    #define TRLC_ARCH_ARM 0
#endif

// Additional architecture family macros
#if defined(__mips__) || defined(__mips) || defined(__MIPS__)
    #define TRLC_ARCH_MIPS 1
#else
    #define TRLC_ARCH_MIPS 0
#endif

#if defined(__powerpc__) || defined(__powerpc) || defined(__ppc__) || defined(__ppc) || \
    defined(_M_PPC)
    #define TRLC_ARCH_POWERPC 1
#else
    #define TRLC_ARCH_POWERPC 0
#endif

#if defined(__riscv)
    #define TRLC_ARCH_RISCV 1
#else
    #define TRLC_ARCH_RISCV 0
#endif

#if defined(__sparc__) || defined(__sparc)
    #define TRLC_ARCH_SPARC 1
#else
    #define TRLC_ARCH_SPARC 0
#endif

// Ensure we have consistent bit width detection
#ifndef TRLC_ARCH_64BIT_PP
    #if defined(__LP64__) || defined(_WIN64) || \
        (defined(__SIZEOF_POINTER__) && __SIZEOF_POINTER__ == 8)
        #define TRLC_ARCH_64BIT_PP 1
        #define TRLC_ARCH_32BIT_PP 0
        #define TRLC_ARCH_64BIT 1
        #define TRLC_ARCH_32BIT 0
    #else
        #define TRLC_ARCH_64BIT_PP 0
        #define TRLC_ARCH_32BIT_PP 1
        #define TRLC_ARCH_64BIT 0
        #define TRLC_ARCH_32BIT 1
    #endif
#endif

#ifndef TRLC_ARCH_32BIT_PP
    #define TRLC_ARCH_32BIT_PP (!TRLC_ARCH_64BIT_PP)
    #define TRLC_ARCH_32BIT (!TRLC_ARCH_64BIT)
#endif

// Byte order macros (preprocessor compatible)
#if defined(__BYTE_ORDER__)
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        #define TRLC_LITTLE_ENDIAN 1
        #define TRLC_BIG_ENDIAN 0
    #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        #define TRLC_LITTLE_ENDIAN 0
        #define TRLC_BIG_ENDIAN 1
    #else
        #define TRLC_LITTLE_ENDIAN 0
        #define TRLC_BIG_ENDIAN 0
    #endif
#elif defined(__LITTLE_ENDIAN__) || defined(__ARMEL__)
    #define TRLC_LITTLE_ENDIAN 1
    #define TRLC_BIG_ENDIAN 0
#elif defined(__BIG_ENDIAN__) || defined(__ARMEB__)
    #define TRLC_LITTLE_ENDIAN 0
    #define TRLC_BIG_ENDIAN 1
#else
   // Architecture-based endianness detection
    #if TRLC_ARCH_X86 || TRLC_ARCH_X86_64 || TRLC_ARCH_ARM || TRLC_ARCH_ARM64 || TRLC_ARCH_RISCV
        #define TRLC_LITTLE_ENDIAN 1
        #define TRLC_BIG_ENDIAN 0
    #elif TRLC_ARCH_SPARC || TRLC_ARCH_MIPS || TRLC_ARCH_POWERPC
        #define TRLC_LITTLE_ENDIAN 0
        #define TRLC_BIG_ENDIAN 1
    #else
        #define TRLC_LITTLE_ENDIAN 1  // Default to little endian
        #define TRLC_BIG_ENDIAN 0
    #endif
#endif

// SIMD and vector capability macros (preprocessor compatible)
#if TRLC_ARCH_X86 || TRLC_ARCH_X86_64 || TRLC_ARCH_ARM || TRLC_ARCH_ARM64
    #define TRLC_HAS_SIMD 1
#else
    #define TRLC_HAS_SIMD 0
#endif

#if TRLC_ARCH_X86_64 || TRLC_ARCH_ARM64 || TRLC_ARCH_RISCV
    #define TRLC_HAS_VECTOR 1
#else
    #define TRLC_HAS_VECTOR 0
#endif

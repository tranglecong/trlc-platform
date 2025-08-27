#pragma once

/**
 * @file features.hpp
 * @brief Language and runtime feature detection utilities
 *
 * This header provides compile-time language feature detection and runtime
 * CPU feature detection for cross-platform C++ development. Language features
 * are detected at compile time with zero overhead, while CPU features may
 * require runtime detection.
 *
 * @copyright Copyright (c) 2025 TRLC Platform
 */

#include <cstdint>

// Include intrinsics headers for CPU feature detection
#if defined(__x86_64__) || defined(__i386__) || defined(_M_X64) || defined(_M_IX86)
    #if defined(_MSC_VER)
        #include <intrin.h>
    #elif defined(__GNUC__) || defined(__clang__)
        #include <cpuid.h>
        #include <immintrin.h>
    #endif
    #define TRLC_HAS_X86_INTRINSICS 1
#else
    #define TRLC_HAS_X86_INTRINSICS 0
#endif

#if defined(__ARM_NEON) || defined(__aarch64__)
    #if defined(__GNUC__) || defined(__clang__)
        #include <arm_neon.h>
    #endif
    #define TRLC_HAS_ARM_INTRINSICS 1
#else
    #define TRLC_HAS_ARM_INTRINSICS 0
#endif

namespace trlc {
namespace platform {

/**
 * @brief Language feature enumeration
 *
 * Represents compile-time detectable language and compiler features.
 * These features are determined at compile time based on compiler
 * capabilities and build settings.
 */
enum class LanguageFeature : int {
    exceptions = 0,               ///< C++ exception handling support
    rtti,                         ///< Runtime Type Information support
    threads,                      ///< Threading support (std::thread)
    atomic_operations,            ///< Atomic operations support
    inline_assembly,              ///< Inline assembly support
    vector_intrinsics,            ///< Vector/SIMD intrinsics support
    stack_protection,             ///< Stack protection features
    address_sanitizer,            ///< AddressSanitizer support
    thread_sanitizer,             ///< ThreadSanitizer support
    memory_sanitizer,             ///< MemorySanitizer support
    undefined_behavior_sanitizer  ///< UndefinedBehaviorSanitizer support
};

/**
 * @brief Runtime feature enumeration
 *
 * Represents CPU features that may require runtime detection.
 * These features depend on the actual hardware capabilities
 * and may not be detectable at compile time.
 */
enum class RuntimeFeature : int {
    sse = 0,         ///< SSE (Streaming SIMD Extensions)
    sse2,            ///< SSE2 extensions
    sse3,            ///< SSE3 extensions
    sse4_1,          ///< SSE4.1 extensions
    sse4_2,          ///< SSE4.2 extensions
    avx,             ///< AVX (Advanced Vector Extensions)
    avx2,            ///< AVX2 extensions
    avx512f,         ///< AVX-512 Foundation
    neon,            ///< ARM NEON SIMD extensions
    hardware_aes,    ///< Hardware AES acceleration
    hardware_random  ///< Hardware random number generation
};

/**
 * @brief Feature detection structure
 *
 * Contains both compile-time language features and runtime CPU features.
 * Language features are determined at compile time, while runtime features
 * may require CPU feature detection at runtime.
 */
struct FeatureSet {
    // Language features (compile-time determined)
    bool has_exceptions;                    ///< Exception handling available
    bool has_rtti;                          ///< RTTI available
    bool has_threads;                       ///< Threading support available
    bool has_atomic;                        ///< Atomic operations available
    bool has_inline_asm;                    ///< Inline assembly available
    bool has_vector_intrinsics;             ///< Vector intrinsics available
    bool has_stack_protection;              ///< Stack protection enabled
    bool has_address_sanitizer;             ///< AddressSanitizer enabled
    bool has_thread_sanitizer;              ///< ThreadSanitizer enabled
    bool has_memory_sanitizer;              ///< MemorySanitizer enabled
    bool has_undefined_behavior_sanitizer;  ///< UBSan enabled

    // Runtime features (may require runtime detection)
    bool has_sse;              ///< SSE support
    bool has_sse2;             ///< SSE2 support
    bool has_sse3;             ///< SSE3 support
    bool has_sse4_1;           ///< SSE4.1 support
    bool has_sse4_2;           ///< SSE4.2 support
    bool has_avx;              ///< AVX support
    bool has_avx2;             ///< AVX2 support
    bool has_avx512f;          ///< AVX-512F support
    bool has_neon;             ///< ARM NEON support
    bool has_hardware_aes;     ///< Hardware AES support
    bool has_hardware_random;  ///< Hardware RNG support

    /**
     * @brief Checks if a specific language feature is available
     * @param feature Language feature to check
     * @return true if feature is available
     */
    constexpr bool hasLanguageFeature(LanguageFeature feature) const noexcept {
        switch (feature) {
            case LanguageFeature::exceptions:
                return has_exceptions;
            case LanguageFeature::rtti:
                return has_rtti;
            case LanguageFeature::threads:
                return has_threads;
            case LanguageFeature::atomic_operations:
                return has_atomic;
            case LanguageFeature::inline_assembly:
                return has_inline_asm;
            case LanguageFeature::vector_intrinsics:
                return has_vector_intrinsics;
            case LanguageFeature::stack_protection:
                return has_stack_protection;
            case LanguageFeature::address_sanitizer:
                return has_address_sanitizer;
            case LanguageFeature::thread_sanitizer:
                return has_thread_sanitizer;
            case LanguageFeature::memory_sanitizer:
                return has_memory_sanitizer;
            case LanguageFeature::undefined_behavior_sanitizer:
                return has_undefined_behavior_sanitizer;
            default:
                return false;
        }
    }

    /**
     * @brief Checks if a specific runtime feature is available
     * @param feature Runtime feature to check
     * @return true if feature is available
     * @note This is not constexpr as it may require runtime detection
     */
    bool hasRuntimeFeature(RuntimeFeature feature) const noexcept {
        switch (feature) {
            case RuntimeFeature::sse:
                return has_sse;
            case RuntimeFeature::sse2:
                return has_sse2;
            case RuntimeFeature::sse3:
                return has_sse3;
            case RuntimeFeature::sse4_1:
                return has_sse4_1;
            case RuntimeFeature::sse4_2:
                return has_sse4_2;
            case RuntimeFeature::avx:
                return has_avx;
            case RuntimeFeature::avx2:
                return has_avx2;
            case RuntimeFeature::avx512f:
                return has_avx512f;
            case RuntimeFeature::neon:
                return has_neon;
            case RuntimeFeature::hardware_aes:
                return has_hardware_aes;
            case RuntimeFeature::hardware_random:
                return has_hardware_random;
            default:
                return false;
        }
    }
};

//
// Language feature detection functions
//

/**
 * @brief Detects if C++ exceptions are enabled
 * @return true if exceptions are available
 */
constexpr bool hasExceptions() noexcept {
#ifdef __cpp_exceptions
    return true;
#elif defined(__EXCEPTIONS) || defined(_CPPUNWIND)
    return true;
#else
    return false;
#endif
}

/**
 * @brief Detects if RTTI (Runtime Type Information) is enabled
 * @return true if RTTI is available
 */
constexpr bool hasRtti() noexcept {
#ifdef __cpp_rtti
    return true;
#elif defined(__GXX_RTTI) || defined(_CPPRTTI)
    return true;
#elif defined(__has_feature)
    #if __has_feature(cxx_rtti)
    return true;
    #else
    return false;
    #endif
#else
    return true;  // Assume available by default
#endif
}

/**
 * @brief Detects if threading support is available
 * @return true if std::thread and threading primitives are available
 */
constexpr bool hasThreads() noexcept {
#ifdef __STDCPP_THREADS__
    return __STDCPP_THREADS__ == 1;
#elif defined(_REENTRANT) || defined(_MT)
    return true;
#else
    return false;
#endif
}

/**
 * @brief Detects if atomic operations are supported
 * @return true if std::atomic and atomic operations are available
 */
constexpr bool hasAtomicOperations() noexcept {
#ifdef __cpp_lib_atomic_shared_ptr
    return true;
#elif defined(__ATOMIC_RELAXED)
    return true;
#elif defined(_MSC_VER)
    return true;  // MSVC has atomic support
#else
    return true;  // Assume available in C++17+
#endif
}

/**
 * @brief Detects if inline assembly is supported
 * @return true if inline assembly can be used
 */
constexpr bool hasInlineAssembly() noexcept {
#if defined(__GNUC__) || defined(__clang__)
    return true;
#elif defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
    return true;
#else
    return false;
#endif
}

/**
 * @brief Detects if vector intrinsics are available
 * @return true if SIMD intrinsics can be used
 */
constexpr bool hasVectorIntrinsics() noexcept {
    return TRLC_HAS_X86_INTRINSICS || TRLC_HAS_ARM_INTRINSICS;
}

/**
 * @brief Detects if stack protection is enabled
 * @return true if stack protection features are active
 */
constexpr bool hasStackProtection() noexcept {
#ifdef __STACK_CHK_FAIL
    return true;
#elif defined(__SSP__) || defined(__SSP_ALL__)
    return true;
#elif defined(_FORTIFY_SOURCE) && _FORTIFY_SOURCE > 0
    return true;
#else
    return false;
#endif
}

/**
 * @brief Detects if AddressSanitizer is enabled
 * @return true if ASan is active
 */
constexpr bool hasAddressSanitizer() noexcept {
#ifdef __has_feature
    #if __has_feature(address_sanitizer)
    return true;
    #endif
#endif
#ifdef __SANITIZE_ADDRESS__
    return true;
#else
    return false;
#endif
}

/**
 * @brief Detects if ThreadSanitizer is enabled
 * @return true if TSan is active
 */
constexpr bool hasThreadSanitizer() noexcept {
#ifdef __has_feature
    #if __has_feature(thread_sanitizer)
    return true;
    #endif
#endif
#ifdef __SANITIZE_THREAD__
    return true;
#else
    return false;
#endif
}

/**
 * @brief Detects if MemorySanitizer is enabled
 * @return true if MSan is active
 */
constexpr bool hasMemorySanitizer() noexcept {
#ifdef __has_feature
    #if __has_feature(memory_sanitizer)
    return true;
    #endif
#endif
#ifdef __SANITIZE_MEMORY__
    return true;
#else
    return false;
#endif
}

/**
 * @brief Detects if UndefinedBehaviorSanitizer is enabled
 * @return true if UBSan is active
 */
constexpr bool hasUndefinedBehaviorSanitizer() noexcept {
#ifdef __has_feature
    #if __has_feature(undefined_behavior_sanitizer)
    return true;
    #endif
#endif
#ifdef __SANITIZE_UNDEFINED__
    return true;
#else
    return false;
#endif
}

//
// Runtime feature detection functions
//

#if TRLC_HAS_X86_INTRINSICS

namespace detail {

/**
 * @brief CPUID wrapper for x86/x64 processors
 * @param leaf CPUID leaf (function)
 * @param subleaf CPUID subleaf (subfunction)
 * @param regs Output array for EAX, EBX, ECX, EDX
 */
inline void cpuid(uint32_t leaf, uint32_t subleaf, uint32_t regs[4]) noexcept {
    #if defined(_MSC_VER)
    __cpuidex(reinterpret_cast<int*>(regs), static_cast<int>(leaf), static_cast<int>(subleaf));
    #elif defined(__GNUC__) || defined(__clang__)
    __cpuid_count(leaf, subleaf, regs[0], regs[1], regs[2], regs[3]);
    #else
    regs[0] = regs[1] = regs[2] = regs[3] = 0;
    #endif
}

/**
 * @brief Check if a specific CPU feature bit is set
 * @param leaf CPUID leaf
 * @param subleaf CPUID subleaf
 * @param reg Register index (0=EAX, 1=EBX, 2=ECX, 3=EDX)
 * @param bit Bit position to check
 * @return true if feature bit is set
 */
inline bool checkCpuFeature(uint32_t leaf, uint32_t subleaf, int reg, int bit) noexcept {
    uint32_t regs[4];
    cpuid(leaf, subleaf, regs);
    return (regs[reg] & (1u << bit)) != 0;
}

}  // namespace detail

#endif  // TRLC_HAS_X86_INTRINSICS

/**
 * @brief Detects SSE support at runtime
 * @return true if SSE is supported by the CPU
 */
inline bool hasSseSupport() noexcept {
#if TRLC_HAS_X86_INTRINSICS
    return detail::checkCpuFeature(1, 0, 3, 25);  // EDX bit 25
#else
    return false;
#endif
}

/**
 * @brief Detects SSE2 support at runtime
 * @return true if SSE2 is supported by the CPU
 */
inline bool hasSse2Support() noexcept {
#if TRLC_HAS_X86_INTRINSICS
    return detail::checkCpuFeature(1, 0, 3, 26);  // EDX bit 26
#else
    return false;
#endif
}

/**
 * @brief Detects SSE3 support at runtime
 * @return true if SSE3 is supported by the CPU
 */
inline bool hasSse3Support() noexcept {
#if TRLC_HAS_X86_INTRINSICS
    return detail::checkCpuFeature(1, 0, 2, 0);  // ECX bit 0
#else
    return false;
#endif
}

/**
 * @brief Detects SSE4.1 support at runtime
 * @return true if SSE4.1 is supported by the CPU
 */
inline bool hasSse41Support() noexcept {
#if TRLC_HAS_X86_INTRINSICS
    return detail::checkCpuFeature(1, 0, 2, 19);  // ECX bit 19
#else
    return false;
#endif
}

/**
 * @brief Detects SSE4.2 support at runtime
 * @return true if SSE4.2 is supported by the CPU
 */
inline bool hasSse42Support() noexcept {
#if TRLC_HAS_X86_INTRINSICS
    return detail::checkCpuFeature(1, 0, 2, 20);  // ECX bit 20
#else
    return false;
#endif
}

/**
 * @brief Detects AVX support at runtime
 * @return true if AVX is supported by the CPU
 */
inline bool hasAvxSupport() noexcept {
#if TRLC_HAS_X86_INTRINSICS
    return detail::checkCpuFeature(1, 0, 2, 28);  // ECX bit 28
#else
    return false;
#endif
}

/**
 * @brief Detects AVX2 support at runtime
 * @return true if AVX2 is supported by the CPU
 */
inline bool hasAvx2Support() noexcept {
#if TRLC_HAS_X86_INTRINSICS
    return detail::checkCpuFeature(7, 0, 1, 5);  // EBX bit 5
#else
    return false;
#endif
}

/**
 * @brief Detects AVX-512F support at runtime
 * @return true if AVX-512F is supported by the CPU
 */
inline bool hasAvx512fSupport() noexcept {
#if TRLC_HAS_X86_INTRINSICS
    return detail::checkCpuFeature(7, 0, 1, 16);  // EBX bit 16
#else
    return false;
#endif
}

/**
 * @brief Detects ARM NEON support
 * @return true if NEON is supported
 */
inline bool hasNeonSupport() noexcept {
#if TRLC_HAS_ARM_INTRINSICS
    #ifdef __ARM_NEON
    return true;
    #elif defined(__aarch64__)
    return true;  // NEON is mandatory on AArch64
    #else
    return false;
    #endif
#else
    return false;
#endif
}

/**
 * @brief Detects hardware AES support
 * @return true if hardware AES acceleration is available
 */
inline bool hasHardwareAes() noexcept {
#if TRLC_HAS_X86_INTRINSICS
    return detail::checkCpuFeature(1, 0, 2, 25);  // ECX bit 25
#elif TRLC_HAS_ARM_INTRINSICS && defined(__ARM_FEATURE_AES)
    return true;
#else
    return false;
#endif
}

/**
 * @brief Detects hardware random number generation support
 * @return true if hardware RNG is available
 */
inline bool hasHardwareRandom() noexcept {
#if TRLC_HAS_X86_INTRINSICS
    return detail::checkCpuFeature(1, 0, 2, 30);  // ECX bit 30 (RDRAND)
#else
    return false;
#endif
}

//
// Unified feature detection functions
//

/**
 * @brief Gets a complete feature set with all detected features
 * @return FeatureSet containing all available features
 * @note Language features are compile-time, runtime features require detection
 */
constexpr FeatureSet getFeatureSet() noexcept {
    return FeatureSet{// Language features (compile-time)
                      .has_exceptions = hasExceptions(),
                      .has_rtti = hasRtti(),
                      .has_threads = hasThreads(),
                      .has_atomic = hasAtomicOperations(),
                      .has_inline_asm = hasInlineAssembly(),
                      .has_vector_intrinsics = hasVectorIntrinsics(),
                      .has_stack_protection = hasStackProtection(),
                      .has_address_sanitizer = hasAddressSanitizer(),
                      .has_thread_sanitizer = hasThreadSanitizer(),
                      .has_memory_sanitizer = hasMemorySanitizer(),
                      .has_undefined_behavior_sanitizer = hasUndefinedBehaviorSanitizer(),

                      // Runtime features (initialized to false, require runtime detection)
                      .has_sse = false,
                      .has_sse2 = false,
                      .has_sse3 = false,
                      .has_sse4_1 = false,
                      .has_sse4_2 = false,
                      .has_avx = false,
                      .has_avx2 = false,
                      .has_avx512f = false,
                      .has_neon = false,
                      .has_hardware_aes = false,
                      .has_hardware_random = false};
}

/**
 * @brief Checks if a specific language feature is available
 * @param feature Language feature to check
 * @return true if feature is available at compile time
 */
constexpr bool hasLanguageFeature(LanguageFeature feature) noexcept {
    switch (feature) {
        case LanguageFeature::exceptions:
            return hasExceptions();
        case LanguageFeature::rtti:
            return hasRtti();
        case LanguageFeature::threads:
            return hasThreads();
        case LanguageFeature::atomic_operations:
            return hasAtomicOperations();
        case LanguageFeature::inline_assembly:
            return hasInlineAssembly();
        case LanguageFeature::vector_intrinsics:
            return hasVectorIntrinsics();
        case LanguageFeature::stack_protection:
            return hasStackProtection();
        case LanguageFeature::address_sanitizer:
            return hasAddressSanitizer();
        case LanguageFeature::thread_sanitizer:
            return hasThreadSanitizer();
        case LanguageFeature::memory_sanitizer:
            return hasMemorySanitizer();
        case LanguageFeature::undefined_behavior_sanitizer:
            return hasUndefinedBehaviorSanitizer();
        default:
            return false;
    }
}

/**
 * @brief Checks if a specific runtime feature is available
 * @param feature Runtime feature to check
 * @return true if feature is available (requires runtime detection)
 * @note This function is not constexpr as it may require CPU feature detection
 */
inline bool hasRuntimeFeature(RuntimeFeature feature) noexcept {
    switch (feature) {
        case RuntimeFeature::sse:
            return hasSseSupport();
        case RuntimeFeature::sse2:
            return hasSse2Support();
        case RuntimeFeature::sse3:
            return hasSse3Support();
        case RuntimeFeature::sse4_1:
            return hasSse41Support();
        case RuntimeFeature::sse4_2:
            return hasSse42Support();
        case RuntimeFeature::avx:
            return hasAvxSupport();
        case RuntimeFeature::avx2:
            return hasAvx2Support();
        case RuntimeFeature::avx512f:
            return hasAvx512fSupport();
        case RuntimeFeature::neon:
            return hasNeonSupport();
        case RuntimeFeature::hardware_aes:
            return hasHardwareAes();
        case RuntimeFeature::hardware_random:
            return hasHardwareRandom();
        default:
            return false;
    }
}

/**
 * @brief Generic template function for feature testing
 * @tparam TFeature Must be LanguageFeature enum value
 * @return true if the specified language feature is available
 * @note Only works with LanguageFeature enum, not RuntimeFeature
 */
template <LanguageFeature TFeature>
constexpr bool hasFeature() noexcept {
    return hasLanguageFeature(TFeature);
}

}  // namespace platform
}  // namespace trlc

//
// Convenience macros for compile-time features
//

/// Check if C++ exceptions are available
#define TRLC_HAS_EXCEPTIONS (trlc::platform::hasExceptions())

/// Check if RTTI is available
#define TRLC_HAS_RTTI (trlc::platform::hasRtti())

/// Check if threading support is available
#define TRLC_HAS_THREADS (trlc::platform::hasThreads())

/// Check if atomic operations are available
#define TRLC_HAS_ATOMIC (trlc::platform::hasAtomicOperations())

/// Check if inline assembly is available
#define TRLC_HAS_INLINE_ASM (trlc::platform::hasInlineAssembly())

/// Check if vector intrinsics are available
#define TRLC_HAS_VECTOR_INTRINSICS (trlc::platform::hasVectorIntrinsics())

/// Check if stack protection is enabled
#define TRLC_HAS_STACK_PROTECTION (trlc::platform::hasStackProtection())

/// Check if AddressSanitizer is enabled
#define TRLC_HAS_ADDRESS_SANITIZER (trlc::platform::hasAddressSanitizer())

/// Check if ThreadSanitizer is enabled
#define TRLC_HAS_THREAD_SANITIZER (trlc::platform::hasThreadSanitizer())

/// Check if MemorySanitizer is enabled
#define TRLC_HAS_MEMORY_SANITIZER (trlc::platform::hasMemorySanitizer())

/// Check if UndefinedBehaviorSanitizer is enabled
#define TRLC_HAS_UNDEFINED_BEHAVIOR_SANITIZER (trlc::platform::hasUndefinedBehaviorSanitizer())

/// Generic feature test macro
#define TRLC_HAS_FEATURE(X) (trlc::platform::hasFeature<trlc::platform::LanguageFeature::X>())

//
// Runtime feature macros (non-constexpr)
//

/// Check SSE support at runtime
#define TRLC_HAS_SSE_RUNTIME() (trlc::platform::hasSseSupport())

/// Check SSE2 support at runtime
#define TRLC_HAS_SSE2_RUNTIME() (trlc::platform::hasSse2Support())

/// Check SSE3 support at runtime
#define TRLC_HAS_SSE3_RUNTIME() (trlc::platform::hasSse3Support())

/// Check SSE4.1 support at runtime
#define TRLC_HAS_SSE41_RUNTIME() (trlc::platform::hasSse41Support())

/// Check SSE4.2 support at runtime
#define TRLC_HAS_SSE42_RUNTIME() (trlc::platform::hasSse42Support())

/// Check AVX support at runtime
#define TRLC_HAS_AVX_RUNTIME() (trlc::platform::hasAvxSupport())

/// Check AVX2 support at runtime
#define TRLC_HAS_AVX2_RUNTIME() (trlc::platform::hasAvx2Support())

/// Check AVX-512F support at runtime
#define TRLC_HAS_AVX512F_RUNTIME() (trlc::platform::hasAvx512fSupport())

/// Check NEON support at runtime
#define TRLC_HAS_NEON_RUNTIME() (trlc::platform::hasNeonSupport())

/// Check hardware AES support at runtime
#define TRLC_HAS_HARDWARE_AES_RUNTIME() (trlc::platform::hasHardwareAes())

/// Check hardware RNG support at runtime
#define TRLC_HAS_HARDWARE_RANDOM_RUNTIME() (trlc::platform::hasHardwareRandom())

//
// Conditional compilation helpers
//

/// Execute code only if exceptions are available
#ifdef __cpp_exceptions
    #define TRLC_IF_EXCEPTIONS(code) code
#elif defined(__EXCEPTIONS) || defined(_CPPUNWIND)
    #define TRLC_IF_EXCEPTIONS(code) code
#else
    #define TRLC_IF_EXCEPTIONS(code) ((void)0)
#endif

/// Execute code only if RTTI is available
#ifdef __cpp_rtti
    #define TRLC_IF_RTTI(code) code
#elif defined(__GXX_RTTI) || defined(_CPPRTTI)
    #define TRLC_IF_RTTI(code) code
#else
    #define TRLC_IF_RTTI(code) ((void)0)
#endif

/// Execute code only if threading is available
#ifdef __STDCPP_THREADS__
    #if __STDCPP_THREADS__ == 1
        #define TRLC_IF_THREADS(code) code
    #else
        #define TRLC_IF_THREADS(code) ((void)0)
    #endif
#elif defined(_REENTRANT) || defined(_MT)
    #define TRLC_IF_THREADS(code) code
#else
    #define TRLC_IF_THREADS(code) ((void)0)
#endif

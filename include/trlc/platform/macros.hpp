#pragma once

/**
 * @file macros.hpp
 * @brief Portable cross-platform macros for common C++ idioms and optimizations
 *
 * This header provides a comprehensive set of portable macros that abstract
 * compiler-specific features and attributes. All macros gracefully degrade
 * on unsupported compilers while maintaining compatibility.
 *
 * Features:
 * - Portable C++ attributes (nodiscard, deprecated, fallthrough)
 * - Inlining control (force inline, never inline)
 * - Branch prediction hints (likely/unlikely)
 * - Exception safety annotations
 * - Symbol visibility control for shared libraries
 * - Utility macros for conditional compilation
 *
 * @author TRLC Platform Team
 * @version 1.0.0
 */

namespace trlc {
namespace platform {

/**
 * @brief Check if compiler supports nodiscard attribute
 * @return true if nodiscard is supported, false otherwise
 */
constexpr bool supportsNodecard() noexcept {
#ifdef __has_cpp_attribute
    return __has_cpp_attribute(nodiscard) >= 201603L;
#else
    return false;
#endif
}

/**
 * @brief Check if compiler supports deprecated attribute
 * @return true if deprecated is supported, false otherwise
 */
constexpr bool supportsDeprecated() noexcept {
#ifdef __has_cpp_attribute
    return __has_cpp_attribute(deprecated) >= 201309L;
#else
    return false;
#endif
}

/**
 * @brief Check if compiler supports fallthrough attribute
 * @return true if fallthrough is supported, false otherwise
 */
constexpr bool supportsFallthrough() noexcept {
#ifdef __has_cpp_attribute
    return __has_cpp_attribute(fallthrough) >= 201603L;
#else
    return false;
#endif
}

/**
 * @brief Check if compiler supports a specific C++ attribute
 * @param attribute_name Name of the attribute to check
 * @return true if attribute is supported, false otherwise
 */
constexpr bool hasAttributeSupport(const char* /* attribute_name */) noexcept {
    // This is a placeholder for a more sophisticated implementation
    // that could check attribute support by name at compile time
    return false;
}

}  // namespace platform
}  // namespace trlc

// =============================================================================
// Core Attribute Detection
// =============================================================================

/**
 * @brief Check if __has_cpp_attribute is available
 *
 * This macro provides a safe way to check for C++ attribute support.
 * Returns 0 if __has_cpp_attribute is not available.
 */
#ifdef __has_cpp_attribute
    #define TRLC_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
    #define TRLC_HAS_CPP_ATTRIBUTE(x) 0
#endif

/**
 * @brief Check if __has_builtin is available
 *
 * This macro provides a safe way to check for compiler builtin support.
 * Returns 0 if __has_builtin is not available.
 */
#ifdef __has_builtin
    #define TRLC_HAS_BUILTIN(x) __has_builtin(x)
#else
    #define TRLC_HAS_BUILTIN(x) 0
#endif

// =============================================================================
// Portable C++ Attributes
// =============================================================================

/**
 * @brief Portable nodiscard attribute
 *
 * Indicates that the return value of a function should not be discarded.
 * Falls back to compiler-specific attributes or becomes empty on unsupported compilers.
 *
 * @example
 * @code
 * TRLC_NODISCARD int calculate() { return 42; }
 * @endcode
 */
#if TRLC_HAS_CPP_ATTRIBUTE(nodiscard) >= 201603L
    #define TRLC_NODISCARD [[nodiscard]]
#elif defined(__GNUC__) || defined(__clang__)
    #define TRLC_NODISCARD __attribute__((warn_unused_result))
#elif defined(_MSC_VER) && _MSC_VER >= 1700
    #define TRLC_NODISCARD _Check_return_
#else
    #define TRLC_NODISCARD
#endif

/**
 * @brief Portable deprecated attribute
 *
 * Marks a function, class, or variable as deprecated.
 * Generates compiler warnings when the deprecated entity is used.
 *
 * @example
 * @code
 * TRLC_DEPRECATED void oldFunction();
 * @endcode
 */
#if TRLC_HAS_CPP_ATTRIBUTE(deprecated) >= 201309L
    #define TRLC_DEPRECATED [[deprecated]]
#elif defined(__GNUC__) || defined(__clang__)
    #define TRLC_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
    #define TRLC_DEPRECATED __declspec(deprecated)
#else
    #define TRLC_DEPRECATED
#endif

/**
 * @brief Portable deprecated attribute with custom message
 *
 * Marks an entity as deprecated with a custom warning message.
 *
 * @param msg Custom deprecation message
 *
 * @example
 * @code
 * TRLC_DEPRECATED_MSG("Use newFunction() instead") void oldFunction();
 * @endcode
 */
#if TRLC_HAS_CPP_ATTRIBUTE(deprecated) >= 201309L
    #define TRLC_DEPRECATED_MSG(msg) [[deprecated(msg)]]
#elif defined(__GNUC__) || defined(__clang__)
    #define TRLC_DEPRECATED_MSG(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
    #define TRLC_DEPRECATED_MSG(msg) __declspec(deprecated(msg))
#else
    #define TRLC_DEPRECATED_MSG(msg) TRLC_DEPRECATED
#endif

/**
 * @brief Portable fallthrough attribute
 *
 * Indicates intentional fallthrough in switch statements.
 * Suppresses compiler warnings about missing break statements.
 *
 * @example
 * @code
 * switch (value) {
 *     case 1:
 *         doSomething();
 *         TRLC_FALLTHROUGH;
 *     case 2:
 *         doSomethingElse();
 *         break;
 * }
 * @endcode
 */
#if TRLC_HAS_CPP_ATTRIBUTE(fallthrough) >= 201603L
    #define TRLC_FALLTHROUGH [[fallthrough]]
#elif defined(__GNUC__) && __GNUC__ >= 7
    #define TRLC_FALLTHROUGH __attribute__((fallthrough))
#elif defined(__clang__) && TRLC_HAS_CPP_ATTRIBUTE(clang::fallthrough)
    #define TRLC_FALLTHROUGH [[clang::fallthrough]]
#else
    #define TRLC_FALLTHROUGH ((void)0)
#endif

/**
 * @brief Portable maybe_unused attribute
 *
 * Suppresses warnings about unused variables, parameters, or functions.
 *
 * @example
 * @code
 * void function(TRLC_MAYBE_UNUSED int debug_param) {
 *     #ifdef DEBUG
 *         use_debug_param(debug_param);
 *     #endif
 * }
 * @endcode
 */
#if TRLC_HAS_CPP_ATTRIBUTE(maybe_unused) >= 201603L
    #define TRLC_MAYBE_UNUSED [[maybe_unused]]
#elif defined(__GNUC__) || defined(__clang__)
    #define TRLC_MAYBE_UNUSED __attribute__((unused))
#else
    #define TRLC_MAYBE_UNUSED
#endif

// =============================================================================
// Inlining Control
// =============================================================================

/**
 * @brief Force function inlining
 *
 * Strongly suggests to the compiler that a function should be inlined.
 * Use sparingly as it can increase code size and may not always improve performance.
 *
 * @example
 * @code
 * TRLC_FORCE_INLINE int fastFunction() { return 42; }
 * @endcode
 */
#if defined(__GNUC__) || defined(__clang__)
    #define TRLC_FORCE_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
    #define TRLC_FORCE_INLINE __forceinline
#elif defined(__INTEL_COMPILER)
    #define TRLC_FORCE_INLINE __forceinline
#else
    #define TRLC_FORCE_INLINE inline
#endif

/**
 * @brief Prevent function inlining
 *
 * Prevents the compiler from inlining a function.
 * Useful for debugging, profiling, or reducing code size.
 *
 * @example
 * @code
 * TRLC_NEVER_INLINE void debugFunction() { return; }
 * @endcode
 */
#if defined(__GNUC__) || defined(__clang__)
    #define TRLC_NEVER_INLINE __attribute__((noinline))
#elif defined(_MSC_VER)
    #define TRLC_NEVER_INLINE __declspec(noinline)
#elif defined(__INTEL_COMPILER)
    #define TRLC_NEVER_INLINE __declspec(noinline)
#else
    #define TRLC_NEVER_INLINE
#endif

/**
 * @brief Standard inline hint
 *
 * Standard C++ inline keyword. Provided for consistency with other inlining macros.
 *
 * @example
 * @code
 * TRLC_INLINE int normalFunction() { return value; }
 * @endcode
 */
#define TRLC_INLINE inline

// =============================================================================
// Branch Prediction Hints
// =============================================================================

/**
 * @brief Likely branch prediction hint
 *
 * Hints to the compiler that a condition is likely to be true.
 * Can improve performance by optimizing instruction cache usage.
 *
 * @param x Condition expression
 * @return The same condition, potentially with optimization hints
 *
 * @example
 * @code
 * if (TRLC_LIKELY(ptr != nullptr)) {
 *     // Most common case
 * }
 * @endcode
 */
#if defined(__GNUC__) || defined(__clang__)
    #define TRLC_LIKELY(x) __builtin_expect(!!(x), 1)
#elif defined(__INTEL_COMPILER)
    #define TRLC_LIKELY(x) __builtin_expect(!!(x), 1)
#else
    #define TRLC_LIKELY(x) (x)
#endif

/**
 * @brief Unlikely branch prediction hint
 *
 * Hints to the compiler that a condition is unlikely to be true.
 * Useful for error handling paths and rare conditions.
 *
 * @param x Condition expression
 * @return The same condition, potentially with optimization hints
 *
 * @example
 * @code
 * if (TRLC_UNLIKELY(error_occurred)) {
 *     handleError();
 * }
 * @endcode
 */
#if defined(__GNUC__) || defined(__clang__)
    #define TRLC_UNLIKELY(x) __builtin_expect(!!(x), 0)
#elif defined(__INTEL_COMPILER)
    #define TRLC_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
    #define TRLC_UNLIKELY(x) (x)
#endif

// =============================================================================
// Exception Safety
// =============================================================================

/**
 * @brief Conditional noexcept specification
 *
 * Adds noexcept specification if exceptions are enabled.
 * Becomes empty if exceptions are disabled.
 *
 * @example
 * @code
 * void safeFunction() TRLC_NOEXCEPT {
 *     // Function that doesn't throw
 * }
 * @endcode
 */
#if !defined(__cpp_exceptions) || __cpp_exceptions == 0 || \
    defined(_HAS_EXCEPTIONS) && _HAS_EXCEPTIONS == 0
    #define TRLC_NOEXCEPT
    #define TRLC_HAS_EXCEPTIONS_ENABLED 0
#else
    #define TRLC_NOEXCEPT noexcept
    #define TRLC_HAS_EXCEPTIONS_ENABLED 1
#endif

/**
 * @brief Conditional noexcept with condition
 *
 * Adds noexcept specification with a condition if exceptions are enabled.
 *
 * @param cond Boolean expression for noexcept condition
 *
 * @example
 * @code
 * template<typename T>
 * void moveConstruct(T&& t) TRLC_NOEXCEPT_IF(std::is_nothrow_move_constructible_v<T>) {
 *     // ...
 * }
 * @endcode
 */
#if TRLC_HAS_EXCEPTIONS_ENABLED
    #define TRLC_NOEXCEPT_IF(cond) noexcept(cond)
#else
    #define TRLC_NOEXCEPT_IF(cond)
#endif

// =============================================================================
// Symbol Visibility Control
// =============================================================================

/**
 * @brief Export symbols for shared libraries
 *
 * Marks symbols for export in shared libraries/DLLs.
 * Platform-specific implementation with fallback to empty.
 *
 * @example
 * @code
 * class TRLC_API_EXPORT PublicClass {
 * public:
 *     void publicMethod();
 * };
 * @endcode
 */
#if defined(_WIN32) || defined(_WIN64)
    #define TRLC_API_EXPORT __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
    #define TRLC_API_EXPORT __attribute__((visibility("default")))
#elif defined(__INTEL_COMPILER)
    #define TRLC_API_EXPORT __attribute__((visibility("default")))
#else
    #define TRLC_API_EXPORT
#endif

/**
 * @brief Import symbols from shared libraries
 *
 * Marks symbols for import from shared libraries/DLLs.
 * Platform-specific implementation with fallback to empty.
 *
 * @example
 * @code
 * class TRLC_API_IMPORT ExternalClass;
 * @endcode
 */
#if defined(_WIN32) || defined(_WIN64)
    #define TRLC_API_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
    #define TRLC_API_IMPORT __attribute__((visibility("default")))
#elif defined(__INTEL_COMPILER)
    #define TRLC_API_IMPORT __attribute__((visibility("default")))
#else
    #define TRLC_API_IMPORT
#endif

/**
 * @brief Hide symbols from shared library interface
 *
 * Marks symbols as hidden in shared libraries.
 * Used for internal implementation details.
 *
 * @example
 * @code
 * class TRLC_API_HIDDEN InternalClass {
 *     // Implementation details
 * };
 * @endcode
 */
#if defined(__GNUC__) || defined(__clang__)
    #define TRLC_API_HIDDEN __attribute__((visibility("hidden")))
#elif defined(__INTEL_COMPILER)
    #define TRLC_API_HIDDEN __attribute__((visibility("hidden")))
#else
    #define TRLC_API_HIDDEN
#endif

/**
 * @brief Conditional symbol visibility
 *
 * Automatically selects export or import based on whether the library
 * is being built (TRLC_BUILDING_SHARED defined) or used.
 *
 * @example
 * @code
 * class TRLC_API MyLibraryClass {
 * public:
 *     void method();
 * };
 * @endcode
 */
#ifdef TRLC_BUILDING_SHARED
    #define TRLC_API TRLC_API_EXPORT
#else
    #define TRLC_API TRLC_API_IMPORT
#endif

// =============================================================================
// C++ Standard Detection
// =============================================================================

/**
 * @brief C++17 feature detection
 *
 * Detects if C++17 features are available.
 * Works independently from cpp_standard.hpp for macros-only usage.
 */
#if __cplusplus >= 201703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
    #define TRLC_HAS_CPP17 1
#else
    #define TRLC_HAS_CPP17 0
#endif

// =============================================================================
// Utility Macros
// =============================================================================

/**
 * @brief Conditional constexpr for if statements
 *
 * This macro is defined in cpp_standard.hpp and is available here for consistency.
 * It adds constexpr to if statements in C++17 and later.
 *
 * @note This macro is defined in cpp_standard.hpp - no redefinition needed here
 *
 * @example
 * @code
 * template<typename T>
 * void process() {
 *     if TRLC_CONSTEXPR_IF (std::is_integral_v<T>) {
 *         // Integer-specific processing
 *     } else {
 *         // Generic processing
 *     }
 * }
 * @endcode
 */
// TRLC_CONSTEXPR_IF is defined in cpp_standard.hpp

/**
 * @brief Portable restrict keyword
 *
 * Provides the restrict keyword functionality across different compilers.
 * Helps with pointer aliasing optimizations.
 *
 * @example
 * @code
 * void process(int* TRLC_RESTRICT input, int* TRLC_RESTRICT output);
 * @endcode
 */
#if defined(__GNUC__) || defined(__clang__)
    #define TRLC_RESTRICT __restrict__
#elif defined(_MSC_VER)
    #define TRLC_RESTRICT __restrict
#elif defined(__INTEL_COMPILER)
    #define TRLC_RESTRICT __restrict
#else
    #define TRLC_RESTRICT
#endif

/**
 * @brief Packed structure attribute
 *
 * Forces tight packing of structure members without padding.
 * Use with caution as it can affect alignment and performance.
 *
 * @example
 * @code
 * struct TRLC_PACKED {
 *     char a;
 *     int b;
 * } PackedStruct;
 * @endcode
 */
#if defined(__GNUC__) || defined(__clang__)
    #define TRLC_PACKED __attribute__((packed))
#elif defined(_MSC_VER)
    #define TRLC_PACKED
    #define TRLC_PACK_BEGIN __pragma(pack(push, 1))
    #define TRLC_PACK_END __pragma(pack(pop))
#else
    #define TRLC_PACKED
    #define TRLC_PACK_BEGIN
    #define TRLC_PACK_END
#endif

#ifndef TRLC_PACK_BEGIN
    #define TRLC_PACK_BEGIN
    #define TRLC_PACK_END
#endif

/**
 * @brief Alignment specification
 *
 * Specifies minimum alignment for variables or types.
 *
 * @param bytes Alignment in bytes (must be power of 2)
 *
 * @example
 * @code
 * TRLC_ALIGNED(16) float vector[4];
 * @endcode
 */
#if defined(__GNUC__) || defined(__clang__)
    #define TRLC_ALIGNED(bytes) __attribute__((aligned(bytes)))
#elif defined(_MSC_VER)
    #define TRLC_ALIGNED(bytes) __declspec(align(bytes))
#elif defined(__INTEL_COMPILER)
    #define TRLC_ALIGNED(bytes) __declspec(align(bytes))
#else
    #define TRLC_ALIGNED(bytes)
#endif

/**
 * @brief Cache line alignment
 *
 * Aligns variables to cache line boundaries to avoid false sharing.
 *
 * @example
 * @code
 * TRLC_CACHE_ALIGNED int counter;
 * @endcode
 */
#define TRLC_CACHE_ALIGNED TRLC_ALIGNED(64)

/**
 * @brief Disable specific compiler warnings
 *
 * Macros for selectively disabling compiler warnings.
 * Use sparingly and with clear justification.
 *
 * @example
 * @code
 * TRLC_DIAGNOSTIC_PUSH
 * TRLC_DIAGNOSTIC_IGNORE_UNUSED_PARAMETER
 * void func(int unused_param) { }
 * TRLC_DIAGNOSTIC_POP
 * @endcode
 */
#if defined(__GNUC__) || defined(__clang__)
    #define TRLC_DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
    #define TRLC_DIAGNOSTIC_POP _Pragma("GCC diagnostic pop")
    #define TRLC_DIAGNOSTIC_IGNORE_UNUSED_PARAMETER \
        _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")
    #define TRLC_DIAGNOSTIC_IGNORE_UNUSED_VARIABLE \
        _Pragma("GCC diagnostic ignored \"-Wunused-variable\"")
    #define TRLC_DIAGNOSTIC_IGNORE_UNUSED_FUNCTION \
        _Pragma("GCC diagnostic ignored \"-Wunused-function\"")
#elif defined(_MSC_VER)
    #define TRLC_DIAGNOSTIC_PUSH __pragma(warning(push))
    #define TRLC_DIAGNOSTIC_POP __pragma(warning(pop))
    #define TRLC_DIAGNOSTIC_IGNORE_UNUSED_PARAMETER __pragma(warning(disable : 4100))
    #define TRLC_DIAGNOSTIC_IGNORE_UNUSED_VARIABLE __pragma(warning(disable : 4101))
    #define TRLC_DIAGNOSTIC_IGNORE_UNUSED_FUNCTION __pragma(warning(disable : 4505))
#else
    #define TRLC_DIAGNOSTIC_PUSH
    #define TRLC_DIAGNOSTIC_POP
    #define TRLC_DIAGNOSTIC_IGNORE_UNUSED_PARAMETER
    #define TRLC_DIAGNOSTIC_IGNORE_UNUSED_VARIABLE
    #define TRLC_DIAGNOSTIC_IGNORE_UNUSED_FUNCTION
#endif

// =============================================================================
// Conditional Compilation Helpers
// =============================================================================

/**
 * @brief Execute code only if condition is true at compile time
 *
 * Provides a way to conditionally execute code based on compile-time conditions.
 *
 * @param condition Compile-time boolean condition
 * @param code Code to execute if condition is true
 *
 * @example
 * @code
 * TRLC_IF(TRLC_HAS_EXCEPTIONS,
 *     try { riskyOperation(); } catch(...) { handleError(); }
 * )
 * @endcode
 */
#define TRLC_IF(condition, code)   \
    do {                           \
        if constexpr (condition) { \
            code;                  \
        }                          \
    } while (0)

/**
 * @brief Execute different code based on compile-time condition
 *
 * Provides if-else functionality for compile-time conditions.
 *
 * @param condition Compile-time boolean condition
 * @param then_code Code to execute if condition is true
 * @param else_code Code to execute if condition is false
 *
 * @example
 * @code
 * TRLC_IF_ELSE(TRLC_HAS_EXCEPTIONS,
 *     { useExceptions(); },
 *     { useErrorCodes(); }
 * )
 * @endcode
 */
#define TRLC_IF_ELSE(condition, then_code, else_code) \
    do {                                              \
        if constexpr (condition) {                    \
            then_code;                                \
        } else {                                      \
            else_code;                                \
        }                                             \
    } while (0)

// =============================================================================
// Compiler Bug Workarounds
// =============================================================================

/**
 * @brief Workaround for GCC's sometimes-incorrect unused variable warnings
 *
 * Use this to explicitly mark variables as used when GCC incorrectly
 * flags them as unused.
 *
 * @param var Variable to mark as used
 *
 * @example
 * @code
 * void func() {
 *     int result = complexCalculation();
 *     assert(result > 0);  // Only used in debug builds
 *     TRLC_USE(result);    // Prevent unused variable warning
 * }
 * @endcode
 */
#define TRLC_USE(var) ((void)(var))

/**
 * @brief Indicate unreachable code paths
 *
 * Marks code paths that should never be reached.
 * Helps with compiler optimizations and debugging.
 *
 * @example
 * @code
 * switch (value) {
 *     case 1: return handleOne();
 *     case 2: return handleTwo();
 *     default: TRLC_UNREACHABLE();
 * }
 * @endcode
 */
#if defined(__GNUC__) || defined(__clang__)
    #define TRLC_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
    #define TRLC_UNREACHABLE() __assume(0)
#else
    #define TRLC_UNREACHABLE() ((void)0)
#endif

/**
 * @brief Compile-time string concatenation for macros
 *
 * Helper macros for concatenating strings and tokens in macros.
 *
 * @example
 * @code
 * #define DECLARE_GETTER(type, name) \
 *     type TRLC_CONCAT(get, name)() const { return name##_; }
 * @endcode
 */
#define TRLC_STRINGIFY(x) #x
#define TRLC_STRINGIFY_EXPANDED(x) TRLC_STRINGIFY(x)
#define TRLC_CONCAT(a, b) a##b
#define TRLC_CONCAT_EXPANDED(a, b) TRLC_CONCAT(a, b)

// =============================================================================
// Feature Test Helpers
// =============================================================================

/**
 * @brief Test if a feature is available and enabled
 *
 * Provides a consistent way to test for feature availability
 * across the entire platform detection system.
 *
 * @param feature Feature identifier from the features module
 *
 * @example
 * @code
 * #if TRLC_FEATURE_AVAILABLE(EXCEPTIONS_ENABLED)
 *     // Use exceptions
 * #else
 *     // Use error codes
 * #endif
 * @endcode
 */
#define TRLC_FEATURE_AVAILABLE(feature) (TRLC_HAS_##feature)

/**
 * @brief Platform-specific code selection
 *
 * Provides convenient macros for platform-specific implementations.
 *
 * @example
 * @code
 * void platformSpecificFunction() {
 *     TRLC_ON_WINDOWS({
 *         // Windows-specific code
 *     })
 *     TRLC_ON_POSIX({
 *         // POSIX-specific code
 *     })
 * }
 * @endcode
 */
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    #define TRLC_ON_WINDOWS(code) code
    #define TRLC_ON_POSIX(code)
    #define TRLC_PLATFORM_WINDOWS 1
    #define TRLC_PLATFORM_POSIX 0
#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    #define TRLC_ON_WINDOWS(code)
    #define TRLC_ON_POSIX(code) code
    #define TRLC_PLATFORM_WINDOWS 0
    #define TRLC_PLATFORM_POSIX 1
#else
    #define TRLC_ON_WINDOWS(code)
    #define TRLC_ON_POSIX(code)
    #define TRLC_PLATFORM_WINDOWS 0
    #define TRLC_PLATFORM_POSIX 0
#endif

/**
 * @brief Architecture-specific code selection
 *
 * Provides convenient macros for architecture-specific implementations.
 *
 * @example
 * @code
 * void optimizedFunction() {
 *     TRLC_ON_X86_64({
 *         // x86_64 optimized implementation
 *     })
 *     TRLC_ON_ARM64({
 *         // ARM64 optimized implementation
 *     })
 * }
 * @endcode
 */
#if defined(__x86_64__) || defined(__x86_64) || defined(__amd64__) || defined(__amd64) || \
    defined(_M_X64)
    #define TRLC_ON_X86_64(code) code
    #define TRLC_ARCH_X86_64 1
#else
    #define TRLC_ON_X86_64(code)
    #define TRLC_ARCH_X86_64 0
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
    #define TRLC_ON_ARM64(code) code
    #define TRLC_ARCH_ARM64 1
#else
    #define TRLC_ON_ARM64(code)
    #define TRLC_ARCH_ARM64 0
#endif

// =============================================================================
// Version and Compatibility
// =============================================================================

/**
 * @brief Macro version information
 *
 * Allows other components to check the version of the macros module.
 */
#define TRLC_MACROS_VERSION_MAJOR 1
#define TRLC_MACROS_VERSION_MINOR 0
#define TRLC_MACROS_VERSION_PATCH 0
#define TRLC_MACROS_VERSION_STRING "1.0.0"

/**
 * @brief Check if macros module version is at least the specified version
 *
 * @param major Major version
 * @param minor Minor version
 * @param patch Patch version
 *
 * @example
 * @code
 * #if TRLC_MACROS_VERSION_AT_LEAST(1, 0, 0)
 *     // Use features from version 1.0.0 or later
 * #endif
 * @endcode
 */
#define TRLC_MACROS_VERSION_AT_LEAST(major, minor, patch)                             \
    ((TRLC_MACROS_VERSION_MAJOR > (major)) ||                                         \
     (TRLC_MACROS_VERSION_MAJOR == (major) && TRLC_MACROS_VERSION_MINOR > (minor)) || \
     (TRLC_MACROS_VERSION_MAJOR == (major) && TRLC_MACROS_VERSION_MINOR == (minor) && \
      TRLC_MACROS_VERSION_PATCH >= (patch)))

// Mark this header as successfully included
#define TRLC_MACROS_INCLUDED

// =============================================================================
// End of macros.hpp
// =============================================================================

/**
 * @file cpp_standard.hpp
 * @brief C++ standard version and feature detection for trlc-platform
 *
 * This header provides compile-time detection of the active C++ standard version
 * and availability of standard-specific language features. All functions are
 * constexpr and evaluate at compile time for zero runtime overhead.
 *
 * @author trlc-platform
 * @version 1.0.0
 */

#pragma once

namespace trlc {
namespace platform {

/**
 * @brief C++ standard enumeration
 *
 * Represents different C++ standard versions with their corresponding
 * __cplusplus macro values for precise version detection.
 */
enum class CppStandard : long {
    cpp_pre_17 = 0,    ///< Pre-C++17 standards (C++98/03/11/14)
    cpp_17 = 201703L,  ///< C++17 standard
    cpp_20 = 202002L,  ///< C++20 standard
    cpp_23 = 202302L,  ///< C++23 standard
    cpp_26 = 202600L,  ///< C++26 standard (tentative)
    cpp_unknown = -1   ///< Unknown or unsupported standard
};

/**
 * @brief C++ standard information structure
 *
 * Contains comprehensive information about the detected C++ standard,
 * including version, macro value, and feature support capabilities.
 */
struct CppStandardInfo {
    CppStandard standard;       ///< Detected C++ standard
    long version_macro;         ///< Value of __cplusplus macro
    const char* standard_name;  ///< Human-readable standard name

    /**
     * @brief Checks if the current standard is at least the specified version
     * @param min_standard Minimum required C++ standard
     * @return true if current standard >= min_standard
     */
    constexpr bool isAtLeast(CppStandard min_standard) const noexcept {
        return static_cast<long>(standard) >= static_cast<long>(min_standard);
    }

    /**
     * @brief Checks if a specific language feature is supported
     * @param feature_name Name of the feature to check
     * @return true if feature is supported in this standard
     */
    constexpr bool supportsFeature(const char* feature_name) const noexcept {
        // Note: This is a simplified implementation. In practice, this could
        // be extended with a more sophisticated feature mapping system.

        // Compare strings at compile time (simplified for key features)
        if (feature_name[0] == 's' && feature_name[1] == 't' && feature_name[2] == 'r' &&
            feature_name[3] == 'u' && feature_name[4] == 'c' && feature_name[5] == 't') {
            // "structured_bindings"
            return isAtLeast(CppStandard::cpp_17);
        }
        if (feature_name[0] == 'i' && feature_name[1] == 'f' && feature_name[2] == '_' &&
            feature_name[3] == 'c' && feature_name[4] == 'o' && feature_name[5] == 'n') {
            // "if_constexpr"
            return isAtLeast(CppStandard::cpp_17);
        }
        if (feature_name[0] == 'c' && feature_name[1] == 'o' && feature_name[2] == 'n' &&
            feature_name[3] == 'c' && feature_name[4] == 'e' && feature_name[5] == 'p') {
            // "concepts"
            return isAtLeast(CppStandard::cpp_20);
        }
        if (feature_name[0] == 'c' && feature_name[1] == 'o' && feature_name[2] == 'r' &&
            feature_name[3] == 'o' && feature_name[4] == 'u' && feature_name[5] == 't') {
            // "coroutines"
            return isAtLeast(CppStandard::cpp_20);
        }
        if (feature_name[0] == 'm' && feature_name[1] == 'o' && feature_name[2] == 'd' &&
            feature_name[3] == 'u' && feature_name[4] == 'l' && feature_name[5] == 'e') {
            // "modules"
            return isAtLeast(CppStandard::cpp_20);
        }
        if (feature_name[0] == 'r' && feature_name[1] == 'a' && feature_name[2] == 'n' &&
            feature_name[3] == 'g' && feature_name[4] == 'e' && feature_name[5] == 's') {
            // "ranges"
            return isAtLeast(CppStandard::cpp_20);
        }

        return false;  // Unknown feature
    }
};

namespace detail {

/**
 * @brief Detects C++ standard using __cplusplus macro
 * @return CppStandard enum value
 */
constexpr CppStandard detectCppStandard() noexcept {
    // Use _MSVC_LANG for MSVC, __cplusplus for others
#if defined(_MSVC_LANG)
    constexpr long cpp_version = _MSVC_LANG;
#elif defined(__cplusplus)
    constexpr long cpp_version = __cplusplus;
#else
    constexpr long cpp_version = 0L;
#endif

    if (cpp_version >= 202600L) {
        return CppStandard::cpp_26;
    } else if (cpp_version >= 202302L) {
        return CppStandard::cpp_23;
    } else if (cpp_version >= 202002L) {
        return CppStandard::cpp_20;
    } else if (cpp_version >= 201703L) {
        return CppStandard::cpp_17;
    } else if (cpp_version >= 201402L || cpp_version >= 201103L || cpp_version >= 199711L) {
        return CppStandard::cpp_pre_17;
    } else {
        return CppStandard::cpp_unknown;
    }
}

/**
 * @brief Gets the __cplusplus macro value
 * @return Value of __cplusplus macro (or _MSVC_LANG for MSVC)
 */
constexpr long getCppVersionMacroValue() noexcept {
#if defined(_MSVC_LANG)
    return _MSVC_LANG;  // MSVC uses _MSVC_LANG instead of __cplusplus
#elif defined(__cplusplus)
    return __cplusplus;
#else
    return 0L;
#endif
}

/**
 * @brief Gets human-readable standard name
 * @return Standard name string
 */
constexpr const char* getCppStandardName() noexcept {
    constexpr auto standard = detectCppStandard();
    switch (standard) {
        case CppStandard::cpp_17:
            return "C++17";
        case CppStandard::cpp_20:
            return "C++20";
        case CppStandard::cpp_23:
            return "C++23";
        case CppStandard::cpp_26:
            return "C++26";
        case CppStandard::cpp_pre_17:
            return "Pre-C++17";
        default:
            return "Unknown";
    }
}

}  // namespace detail

/**
 * @brief Gets the current C++ standard version
 * @return CppStandard enum value representing the detected standard
 */
constexpr CppStandard getCppStandard() noexcept {
    return detail::detectCppStandard();
}

/**
 * @brief Gets comprehensive C++ standard information
 * @return CppStandardInfo structure with all detected information
 */
constexpr CppStandardInfo getCppStandardInfo() noexcept {
    return CppStandardInfo{detail::detectCppStandard(),
                           detail::getCppVersionMacroValue(),
                           detail::getCppStandardName()};
}

/**
 * @brief Gets the __cplusplus macro value
 * @return Value of __cplusplus macro
 */
constexpr long getCppVersionMacro() noexcept {
    return detail::getCppVersionMacroValue();
}

// Standard feature detection functions

/**
 * @brief Checks if structured bindings are supported (C++17)
 * @return true if structured bindings are available
 */
constexpr bool hasStructuredBindings() noexcept {
#ifdef __cpp_structured_bindings
    return __cpp_structured_bindings >= 201606L;
#else
    return getCppStandard() >= CppStandard::cpp_17;
#endif
}

/**
 * @brief Checks if if constexpr is supported (C++17)
 * @return true if if constexpr is available
 */
constexpr bool hasIfConstexpr() noexcept {
#ifdef __cpp_if_constexpr
    return __cpp_if_constexpr >= 201606L;
#else
    return getCppStandard() >= CppStandard::cpp_17;
#endif
}

/**
 * @brief Checks if concepts are supported (C++20)
 * @return true if concepts are available
 */
constexpr bool hasConcepts() noexcept {
#ifdef __cpp_concepts
    return __cpp_concepts >= 201907L;
#else
    return getCppStandard() >= CppStandard::cpp_20;
#endif
}

/**
 * @brief Checks if coroutines are supported (C++20)
 * @return true if coroutines are available
 */
constexpr bool hasCoroutines() noexcept {
#ifdef __cpp_impl_coroutine
    return __cpp_impl_coroutine >= 201902L;
#elif defined(__cpp_coroutines)
    return __cpp_coroutines >= 201902L;
#else
    return getCppStandard() >= CppStandard::cpp_20;
#endif
}

/**
 * @brief Checks if modules are supported (C++20)
 * @return true if modules are available
 */
constexpr bool hasModules() noexcept {
#ifdef __cpp_modules
    return __cpp_modules >= 201907L;
#else
    // Modules support varies significantly by compiler, be conservative
    return false;
#endif
}

/**
 * @brief Checks if ranges library is supported (C++20)
 * @return true if ranges are available
 */
constexpr bool hasRanges() noexcept {
#ifdef __cpp_lib_ranges
    return __cpp_lib_ranges >= 201911L;
#else
    return getCppStandard() >= CppStandard::cpp_20;
#endif
}

/**
 * @brief Checks if fold expressions are supported (C++17)
 * @return true if fold expressions are available
 */
constexpr bool hasFoldExpressions() noexcept {
#ifdef __cpp_fold_expressions
    return __cpp_fold_expressions >= 201603L;
#else
    return getCppStandard() >= CppStandard::cpp_17;
#endif
}

/**
 * @brief Checks if inline variables are supported (C++17)
 * @return true if inline variables are available
 */
constexpr bool hasInlineVariables() noexcept {
#ifdef __cpp_inline_variables
    return __cpp_inline_variables >= 201606L;
#else
    return getCppStandard() >= CppStandard::cpp_17;
#endif
}

/**
 * @brief Checks if consteval is supported (C++20)
 * @return true if consteval is available
 */
constexpr bool hasConsteval() noexcept {
#ifdef __cpp_consteval
    return __cpp_consteval >= 201811L;
#else
    return getCppStandard() >= CppStandard::cpp_20;
#endif
}

/**
 * @brief Checks if constinit is supported (C++20)
 * @return true if constinit is available
 */
constexpr bool hasConstinit() noexcept {
#ifdef __cpp_constinit
    return __cpp_constinit >= 201907L;
#else
    return getCppStandard() >= CppStandard::cpp_20;
#endif
}

/**
 * @brief Checks if designated initializers are supported (C++20)
 * @return true if designated initializers are available
 */
constexpr bool hasDesignatedInitializers() noexcept {
#ifdef __cpp_designated_initializers
    return __cpp_designated_initializers >= 201707L;
#else
    return getCppStandard() >= CppStandard::cpp_20;
#endif
}

/**
 * @brief Checks if three-way comparison (spaceship operator) is supported (C++20)
 * @return true if three-way comparison is available
 */
constexpr bool hasThreeWayComparison() noexcept {
#ifdef __cpp_impl_three_way_comparison
    return __cpp_impl_three_way_comparison >= 201907L;
#else
    return getCppStandard() >= CppStandard::cpp_20;
#endif
}

/**
 * @brief Checks if the current C++ standard is at least C++17
 * @return true if C++17 or later
 */
constexpr bool isCpp17OrLater() noexcept {
    return getCppStandard() >= CppStandard::cpp_17;
}

/**
 * @brief Checks if the current C++ standard is at least C++20
 * @return true if C++20 or later
 */
constexpr bool isCpp20OrLater() noexcept {
    return getCppStandard() >= CppStandard::cpp_20;
}

/**
 * @brief Checks if the current C++ standard is at least C++23
 * @return true if C++23 or later
 */
constexpr bool isCpp23OrLater() noexcept {
    return getCppStandard() >= CppStandard::cpp_23;
}

/**
 * @brief Checks if the current C++ standard is exactly C++17
 * @return true if exactly C++17
 */
constexpr bool isExactlyCpp17() noexcept {
    return getCppStandard() == CppStandard::cpp_17;
}

/**
 * @brief Checks if the current C++ standard is exactly C++20
 * @return true if exactly C++20
 */
constexpr bool isExactlyCpp20() noexcept {
    return getCppStandard() == CppStandard::cpp_20;
}

/**
 * @brief Checks if the current C++ standard is exactly C++23
 * @return true if exactly C++23
 */
constexpr bool isExactlyCpp23() noexcept {
    return getCppStandard() == CppStandard::cpp_23;
}

}  // namespace platform
}  // namespace trlc

// Convenience macros for C++ standard detection (preprocessor-compatible)

// Standard version macros
#if defined(__cplusplus) && __cplusplus >= 201703L
    #define TRLC_CPP17 1
#else
    #define TRLC_CPP17 0
#endif

#if defined(__cplusplus) && __cplusplus >= 202002L
    #define TRLC_CPP20 1
#else
    #define TRLC_CPP20 0
#endif

#if defined(__cplusplus) && __cplusplus >= 202302L
    #define TRLC_CPP23 1
#else
    #define TRLC_CPP23 0
#endif

#if defined(__cplusplus) && __cplusplus >= 202600L
    #define TRLC_CPP26 1
#else
    #define TRLC_CPP26 0
#endif

// Version macro value
#ifdef __cplusplus
    #define TRLC_CPP_VERSION __cplusplus
#else
    #define TRLC_CPP_VERSION 0L
#endif

// Feature detection macros (preprocessor-compatible)
#ifdef __cpp_structured_bindings
    #if __cpp_structured_bindings >= 201606L
        #define TRLC_HAS_STRUCTURED_BINDINGS 1
    #else
        #define TRLC_HAS_STRUCTURED_BINDINGS 0
    #endif
#else
    #define TRLC_HAS_STRUCTURED_BINDINGS TRLC_CPP17
#endif

#ifdef __cpp_if_constexpr
    #if __cpp_if_constexpr >= 201606L
        #define TRLC_HAS_IF_CONSTEXPR 1
    #else
        #define TRLC_HAS_IF_CONSTEXPR 0
    #endif
#else
    #define TRLC_HAS_IF_CONSTEXPR TRLC_CPP17
#endif

#ifdef __cpp_concepts
    #if __cpp_concepts >= 201907L
        #define TRLC_HAS_CONCEPTS 1
    #else
        #define TRLC_HAS_CONCEPTS 0
    #endif
#else
    #define TRLC_HAS_CONCEPTS TRLC_CPP20
#endif

#ifdef __cpp_impl_coroutine
    #if __cpp_impl_coroutine >= 201902L
        #define TRLC_HAS_COROUTINES 1
    #else
        #define TRLC_HAS_COROUTINES 0
    #endif
#elif defined(__cpp_coroutines)
    #if __cpp_coroutines >= 201902L
        #define TRLC_HAS_COROUTINES 1
    #else
        #define TRLC_HAS_COROUTINES 0
    #endif
#else
    #define TRLC_HAS_COROUTINES TRLC_CPP20
#endif

#ifdef __cpp_modules
    #if __cpp_modules >= 201907L
        #define TRLC_HAS_MODULES 1
    #else
        #define TRLC_HAS_MODULES 0
    #endif
#else
    #define TRLC_HAS_MODULES 0  // Conservative - modules support varies
#endif

#ifdef __cpp_lib_ranges
    #if __cpp_lib_ranges >= 201911L
        #define TRLC_HAS_RANGES 1
    #else
        #define TRLC_HAS_RANGES 0
    #endif
#else
    #define TRLC_HAS_RANGES TRLC_CPP20
#endif

#ifdef __cpp_fold_expressions
    #if __cpp_fold_expressions >= 201603L
        #define TRLC_HAS_FOLD_EXPRESSIONS 1
    #else
        #define TRLC_HAS_FOLD_EXPRESSIONS 0
    #endif
#else
    #define TRLC_HAS_FOLD_EXPRESSIONS TRLC_CPP17
#endif

#ifdef __cpp_consteval
    #if __cpp_consteval >= 201811L
        #define TRLC_HAS_CONSTEVAL 1
    #else
        #define TRLC_HAS_CONSTEVAL 0
    #endif
#else
    #define TRLC_HAS_CONSTEVAL TRLC_CPP20
#endif

// Conditional compilation helpers
#if TRLC_CPP17
    #define TRLC_IF_CPP17(code) code
#else
    #define TRLC_IF_CPP17(code)
#endif

#if TRLC_CPP20
    #define TRLC_IF_CPP20(code) code
#else
    #define TRLC_IF_CPP20(code)
#endif

#if TRLC_CPP23
    #define TRLC_IF_CPP23(code) code
#else
    #define TRLC_IF_CPP23(code)
#endif

// Constexpr utility macros
#if TRLC_HAS_IF_CONSTEXPR
    #define TRLC_CONSTEXPR_IF(...) if constexpr (__VA_ARGS__)
#else
    #define TRLC_CONSTEXPR_IF(...) if (__VA_ARGS__)
#endif

// Feature-specific conditional compilation
#if TRLC_HAS_STRUCTURED_BINDINGS
    #define TRLC_IF_STRUCTURED_BINDINGS(code) code
#else
    #define TRLC_IF_STRUCTURED_BINDINGS(code)
#endif

#if TRLC_HAS_CONCEPTS
    #define TRLC_IF_CONCEPTS(code) code
#else
    #define TRLC_IF_CONCEPTS(code)
#endif

#if TRLC_HAS_COROUTINES
    #define TRLC_IF_COROUTINES(code) code
#else
    #define TRLC_IF_COROUTINES(code)
#endif

#if TRLC_HAS_RANGES
    #define TRLC_IF_RANGES(code) code
#else
    #define TRLC_IF_RANGES(code)
#endif

// Standard library feature availability
#if TRLC_CPP17
    #define TRLC_HAS_STRING_VIEW 1
    #define TRLC_HAS_OPTIONAL 1
    #define TRLC_HAS_VARIANT 1
    #define TRLC_HAS_ANY 1
    #define TRLC_HAS_FILESYSTEM 1
#else
    #define TRLC_HAS_STRING_VIEW 0
    #define TRLC_HAS_OPTIONAL 0
    #define TRLC_HAS_VARIANT 0
    #define TRLC_HAS_ANY 0
    #define TRLC_HAS_FILESYSTEM 0
#endif

#if TRLC_CPP20
    #define TRLC_HAS_SPAN 1
    #define TRLC_HAS_FORMAT 1
    #define TRLC_HAS_CONCEPTS_LIB 1
    #define TRLC_HAS_CHRONO_CALENDAR 1
#else
    #define TRLC_HAS_SPAN 0
    #define TRLC_HAS_FORMAT 0
    #define TRLC_HAS_CONCEPTS_LIB 0
    #define TRLC_HAS_CHRONO_CALENDAR 0
#endif

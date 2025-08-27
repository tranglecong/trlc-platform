/**
 * @file traits.hpp
 * @brief Advanced template specializations and metaprogramming utilities
 *
 * This header provides template specializations and advanced metaprogramming
 * utilities for feature detection, extending the basic feature detection
 * capabilities with type-safe compile-time and platform-specific features.
 *
 * @author TRLC Platform Team
 * @date 2025
 * @copyright MIT License
 */

#pragma once

#include <cstddef>
#include <type_traits>

#include "trlc/platform/features.hpp"

namespace trlc {
namespace platform {

// =============================================================================
// Template Specializations for hasFeature
// =============================================================================

/**
 * @brief Template specializations for specific language features
 * Provides optimized compile-time constants for individual features
 */

/// Specialization for exceptions - Direct function call for better optimization
template <>
constexpr bool hasFeature<LanguageFeature::exceptions>() noexcept {
    return hasExceptions();
}

/// Specialization for RTTI - Direct function call for better optimization
template <>
constexpr bool hasFeature<LanguageFeature::rtti>() noexcept {
    return hasRtti();
}

/// Specialization for threads - Direct function call for better optimization
template <>
constexpr bool hasFeature<LanguageFeature::threads>() noexcept {
    return hasThreads();
}

/// Specialization for atomic operations - Direct function call for better optimization
template <>
constexpr bool hasFeature<LanguageFeature::atomic_operations>() noexcept {
    return hasAtomicOperations();
}

/// Specialization for inline assembly - Direct function call for better optimization
template <>
constexpr bool hasFeature<LanguageFeature::inline_assembly>() noexcept {
    return hasInlineAssembly();
}

/// Specialization for vector intrinsics - Direct function call for better optimization
template <>
constexpr bool hasFeature<LanguageFeature::vector_intrinsics>() noexcept {
    return hasVectorIntrinsics();
}

/// Specialization for stack protection - Direct function call for better optimization
template <>
constexpr bool hasFeature<LanguageFeature::stack_protection>() noexcept {
    return hasStackProtection();
}

/// Specialization for AddressSanitizer - Direct function call for better optimization
template <>
constexpr bool hasFeature<LanguageFeature::address_sanitizer>() noexcept {
    return hasAddressSanitizer();
}

/// Specialization for ThreadSanitizer - Direct function call for better optimization
template <>
constexpr bool hasFeature<LanguageFeature::thread_sanitizer>() noexcept {
    return hasThreadSanitizer();
}

/// Specialization for MemorySanitizer - Direct function call for better optimization
template <>
constexpr bool hasFeature<LanguageFeature::memory_sanitizer>() noexcept {
    return hasMemorySanitizer();
}

// =============================================================================
// Runtime Feature Template Functions
// =============================================================================

/**
 * @brief Template function for runtime feature testing
 * @tparam TFeature Must be RuntimeFeature enum value
 * @return true if the specified runtime feature is available
 */
template <RuntimeFeature TFeature>
bool hasRuntimeFeature() noexcept {
    return getFeatureSet().hasRuntimeFeature(TFeature);
}

// Specializations for common runtime features (for better optimization)
template <>
inline bool hasRuntimeFeature<RuntimeFeature::sse>() noexcept {
    return hasSseSupport();
}

template <>
inline bool hasRuntimeFeature<RuntimeFeature::sse2>() noexcept {
    return hasSse2Support();
}

template <>
inline bool hasRuntimeFeature<RuntimeFeature::sse3>() noexcept {
    return hasSse3Support();
}

template <>
inline bool hasRuntimeFeature<RuntimeFeature::sse4_1>() noexcept {
    return hasSse41Support();
}

template <>
inline bool hasRuntimeFeature<RuntimeFeature::sse4_2>() noexcept {
    return hasSse42Support();
}

template <>
inline bool hasRuntimeFeature<RuntimeFeature::avx>() noexcept {
    return hasAvxSupport();
}

template <>
inline bool hasRuntimeFeature<RuntimeFeature::avx2>() noexcept {
    return hasAvx2Support();
}

template <>
inline bool hasRuntimeFeature<RuntimeFeature::avx512f>() noexcept {
    return hasAvx512fSupport();
}

template <>
inline bool hasRuntimeFeature<RuntimeFeature::neon>() noexcept {
    return hasNeonSupport();
}

namespace traits {

// =============================================================================
// C++17 Compatibility Helpers
// =============================================================================

#if __cplusplus >= 201703L
// C++17 has these in std
template <bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template <bool B>
using bool_constant = std::integral_constant<bool, B>;
#else
// C++14 compatibility
template <bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template <bool B>
using bool_constant = std::integral_constant<bool, B>;
#endif

// =============================================================================
// SFINAE Helpers for Template Constraints
// =============================================================================

/**
 * @brief SFINAE helper to enable template only if feature is available
 * @tparam TFeature LanguageFeature to check
 */
template <LanguageFeature TFeature>
using enable_if_feature_t = enable_if_t<hasFeature<TFeature>()>;

/**
 * @brief SFINAE helper to enable template only if feature is NOT available
 * @tparam TFeature LanguageFeature to check
 */
template <LanguageFeature TFeature>
using enable_if_no_feature_t = enable_if_t<!hasFeature<TFeature>()>;

/**
 * @brief SFINAE helper to enable template only if ALL features are available
 * @tparam Features Variadic LanguageFeatures to check
 */
template <LanguageFeature... Features>
using enable_if_all_features_t = enable_if_t<(hasFeature<Features>() && ...)>;

/**
 * @brief SFINAE helper to enable template only if ANY feature is available
 * @tparam Features Variadic LanguageFeatures to check
 */
template <LanguageFeature... Features>
using enable_if_any_feature_t = enable_if_t<(hasFeature<Features>() || ...)>;

// =============================================================================
// Variadic Template Functions for Multi-Feature Requirements
// =============================================================================

/**
 * @brief Check if ALL specified features are available
 * @tparam Features Variadic list of LanguageFeatures
 * @return true if all features are available, false otherwise
 */
template <LanguageFeature... Features>
constexpr bool hasAllFeatures() noexcept {
    return (hasFeature<Features>() && ...);
}

/**
 * @brief Check if ANY of the specified features are available
 * @tparam Features Variadic list of LanguageFeatures
 * @return true if any feature is available, false otherwise
 */
template <LanguageFeature... Features>
constexpr bool hasAnyFeature() noexcept {
    return (hasFeature<Features>() || ...);
}

/**
 * @brief Count how many of the specified features are available
 * @tparam Features Variadic list of LanguageFeatures
 * @return Number of available features
 */
template <LanguageFeature... Features>
constexpr std::size_t countAvailableFeatures() noexcept {
    return (static_cast<std::size_t>(hasFeature<Features>()) + ...);
}

// =============================================================================
// Type Traits for Feature Detection
// =============================================================================

/**
 * @brief Type trait to check if a language feature is available
 * @tparam TFeature LanguageFeature to check
 */
template <LanguageFeature TFeature>
struct FeatureAvailable : bool_constant<hasFeature<TFeature>()> {};

/**
 * @brief Variable template for FeatureAvailable (C++14+)
 * @tparam TFeature LanguageFeature to check
 */
template <LanguageFeature TFeature>
constexpr bool feature_available_v = FeatureAvailable<TFeature>::value;

/**
 * @brief Type trait to check if ALL features are available
 * @tparam Features Variadic LanguageFeatures to check
 */
template <LanguageFeature... Features>
struct AllFeaturesAvailable : bool_constant<hasAllFeatures<Features...>()> {};

/**
 * @brief Variable template for AllFeaturesAvailable
 * @tparam Features Variadic LanguageFeatures to check
 */
template <LanguageFeature... Features>
constexpr bool all_features_available_v = AllFeaturesAvailable<Features...>::value;

/**
 * @brief Type trait to check if ANY feature is available
 * @tparam Features Variadic LanguageFeatures to check
 */
template <LanguageFeature... Features>
struct AnyFeatureAvailable : bool_constant<hasAnyFeature<Features...>()> {};

/**
 * @brief Variable template for AnyFeatureAvailable
 * @tparam Features Variadic LanguageFeatures to check
 */
template <LanguageFeature... Features>
constexpr bool any_feature_available_v = AnyFeatureAvailable<Features...>::value;

// =============================================================================
// Runtime Feature Type Traits
// =============================================================================

/**
 * @brief Type trait for runtime feature availability (non-constexpr)
 * @tparam TFeature RuntimeFeature to check
 */
template <RuntimeFeature TFeature>
struct RuntimeFeatureAvailable {
    static bool value() noexcept { return hasRuntimeFeature<TFeature>(); }
};

// =============================================================================
// Compile-Time Feature Constants
// =============================================================================

/**
 * @brief Get compile-time constant for a language feature
 * @tparam TFeature LanguageFeature to get constant for
 * @return Compile-time boolean constant
 */
template <LanguageFeature TFeature>
constexpr bool getFeatureConstant() noexcept {
    return hasFeature<TFeature>();
}

/**
 * @brief Template variable for feature constants (C++14+)
 * @tparam TFeature LanguageFeature to check
 */
template <LanguageFeature TFeature>
constexpr bool feature_constant_v = getFeatureConstant<TFeature>();

}  // namespace traits
}  // namespace platform
}  // namespace trlc

// =============================================================================
// Convenience Macros for Advanced Feature Testing
// =============================================================================

/**
 * @brief Macro for testing specific language feature availability
 * @param feature LanguageFeature enum value (without namespace)
 */
#define TRLC_HAS_LANGUAGE_FEATURE(feature) \
    (trlc::platform::hasFeature<trlc::platform::LanguageFeature::feature>())

/**
 * @brief Macro for testing specific runtime feature availability
 * @param feature RuntimeFeature enum value (without namespace)
 */
#define TRLC_HAS_RUNTIME_FEATURE(feature) \
    (trlc::platform::hasRuntimeFeature<trlc::platform::RuntimeFeature::feature>())

/**
 * @brief Macro for conditional compilation based on language feature
 * @param feature LanguageFeature enum value (without namespace)
 */
#define TRLC_IF_LANGUAGE_FEATURE(feature) \
    if constexpr (trlc::platform::hasFeature<trlc::platform::LanguageFeature::feature>())

/**
 * @brief Macro for conditional compilation when language feature is NOT available
 * @param feature LanguageFeature enum value (without namespace)
 */
#define TRLC_IF_NO_LANGUAGE_FEATURE(feature) \
    if constexpr (!trlc::platform::hasFeature<trlc::platform::LanguageFeature::feature>())

/**
 * @brief Macro for template specialization based on feature availability
 * @param feature LanguageFeature enum value (without namespace)
 */
#define TRLC_REQUIRE_FEATURE(feature)                                 \
    template <typename = trlc::platform::traits::enable_if_feature_t< \
                  trlc::platform::LanguageFeature::feature>>

/**
 * @brief Macro for template specialization when feature is NOT available
 * @param feature LanguageFeature enum value (without namespace)
 */
#define TRLC_REQUIRE_NO_FEATURE(feature)                                 \
    template <typename = trlc::platform::traits::enable_if_no_feature_t< \
                  trlc::platform::LanguageFeature::feature>>

/**
 * @brief Macro for testing multiple feature requirements
 * @param ... Variadic list of LanguageFeature enum values (without namespace)
 */
#define TRLC_HAS_ALL_FEATURES(...) (trlc::platform::traits::hasAllFeatures<__VA_ARGS__>())

/**
 * @brief Macro for testing if any feature is available
 * @param ... Variadic list of LanguageFeature enum values (without namespace)
 */
#define TRLC_HAS_ANY_FEATURES(...) (trlc::platform::traits::hasAnyFeature<__VA_ARGS__>())

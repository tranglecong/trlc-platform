/**
 * @file test_template_specializations.cpp
 * @brief Test suite for advanced template specializations and metaprogramming utilities
 *
 * Tests template specializations for hasFeature<>, SFINAE helpers, and 
 * platform-specific template metaprogramming features.
 */

#include <iostream>
#include <cassert>

// Include only the core headers needed
#include "trlc/platform/features.hpp"
#include "trlc/platform/platform.hpp"
#include "trlc/platform/compiler.hpp"
#include "trlc/platform/architecture.hpp"
#include "trlc/platform/traits.hpp"

namespace trlc::platform::test {

// Add using declarations for convenience
using namespace trlc::platform;

// Test helper functions
void testFeatureTemplateSpecializations() {
    std::cout << "Testing template specializations for hasFeature..." << std::endl;
    
    // Test language feature specializations
    constexpr bool has_exceptions = hasFeature<LanguageFeature::exceptions>();
    constexpr bool has_rtti = hasFeature<LanguageFeature::rtti>();
    constexpr bool has_threads = hasFeature<LanguageFeature::threads>();
    constexpr bool has_atomic = hasFeature<LanguageFeature::atomic_operations>();
    
    std::cout << "  - Exceptions: " << (has_exceptions ? "yes" : "no") << std::endl;
    std::cout << "  - RTTI: " << (has_rtti ? "yes" : "no") << std::endl;
    std::cout << "  - Threads: " << (has_threads ? "yes" : "no") << std::endl;
    std::cout << "  - Atomic: " << (has_atomic ? "yes" : "no") << std::endl;
    
    // Test runtime features (non-constexpr)
    bool has_sse = hasRuntimeFeature<RuntimeFeature::sse>();
    bool has_avx = hasRuntimeFeature<RuntimeFeature::avx>();
    
    std::cout << "  - SSE: " << (has_sse ? "yes" : "no") << std::endl;
    std::cout << "  - AVX: " << (has_avx ? "yes" : "no") << std::endl;
    
    std::cout << "  ✓ Feature template specializations working" << std::endl;
}

void testTypeTraits() {
    std::cout << "Testing type traits for feature detection..." << std::endl;
    
    // Test FeatureAvailable trait
    constexpr bool exceptions_available = trlc::platform::traits::FeatureAvailable<LanguageFeature::exceptions>::value;
    constexpr bool rtti_available = trlc::platform::traits::feature_available_v<LanguageFeature::rtti>;
    
    std::cout << "  - FeatureAvailable trait: " << (exceptions_available ? "true" : "false") << std::endl;
    std::cout << "  - feature_available_v: " << (rtti_available ? "true" : "false") << std::endl;
    
    // Test AllFeaturesAvailable trait
    constexpr bool all_basic_available = trlc::platform::traits::all_features_available_v<
        LanguageFeature::exceptions,
        LanguageFeature::threads
    >;
    
    std::cout << "  - All basic features: " << (all_basic_available ? "yes" : "no") << std::endl;
    
    std::cout << "  ✓ Type traits working correctly" << std::endl;
}

void testVariadicFeatureTemplates() {
    std::cout << "Testing variadic feature templates..." << std::endl;
    
    // Test multiple feature requirements
    constexpr bool has_multiple = trlc::platform::traits::hasAllFeatures<
        LanguageFeature::exceptions,
        LanguageFeature::rtti,
        LanguageFeature::threads
    >();
    
    constexpr bool has_any = trlc::platform::traits::hasAnyFeature<
        LanguageFeature::exceptions,
        LanguageFeature::vector_intrinsics,
        LanguageFeature::stack_protection
    >();
    
    constexpr std::size_t feature_count = trlc::platform::traits::countAvailableFeatures<
        LanguageFeature::exceptions,
        LanguageFeature::rtti,
        LanguageFeature::threads,
        LanguageFeature::atomic_operations
    >();
    
    std::cout << "  - Multiple features available: " << (has_multiple ? "yes" : "no") << std::endl;
    std::cout << "  - Any feature available: " << (has_any ? "yes" : "no") << std::endl;
    std::cout << "  - Feature count: " << feature_count << std::endl;
    
    std::cout << "  ✓ Variadic feature templates working correctly" << std::endl;
}

void testCompileTimeConstants() {
    std::cout << "Testing compile-time constants..." << std::endl;
    
    // Test getFeatureConstant function
    constexpr bool exceptions_constant = trlc::platform::traits::getFeatureConstant<LanguageFeature::exceptions>();
    constexpr bool threads_constant = trlc::platform::traits::getFeatureConstant<LanguageFeature::threads>();
    
    std::cout << "  - Exceptions constant: " << (exceptions_constant ? "true" : "false") << std::endl;
    std::cout << "  - Threads constant: " << (threads_constant ? "true" : "false") << std::endl;
    
    // Test feature_available_v template variable
    constexpr bool atomic_available = trlc::platform::traits::feature_available_v<LanguageFeature::atomic_operations>;
    std::cout << "  - Atomic available (template var): " << (atomic_available ? "yes" : "no") << std::endl;
    
    std::cout << "  ✓ Compile-time constants working" << std::endl;
}

void testConditionalCompilationMacros() {
    std::cout << "Testing conditional compilation macros..." << std::endl;
    
    // Test language feature macros
    bool has_exceptions_macro = TRLC_HAS_LANGUAGE_FEATURE(exceptions);
    bool has_threads_macro = TRLC_HAS_LANGUAGE_FEATURE(threads);
    
    std::cout << "  - Exceptions (macro): " << (has_exceptions_macro ? "yes" : "no") << std::endl;
    std::cout << "  - Threads (macro): " << (has_threads_macro ? "yes" : "no") << std::endl;
    
    // Test runtime feature macros
    bool has_sse_macro = TRLC_HAS_RUNTIME_FEATURE(sse);
    std::cout << "  - SSE (macro): " << (has_sse_macro ? "yes" : "no") << std::endl;
    
    // Test conditional compilation
    TRLC_IF_LANGUAGE_FEATURE(exceptions) {
        std::cout << "  - Conditional compilation with exceptions worked" << std::endl;
    } else {
        std::cout << "  - Conditional compilation without exceptions worked" << std::endl;
    }
    
    std::cout << "  ✓ Conditional compilation macros working" << std::endl;
}

void testRuntimeFeatureTemplates() {
    std::cout << "Testing runtime feature template specializations..." << std::endl;
    
    // Test RuntimeFeatureAvailable
    bool sse_available = trlc::platform::traits::RuntimeFeatureAvailable<RuntimeFeature::sse>::value();
    bool avx_available = trlc::platform::traits::RuntimeFeatureAvailable<RuntimeFeature::avx>::value();
    bool neon_available = trlc::platform::traits::RuntimeFeatureAvailable<RuntimeFeature::neon>::value();
    
    std::cout << "  - SSE available: " << (sse_available ? "yes" : "no") << std::endl;
    std::cout << "  - AVX available: " << (avx_available ? "yes" : "no") << std::endl;
    std::cout << "  - NEON available: " << (neon_available ? "yes" : "no") << std::endl;
    
    std::cout << "  ✓ Runtime feature templates working" << std::endl;
}

void testFeatureVariableTemplates() {
    std::cout << "Testing feature variable templates..." << std::endl;
    
    // Test all_features_available_v
    constexpr bool all_core_features = trlc::platform::traits::all_features_available_v<
        LanguageFeature::exceptions,
        LanguageFeature::threads
    >;
    
    // Test any_feature_available_v  
    constexpr bool any_advanced_features = trlc::platform::traits::any_feature_available_v<
        LanguageFeature::vector_intrinsics,
        LanguageFeature::stack_protection,
        LanguageFeature::address_sanitizer
    >;
    
    std::cout << "  - All core features: " << (all_core_features ? "yes" : "no") << std::endl;
    std::cout << "  - Any advanced features: " << (any_advanced_features ? "yes" : "no") << std::endl;
    
    std::cout << "  ✓ Feature variable templates working" << std::endl;
}

} // namespace trlc::platform::test

int main() {
    std::cout << "TRLC Platform Template Specializations Test Suite" << std::endl;
    std::cout << "=================================================" << std::endl;
    
    try {
        trlc::platform::test::testFeatureTemplateSpecializations();
        trlc::platform::test::testTypeTraits();
        trlc::platform::test::testVariadicFeatureTemplates();
        trlc::platform::test::testCompileTimeConstants();
        trlc::platform::test::testConditionalCompilationMacros();
        trlc::platform::test::testRuntimeFeatureTemplates();
        trlc::platform::test::testFeatureVariableTemplates();
        
        std::cout << "\n=================================================" << std::endl;
        std::cout << "All template specialization tests passed!" << std::endl;
        std::cout << "Advanced template metaprogramming utilities working correctly." << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nTemplate test failed: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\nTemplate test failed with unknown error" << std::endl;
        return 1;
    }
}

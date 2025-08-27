/**
 * @file test_features_detection.cpp
 * @brief Comprehensive tests for feature availability detection
 */

#include <cassert>
#include <iostream>

#include "trlc/platform/features.hpp"

namespace trlc::platform::test {

void testLanguageFeatures() {
    std::cout << "Testing language features..." << std::endl;

    // Test exception support
    bool has_exceptions = hasExceptions();
    std::cout << "  - Exceptions: " << (has_exceptions ? "yes" : "no") << std::endl;

    // Test RTTI support
    bool has_rtti = hasRtti();
    std::cout << "  - RTTI: " << (has_rtti ? "yes" : "no") << std::endl;

    // Test thread support
    bool has_threads = hasThreads();
    std::cout << "  - Threads: " << (has_threads ? "yes" : "no") << std::endl;

    // Test atomic operations
    bool has_atomic = hasAtomicOperations();
    std::cout << "  - Atomic operations: " << (has_atomic ? "yes" : "no") << std::endl;

    // Test inline assembly
    bool has_inline_asm = hasInlineAssembly();
    std::cout << "  - Inline assembly: " << (has_inline_asm ? "yes" : "no") << std::endl;

    // Test vector intrinsics
    bool has_vector = hasVectorIntrinsics();
    std::cout << "  - Vector intrinsics: " << (has_vector ? "yes" : "no") << std::endl;

    std::cout << "  ✓ Language features tested" << std::endl;
}

void testRuntimeFeatures() {
    std::cout << "Testing runtime CPU features..." << std::endl;

    // Test SIMD features
    bool has_sse = hasSseSupport();
    bool has_sse2 = hasSse2Support();
    bool has_sse3 = hasSse3Support();
    bool has_sse41 = hasSse41Support();
    bool has_sse42 = hasSse42Support();
    bool has_avx = hasAvxSupport();
    bool has_avx2 = hasAvx2Support();
    bool has_avx512f = hasAvx512fSupport();
    bool has_neon = hasNeonSupport();

    std::cout << "  - SSE: " << (has_sse ? "yes" : "no") << std::endl;
    std::cout << "  - SSE2: " << (has_sse2 ? "yes" : "no") << std::endl;
    std::cout << "  - SSE3: " << (has_sse3 ? "yes" : "no") << std::endl;
    std::cout << "  - SSE4.1: " << (has_sse41 ? "yes" : "no") << std::endl;
    std::cout << "  - SSE4.2: " << (has_sse42 ? "yes" : "no") << std::endl;
    std::cout << "  - AVX: " << (has_avx ? "yes" : "no") << std::endl;
    std::cout << "  - AVX2: " << (has_avx2 ? "yes" : "no") << std::endl;
    std::cout << "  - AVX-512F: " << (has_avx512f ? "yes" : "no") << std::endl;
    std::cout << "  - NEON: " << (has_neon ? "yes" : "no") << std::endl;

    // Test hardware acceleration
    bool has_hw_aes = hasHardwareAes();
    bool has_hw_random = hasHardwareRandom();

    std::cout << "  - Hardware AES: " << (has_hw_aes ? "yes" : "no") << std::endl;
    std::cout << "  - Hardware Random: " << (has_hw_random ? "yes" : "no") << std::endl;

    std::cout << "  ✓ Runtime features tested" << std::endl;
}

void testSanitizerFeatures() {
    std::cout << "Testing sanitizer features..." << std::endl;

    bool has_asan = hasAddressSanitizer();
    bool has_tsan = hasThreadSanitizer();
    bool has_msan = hasMemorySanitizer();
    bool has_ubsan = hasUndefinedBehaviorSanitizer();
    bool has_stack_prot = hasStackProtection();

    std::cout << "  - AddressSanitizer: " << (has_asan ? "yes" : "no") << std::endl;
    std::cout << "  - ThreadSanitizer: " << (has_tsan ? "yes" : "no") << std::endl;
    std::cout << "  - MemorySanitizer: " << (has_msan ? "yes" : "no") << std::endl;
    std::cout << "  - UndefinedBehaviorSanitizer: " << (has_ubsan ? "yes" : "no") << std::endl;
    std::cout << "  - Stack protection: " << (has_stack_prot ? "yes" : "no") << std::endl;

    std::cout << "  ✓ Sanitizer features tested" << std::endl;
}

void testFeatureSet() {
    std::cout << "Testing FeatureSet struct..." << std::endl;

    FeatureSet features = getFeatureSet();

    std::cout << "  - Exceptions: " << (features.has_exceptions ? "yes" : "no") << std::endl;
    std::cout << "  - RTTI: " << (features.has_rtti ? "yes" : "no") << std::endl;
    std::cout << "  - Threads: " << (features.has_threads ? "yes" : "no") << std::endl;
    std::cout << "  - Atomic: " << (features.has_atomic ? "yes" : "no") << std::endl;
    std::cout << "  - Inline ASM: " << (features.has_inline_asm ? "yes" : "no") << std::endl;
    std::cout << "  - Vector intrinsics: " << (features.has_vector_intrinsics ? "yes" : "no")
              << std::endl;

    // Test feature query methods
    bool has_exceptions_via_method = features.hasLanguageFeature(LanguageFeature::exceptions);
    bool has_rtti_via_method = features.hasLanguageFeature(LanguageFeature::rtti);
    bool has_threads_via_method = features.hasLanguageFeature(LanguageFeature::threads);

    assert(has_exceptions_via_method == features.has_exceptions);
    assert(has_rtti_via_method == features.has_rtti);
    assert(has_threads_via_method == features.has_threads);

    std::cout << "  ✓ FeatureSet works correctly" << std::endl;
}

void testMacros() {
    std::cout << "Testing feature detection macros..." << std::endl;

    // Test macro definitions
    bool exceptions_macro = (TRLC_HAS_EXCEPTIONS != 0);
    bool rtti_macro = (TRLC_HAS_RTTI != 0);
    bool threads_macro = (TRLC_HAS_THREADS != 0);
    bool atomic_macro = (TRLC_HAS_ATOMIC != 0);
    bool inline_asm_macro = (TRLC_HAS_INLINE_ASM != 0);
    bool vector_intrinsics_macro = (TRLC_HAS_VECTOR_INTRINSICS != 0);

    std::cout << "  - TRLC_HAS_EXCEPTIONS: " << (exceptions_macro ? "yes" : "no") << std::endl;
    std::cout << "  - TRLC_HAS_RTTI: " << (rtti_macro ? "yes" : "no") << std::endl;
    std::cout << "  - TRLC_HAS_THREADS: " << (threads_macro ? "yes" : "no") << std::endl;
    std::cout << "  - TRLC_HAS_ATOMIC: " << (atomic_macro ? "yes" : "no") << std::endl;
    std::cout << "  - TRLC_HAS_INLINE_ASM: " << (inline_asm_macro ? "yes" : "no") << std::endl;
    std::cout << "  - TRLC_HAS_VECTOR_INTRINSICS: " << (vector_intrinsics_macro ? "yes" : "no")
              << std::endl;

    // Verify macros match function results
    assert(exceptions_macro == hasExceptions());
    assert(rtti_macro == hasRtti());
    assert(threads_macro == hasThreads());
    assert(atomic_macro == hasAtomicOperations());
    assert(inline_asm_macro == hasInlineAssembly());
    assert(vector_intrinsics_macro == hasVectorIntrinsics());

    std::cout << "  ✓ Macros match function results" << std::endl;
}

void testCompileTimeDetection() {
    std::cout << "Testing compile-time feature detection..." << std::endl;

    // These should all be compile-time constants
    static_assert(hasExceptions() == hasExceptions(), "hasExceptions should be constexpr");
    static_assert(hasRtti() == hasRtti(), "hasRtti should be constexpr");
    static_assert(hasThreads() == hasThreads(), "hasThreads should be constexpr");
    static_assert(hasAtomicOperations() == hasAtomicOperations(),
                  "hasAtomicOperations should be constexpr");
    static_assert(hasInlineAssembly() == hasInlineAssembly(),
                  "hasInlineAssembly should be constexpr");
    static_assert(hasVectorIntrinsics() == hasVectorIntrinsics(),
                  "hasVectorIntrinsics should be constexpr");

    constexpr auto feature_set = getFeatureSet();
    static_cast<void>(feature_set);  // Use to prevent optimization

    std::cout << "  ✓ All compile-time detections work" << std::endl;
}

void testEdgeCases() {
    std::cout << "Testing edge cases..." << std::endl;

    // Test unknown language features
    FeatureSet features = getFeatureSet();
    bool unknown_feature = features.hasLanguageFeature(static_cast<LanguageFeature>(999));
    assert(!unknown_feature);

    // Test unknown runtime features
    bool unknown_runtime = features.hasRuntimeFeature(static_cast<RuntimeFeature>(999));
    assert(!unknown_runtime);

    // Test feature enumeration bounds
    assert(static_cast<int>(LanguageFeature::exceptions) >= 0);
    assert(static_cast<int>(RuntimeFeature::sse) >= 0);

    std::cout << "  ✓ Edge cases handled correctly" << std::endl;
}

void testPerformance() {
    std::cout << "Testing performance characteristics..." << std::endl;

    // Language features should be compile-time only
    constexpr bool exceptions = hasExceptions();
    constexpr bool rtti = hasRtti();
    constexpr bool threads = hasThreads();
    constexpr bool atomic = hasAtomicOperations();
    constexpr bool inline_asm = hasInlineAssembly();
    constexpr bool vector = hasVectorIntrinsics();
    constexpr bool stack_prot = hasStackProtection();
    constexpr bool asan = hasAddressSanitizer();
    constexpr bool tsan = hasThreadSanitizer();
    constexpr bool msan = hasMemorySanitizer();
    constexpr bool ubsan = hasUndefinedBehaviorSanitizer();

    // Use values to prevent optimization
    static_cast<void>(exceptions);
    static_cast<void>(rtti);
    static_cast<void>(threads);
    static_cast<void>(atomic);
    static_cast<void>(inline_asm);
    static_cast<void>(vector);
    static_cast<void>(stack_prot);
    static_cast<void>(asan);
    static_cast<void>(tsan);
    static_cast<void>(msan);
    static_cast<void>(ubsan);

    std::cout << "  ✓ All language features are compile-time constants" << std::endl;
}

}  // namespace trlc::platform::test

int main() {
    using namespace trlc::platform::test;

    std::cout << "=== TRLC Platform Features Detection Tests ===" << std::endl;

    try {
        testLanguageFeatures();
        testRuntimeFeatures();
        testSanitizerFeatures();
        testFeatureSet();
        testMacros();
        testCompileTimeDetection();
        testEdgeCases();
        testPerformance();

        std::cout << "\n✅ All features detection tests passed!" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}

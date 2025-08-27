/**
 * @file portable_library_demo.cpp
 * @brief Comprehensive demonstration of TRLC Platform Library usage
 */

#include "trlc/platform/core.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace trlc::platform;

int main() {
    std::cout << "🚀 TRLC Platform Demo - Using API instead of traditional macros" << std::endl;
    std::cout << "=================================================================" << std::endl;
    
    // Initialize platform
    initializePlatform();
    
    // Demo 1: Platform detection using TRLC APIs
    std::cout << "\n=== Platform Detection Demo ===" << std::endl;
    std::cout << "Using TRLC Platform API instead of traditional macros:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    // Traditional way: #ifdef _WIN32, #ifdef __linux__, etc.
    // TRLC way: Use API functions
    
    constexpr auto os = getOperatingSystem();
    constexpr auto compiler = getCompilerType();
    constexpr auto arch = getCpuArchitecture();
    
    std::cout << "Operating System: ";
    if constexpr (os == OperatingSystem::windows) {
        std::cout << "Windows" << std::endl;
    } else if constexpr (os == OperatingSystem::linux_generic) {
        std::cout << "Linux" << std::endl;
    } else if constexpr (os == OperatingSystem::macos) {
        std::cout << "macOS" << std::endl;
    } else {
        std::cout << "Other Unix-like" << std::endl;
    }
    
    std::cout << "Compiler: ";
    if constexpr (compiler == CompilerType::gcc) {
        std::cout << "GCC" << std::endl;
    } else if constexpr (compiler == CompilerType::clang) {
        std::cout << "Clang" << std::endl;
    } else if constexpr (compiler == CompilerType::msvc) {
        std::cout << "MSVC" << std::endl;
    } else {
        std::cout << "Other" << std::endl;
    }
    
    std::cout << "Architecture: ";
    if constexpr (arch == CpuArchitecture::x86_64) {
        std::cout << "x86-64" << std::endl;
    } else if constexpr (arch == CpuArchitecture::arm_v8_64) {
        std::cout << "ARM64" << std::endl;
    } else {
        std::cout << "Other" << std::endl;
    }
    
    std::cout << "Pointer size: " << getPointerSize() << " bits" << std::endl;
    std::cout << "Endianness: " << (isLittleEndian() ? "Little" : "Big") << " Endian" << std::endl;
    
    // Demo 2: Feature detection using TRLC APIs  
    std::cout << "\n=== Feature Detection Demo ===" << std::endl;
    std::cout << "Using TRLC Platform feature detection instead of compiler macros:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    // Traditional way: #ifdef __cpp_exceptions, #ifdef _OPENMP, etc.
    // TRLC way: Use template-based feature detection
    
    std::cout << "Language Features:" << std::endl;
    std::cout << "  Exceptions: " << (hasFeature<LanguageFeature::exceptions>() ? "✓" : "✗") << std::endl;
    std::cout << "  RTTI: " << (hasFeature<LanguageFeature::rtti>() ? "✓" : "✗") << std::endl;
    std::cout << "  Threads: " << (hasFeature<LanguageFeature::threads>() ? "✓" : "✗") << std::endl;
    std::cout << "  Atomic: " << (hasFeature<LanguageFeature::atomic_operations>() ? "✓" : "✗") << std::endl;
    
    std::cout << "\nSIMD Features:" << std::endl;
    std::cout << "  SSE: " << (hasRuntimeFeature(RuntimeFeature::sse) ? "✓" : "✗") << std::endl;
    std::cout << "  AVX: " << (hasRuntimeFeature(RuntimeFeature::avx) ? "✓" : "✗") << std::endl;
    std::cout << "  NEON: " << (hasRuntimeFeature(RuntimeFeature::neon) ? "✓" : "✗") << std::endl;
    
    std::cout << "\nCompiler Features:" << std::endl;
    std::cout << "  Builtin Functions: " << ((getCompilerType() == CompilerType::gcc || getCompilerType() == CompilerType::clang) ? "✓" : "✗") << std::endl;
    std::cout << "  Inline Assembly: " << ((getCompilerType() == CompilerType::gcc || getCompilerType() == CompilerType::clang) ? "✓" : "✗") << std::endl;
    std::cout << "  Attributes: " << ((getCompilerType() == CompilerType::gcc || getCompilerType() == CompilerType::clang) ? "✓" : "✗") << std::endl;
    
    // Demo 3: Compiler-specific optimizations using TRLC APIs
    std::cout << "\n=== Compiler Optimizations Demo ===" << std::endl;
    std::cout << "Using TRLC Platform API for compiler-specific optimizations:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    // Traditional way: #if defined(__GNUC__) || defined(__clang__)
    // TRLC way: Use compiler detection
    unsigned int test_value = 0b11010110101011010110101101011010;
    std::cout << "Test value: 0x" << std::hex << test_value << std::dec << std::endl;
    
    // Population count using compiler builtin
    unsigned int popcount = 0;
    if constexpr (getCompilerType() == CompilerType::gcc || getCompilerType() == CompilerType::clang) {
        popcount = __builtin_popcount(test_value);
        std::cout << "Population count (using builtin): " << popcount << std::endl;
    } else {
        // Fallback implementation
        unsigned int x = test_value;
        while (x) {
            popcount += x & 1;
            x >>= 1;
        }
        std::cout << "Population count (fallback): " << popcount << std::endl;
    }
    
    // Demo 4: Debug utilities
    std::cout << "\n=== Debug Utilities Demo ===" << std::endl;
    std::cout << "Using TRLC Platform debug API instead of traditional macros:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
#ifdef TRLC_PLATFORM_ENABLE_DEBUG_UTILS
    std::cout << "Debug build: " << (isDebugBuild() ? "Yes" : "No") << std::endl;
    std::cout << "Release build: " << (isReleaseBuild() ? "Yes" : "No") << std::endl;
    std::cout << "Debug info available: " << (hasDebugInfo() ? "Yes" : "No") << std::endl;
#else
    std::cout << "Debug utilities not enabled in this build" << std::endl;
    std::cout << "Enable with -DTRLC_PLATFORM_ENABLE_DEBUG_UTILS=1 or Debug CMake build" << std::endl;
#endif
    
    // Generate comprehensive platform report
    std::cout << "\n=== Complete Platform Report ===" << std::endl;
    std::cout << "Generated using TRLC Platform comprehensive reporting:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    auto report = getPlatformReport();
    std::cout << report.getBriefSummary() << std::endl;
    
    std::cout << "\n✅ Demo completed!" << std::endl;
    std::cout << "\nTraditional approach vs TRLC Platform approach:" << std::endl;
    std::cout << "❌ Traditional: #if defined(__GNUC__) || defined(__clang__)" << std::endl;
    std::cout << "✅ TRLC: if constexpr (getCompilerType() == CompilerType::gcc)" << std::endl;
    std::cout << "\n❌ Traditional: #ifdef _WIN32 / #ifdef __linux__" << std::endl;
    std::cout << "✅ TRLC: if constexpr (getOperatingSystem() == OperatingSystem::windows)" << std::endl;
    std::cout << "\n❌ Traditional: #ifdef __cpp_exceptions" << std::endl;
    std::cout << "✅ TRLC: if constexpr (hasFeature<LanguageFeature::exceptions>())" << std::endl;
    
    std::cout << "\nAdvantages of TRLC Platform approach:" << std::endl;
    std::cout << "  ✓ Type-safe compile-time decisions" << std::endl;
    std::cout << "  ✓ IDE-friendly with auto-completion" << std::endl;
    std::cout << "  ✓ Template-based feature detection" << std::endl;
    std::cout << "  ✓ Consistent API across all platforms" << std::endl;
    std::cout << "  ✓ Zero runtime overhead (constexpr evaluation)" << std::endl;
    std::cout << "  ✓ Compile-time verification of platform assumptions" << std::endl;
    
    return 0;
}

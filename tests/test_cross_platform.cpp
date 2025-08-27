#include "trlc/platform/core.hpp"
#include <iostream>

// Test that works on all platforms
int main() {
    // Initialize platform for runtime features
    trlc::platform::initializePlatform();
    
    // Get comprehensive platform report
    auto report = trlc::platform::getPlatformReport();
    
    std::cout << "Cross-Platform Compatibility Test" << std::endl;
    std::cout << "=================================" << std::endl;
    std::cout << report.getBriefSummary() << std::endl;
    std::cout << std::endl;
    
    // Test compile-time platform detection
    std::cout << "Platform Detection:" << std::endl;
    std::cout << "- Operating System: " << static_cast<int>(trlc::platform::getOperatingSystem()) << std::endl;
    std::cout << "- Compiler: " << static_cast<int>(trlc::platform::getCompilerType()) << std::endl;
    std::cout << "- Architecture: " << static_cast<int>(trlc::platform::getCpuArchitecture()) << std::endl;
    std::cout << "- C++ Standard: " << static_cast<int>(trlc::platform::getCppStandard()) << std::endl;
    std::cout << std::endl;
    
    // Test feature detection
    std::cout << "Feature Detection:" << std::endl;
    std::cout << "- Exceptions: " << (trlc::platform::hasFeature<trlc::platform::LanguageFeature::exceptions>() ? "yes" : "no") << std::endl;
    std::cout << "- Threads: " << (trlc::platform::hasFeature<trlc::platform::LanguageFeature::threads>() ? "yes" : "no") << std::endl;
    std::cout << "- RTTI: " << (trlc::platform::hasFeature<trlc::platform::LanguageFeature::rtti>() ? "yes" : "no") << std::endl;
    std::cout << "- Atomic: " << (trlc::platform::hasFeature<trlc::platform::LanguageFeature::atomic_operations>() ? "yes" : "no") << std::endl;
    std::cout << std::endl;
    
    // Test that macros work
    std::cout << "Macro Tests:" << std::endl;
    #ifdef TRLC_HAS_EXCEPTIONS
    std::cout << "- Exception macro: defined" << std::endl;
    #else
    std::cout << "- Exception macro: not defined" << std::endl;
    #endif
    
    #ifdef TRLC_PLATFORM_LINUX
    std::cout << "- Linux macro: defined" << std::endl;
    #elif defined(TRLC_PLATFORM_WINDOWS)
    std::cout << "- Windows macro: defined" << std::endl;
    #elif defined(TRLC_PLATFORM_MACOS)
    std::cout << "- macOS macro: defined" << std::endl;
    #else
    std::cout << "- Platform macro: other" << std::endl;
    #endif
    
    std::cout << std::endl;
    std::cout << "Cross-platform compatibility test completed successfully!" << std::endl;
    
    return 0;
}

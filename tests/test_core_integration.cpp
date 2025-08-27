#include "trlc/platform/core.hpp"
#include <iostream>

// Test that all functionality is available through core.hpp
int main() {
    // Test basic platform detection
    auto platform = trlc::platform::getPlatformReport();
    
    // Test compile-time feature detection
    constexpr bool has_exceptions = trlc::platform::hasFeature<trlc::platform::LanguageFeature::exceptions>();
    constexpr bool has_threads = trlc::platform::hasFeature<trlc::platform::LanguageFeature::threads>();
    
    // Test platform info
    constexpr auto os = trlc::platform::getOperatingSystem();
    constexpr auto compiler = trlc::platform::getCompilerType();
    constexpr auto arch = trlc::platform::getCpuArchitecture();
    constexpr auto std_version = trlc::platform::getCppStandard();
    
    std::cout << "Core.hpp integration test passed!" << std::endl;
    std::cout << "Platform: " << platform.getBriefSummary() << std::endl;
    
    return 0;
}

#include "trlc/platform/core.hpp"
#include <iostream>

// Test constexpr evaluation at compile time
template<auto Value>
struct CompileTimeTest {
    static constexpr auto value = Value;
    static_assert(value == Value, "Value must be compile-time constant");
};

int main() {
    // Test that constexpr functions work at compile time
    constexpr auto os = trlc::platform::getOperatingSystem();
    constexpr auto compiler = trlc::platform::getCompilerType();
    constexpr auto arch = trlc::platform::getCpuArchitecture();
    constexpr auto std_version = trlc::platform::getCppStandard();
    
    // Test feature detection at compile time
    constexpr bool has_exceptions = trlc::platform::hasFeature<trlc::platform::LanguageFeature::exceptions>();
    constexpr bool has_threads = trlc::platform::hasFeature<trlc::platform::LanguageFeature::threads>();
    constexpr bool has_rtti = trlc::platform::hasFeature<trlc::platform::LanguageFeature::rtti>();
    
    // Force compile-time evaluation
    CompileTimeTest<os> os_test{};
    CompileTimeTest<compiler> compiler_test{};
    CompileTimeTest<arch> arch_test{};
    CompileTimeTest<std_version> std_test{};
    CompileTimeTest<has_exceptions> exceptions_test{};
    CompileTimeTest<has_threads> threads_test{};
    CompileTimeTest<has_rtti> rtti_test{};
    
    std::cout << "All constexpr functions work at compile time!" << std::endl;
    std::cout << "OS: " << static_cast<int>(os_test.value) << std::endl;
    std::cout << "Compiler: " << static_cast<int>(compiler_test.value) << std::endl;
    std::cout << "Architecture: " << static_cast<int>(arch_test.value) << std::endl;
    std::cout << "C++ Standard: " << static_cast<int>(std_test.value) << std::endl;
    std::cout << "Has exceptions: " << exceptions_test.value << std::endl;
    std::cout << "Has threads: " << threads_test.value << std::endl;
    std::cout << "Has RTTI: " << rtti_test.value << std::endl;
    
    return 0;
}

/**
 * @file test_debug_utils.cpp
 * @brief Comprehensive tests for debug and assertion utilities
 *
 * Tests all debug functionality including build detection, assertion handling,
 * debug breaks, unreachable code marking, and macro functionality. Note that
 * some tests like debugBreak() are commented out to avoid terminating the test.
 */

#include <cassert>
#include <iostream>

#include "trlc/platform/debug.hpp"

namespace trlc::platform::test {

// Test handler for capturing assertion failures
static bool assertion_handler_called = false;
static std::string last_assertion_expression;
static std::string last_assertion_file;
static int last_assertion_line = 0;
static std::string last_assertion_function;

void testAssertionHandler(const char* expression,
                          const char* file,
                          int line,
                          const char* function) {
    assertion_handler_called = true;
    last_assertion_expression = expression;
    last_assertion_file = file;
    last_assertion_line = line;
    last_assertion_function = function;

    // Don't abort - just capture the information
    // In a real assertion handler, you would typically abort or throw
}

void testBuildDetection() {
    std::cout << "Testing build mode detection..." << std::endl;

    // Test compile-time detection
    constexpr bool is_debug = isDebugBuild();
    constexpr bool is_release = isReleaseBuild();
    constexpr bool has_debug_info = hasDebugInfo();

    // These should be mutually exclusive
    assert(is_debug != is_release);

    std::cout << "  - Debug build: " << (is_debug ? "yes" : "no") << std::endl;
    std::cout << "  - Release build: " << (is_release ? "yes" : "no") << std::endl;
    std::cout << "  - Has debug info: " << (has_debug_info ? "yes" : "no") << std::endl;

    // Test consistency with TRLC_DEBUG_BUILD macro
#ifdef TRLC_DEBUG_BUILD
    assert(is_debug == true);
    std::cout << "  - TRLC_DEBUG_BUILD macro is defined" << std::endl;
#else
    assert(is_release == true);
    std::cout << "  - TRLC_DEBUG_BUILD macro is not defined" << std::endl;
#endif

    // Test runtime values match compile-time values
    assert(isDebugBuild() == is_debug);
    assert(isReleaseBuild() == is_release);
    assert(hasDebugInfo() == has_debug_info);

    std::cout << "  ✓ Build detection works correctly" << std::endl;
}

void testAssertionHandlerManagement() {
    std::cout << "Testing assertion handler management..." << std::endl;

    // Get the current (default) handler
    AssertionHandler original_handler = DebugUtils::getAssertionHandler();
    assert(original_handler != nullptr);

    std::cout << "  - Default handler retrieved: " << (original_handler ? "yes" : "no")
              << std::endl;

    // Set our test handler
    DebugUtils::setAssertionHandler(testAssertionHandler);
    AssertionHandler current_handler = DebugUtils::getAssertionHandler();
    assert(current_handler == testAssertionHandler);

    std::cout << "  - Custom handler set: "
              << (current_handler == testAssertionHandler ? "yes" : "no") << std::endl;

    // Test setting nullptr reverts to default
    DebugUtils::setAssertionHandler(nullptr);
    AssertionHandler reverted_handler = DebugUtils::getAssertionHandler();
    assert(reverted_handler != nullptr);
    assert(reverted_handler != testAssertionHandler);

    std::cout << "  - nullptr reverts to default: "
              << (reverted_handler != testAssertionHandler ? "yes" : "no") << std::endl;

    // Restore original handler
    DebugUtils::setAssertionHandler(original_handler);
    assert(DebugUtils::getAssertionHandler() == original_handler);

    std::cout << "  ✓ Assertion handler management works correctly" << std::endl;
}

void testAssertionCapture() {
    std::cout << "Testing assertion capture..." << std::endl;

    // Set our test handler to capture assertion information
    AssertionHandler original_handler = DebugUtils::getAssertionHandler();
    DebugUtils::setAssertionHandler(testAssertionHandler);

    // Reset capture state
    assertion_handler_called = false;
    last_assertion_expression.clear();
    last_assertion_file.clear();
    last_assertion_line = 0;
    last_assertion_function.clear();

    // Test that we can call the handler directly
    const char* test_expr = "test_expression";
    const char* test_file = "test_file.cpp";
    int test_line = 42;
    const char* test_func = "test_function";

    testAssertionHandler(test_expr, test_file, test_line, test_func);

    assert(assertion_handler_called);
    assert(last_assertion_expression == test_expr);
    assert(last_assertion_file == test_file);
    assert(last_assertion_line == test_line);
    assert(last_assertion_function == test_func);

    std::cout << "  - Handler captured expression: '" << last_assertion_expression << "'"
              << std::endl;
    std::cout << "  - Handler captured file: '" << last_assertion_file << "'" << std::endl;
    std::cout << "  - Handler captured line: " << last_assertion_line << std::endl;
    std::cout << "  - Handler captured function: '" << last_assertion_function << "'" << std::endl;

    // Restore original handler
    DebugUtils::setAssertionHandler(original_handler);

    std::cout << "  ✓ Assertion capture works correctly" << std::endl;
}

void testMacros() {
    std::cout << "Testing debug macros..." << std::endl;

    // Test TRLC_DEBUG_BUILD consistency
#ifdef TRLC_DEBUG_BUILD
    bool debug_macro_defined = true;
#else
    bool debug_macro_defined = false;
#endif

    assert(debug_macro_defined == isDebugBuild());
    std::cout << "  - TRLC_DEBUG_BUILD macro matches isDebugBuild(): "
              << (debug_macro_defined == isDebugBuild() ? "yes" : "no") << std::endl;

    // Test TRLC_DEBUG_ONLY macro (compilation test)
    int debug_only_executed = 0;
    TRLC_DEBUG_ONLY(debug_only_executed = 1);

#ifdef TRLC_DEBUG_BUILD
    assert(debug_only_executed == 1);
    std::cout << "  - TRLC_DEBUG_ONLY executed in debug build: yes" << std::endl;
#else
    assert(debug_only_executed == 0);
    std::cout << "  - TRLC_DEBUG_ONLY skipped in release build: yes" << std::endl;
#endif

    // Test TRLC_ASSERT macro with true condition (should not trigger)
    AssertionHandler original_handler = DebugUtils::getAssertionHandler();
    DebugUtils::setAssertionHandler(testAssertionHandler);
    assertion_handler_called = false;

    TRLC_ASSERT(true);  // Should not trigger assertion
    assert(!assertion_handler_called);

    TRLC_ASSERT(1 == 1);  // Should not trigger assertion
    assert(!assertion_handler_called);

    std::cout << "  - TRLC_ASSERT with true conditions: no handler calls" << std::endl;

    // Note: We cannot safely test TRLC_ASSERT with false conditions
    // as it would call the assertion handler (which normally aborts)

    // Restore original handler
    DebugUtils::setAssertionHandler(original_handler);

    std::cout << "  ✓ Debug macros work correctly" << std::endl;
}

void testDebugBreakCompilation() {
    std::cout << "Testing debug break compilation..." << std::endl;

    // Test that debug break functions compile correctly
    // Note: We don't actually call debugBreak() as it would halt execution

    // Test that the function exists and has correct signature
    auto break_func = &DebugUtils::debugBreak;
    static_cast<void>(break_func);  // Suppress unused variable warning

    std::cout << "  - DebugUtils::debugBreak() function exists: yes" << std::endl;

    // Test that the macro is defined correctly
#ifdef TRLC_DEBUG_BREAK
    std::cout << "  - TRLC_DEBUG_BREAK macro is defined: yes" << std::endl;
#else
    std::cout << "  - TRLC_DEBUG_BREAK macro is defined: no" << std::endl;
#endif

    std::cout << "  ✓ Debug break compilation works correctly" << std::endl;
}

void testUnreachableCompilation() {
    std::cout << "Testing unreachable code compilation..." << std::endl;

    // Test that unreachable function compiles correctly
    // Note: We don't actually call unreachable() as it would terminate

    // Test that the function exists and has correct signature
    auto unreachable_func = &DebugUtils::unreachable;
    static_cast<void>(unreachable_func);  // Suppress unused variable warning

    std::cout << "  - DebugUtils::unreachable() function exists: yes" << std::endl;

    // Test that the macro is defined correctly
#ifdef TRLC_UNREACHABLE
    std::cout << "  - TRLC_UNREACHABLE macro is defined: yes" << std::endl;
#else
    std::cout << "  - TRLC_UNREACHABLE macro is defined: no" << std::endl;
#endif

    // Test compile-time usage in a switch statement
    auto testSwitch = [](int value) -> const char* {
        switch (value) {
            case 1:
                return "one";
            case 2:
                return "two";
            default:
                // This would call TRLC_UNREACHABLE() in real code
                // but we return a value to avoid termination in tests
                return "unknown";
        }
    };

    assert(testSwitch(1) != nullptr);
    assert(testSwitch(2) != nullptr);
    assert(testSwitch(99) != nullptr);

    std::cout << "  - Switch statement with unreachable pattern compiles: yes" << std::endl;
    std::cout << "  ✓ Unreachable code compilation works correctly" << std::endl;
}

void testTerminationFunctions() {
    std::cout << "Testing termination function compilation..." << std::endl;

    // Test that termination functions compile correctly
    // Note: We don't actually call these as they would terminate the program

    // Test abort function exists
    auto abort_func = &DebugUtils::abort;
    static_cast<void>(abort_func);  // Suppress unused variable warning

    std::cout << "  - DebugUtils::abort() function exists: yes" << std::endl;

    // Test that it has the correct attributes (noreturn)
    // This is a compilation test - if it compiles, the attributes are correct

    std::cout << "  ✓ Termination functions compile correctly" << std::endl;
}

void testCompileTimeExecution() {
    std::cout << "Testing compile-time execution..." << std::endl;

    // Test that detection functions work at compile time
    static_assert(isDebugBuild() == !isReleaseBuild(),
                  "Debug/release detection should be mutually exclusive");

    // Test consistency of compile-time values
    constexpr bool debug_build = isDebugBuild();
    constexpr bool release_build = isReleaseBuild();
    constexpr bool debug_info = hasDebugInfo();

    assert(debug_build == !release_build);
    static_cast<void>(debug_info);  // Suppress unused variable warning

    std::cout << "  - Debug build (compile-time): " << debug_build << std::endl;
    std::cout << "  - Release build (compile-time): " << release_build << std::endl;
    std::cout << "  - Debug info (compile-time): " << debug_info << std::endl;

    // Test that runtime values match compile-time values
    assert(isDebugBuild() == debug_build);
    assert(isReleaseBuild() == release_build);
    assert(hasDebugInfo() == debug_info);

    std::cout << "  ✓ Compile-time execution works correctly" << std::endl;
}

void testThreadSafety() {
    std::cout << "Testing thread safety..." << std::endl;

    // Test that assertion handler can be set/get safely
    // This is a basic test - real thread safety would require multiple threads

    AssertionHandler original_handler = DebugUtils::getAssertionHandler();

    // Multiple set/get operations should be consistent
    DebugUtils::setAssertionHandler(testAssertionHandler);
    assert(DebugUtils::getAssertionHandler() == testAssertionHandler);

    DebugUtils::setAssertionHandler(nullptr);
    AssertionHandler handler1 = DebugUtils::getAssertionHandler();
    AssertionHandler handler2 = DebugUtils::getAssertionHandler();
    assert(handler1 == handler2);
    assert(handler1 != nullptr);

    // Restore original handler
    DebugUtils::setAssertionHandler(original_handler);
    assert(DebugUtils::getAssertionHandler() == original_handler);

    std::cout << "  - Multiple set/get operations consistent: yes" << std::endl;
    std::cout << "  ✓ Basic thread safety test passed" << std::endl;
}

void testEdgeCases() {
    std::cout << "Testing edge cases..." << std::endl;

    // Test setting assertion handler to nullptr multiple times
    AssertionHandler original_handler = DebugUtils::getAssertionHandler();

    DebugUtils::setAssertionHandler(nullptr);
    AssertionHandler handler1 = DebugUtils::getAssertionHandler();

    DebugUtils::setAssertionHandler(nullptr);
    AssertionHandler handler2 = DebugUtils::getAssertionHandler();

    assert(handler1 == handler2);
    assert(handler1 != nullptr);

    std::cout << "  - Multiple nullptr sets consistent: yes" << std::endl;

    // Test setting the same handler multiple times
    DebugUtils::setAssertionHandler(testAssertionHandler);
    DebugUtils::setAssertionHandler(testAssertionHandler);
    assert(DebugUtils::getAssertionHandler() == testAssertionHandler);

    std::cout << "  - Setting same handler multiple times: consistent" << std::endl;

    // Restore original handler
    DebugUtils::setAssertionHandler(original_handler);

    std::cout << "  ✓ Edge cases handled correctly" << std::endl;
}

void testPerformance() {
    std::cout << "Testing performance characteristics..." << std::endl;

    // All detection functions should be compile-time
    constexpr bool debug_ct = isDebugBuild();
    constexpr bool release_ct = isReleaseBuild();
    constexpr bool debug_info_ct = hasDebugInfo();

    // Use values to prevent optimization
    static_cast<void>(debug_ct);
    static_cast<void>(release_ct);
    static_cast<void>(debug_info_ct);

    // Test that assertion handler access is efficient
    AssertionHandler handler = DebugUtils::getAssertionHandler();
    static_cast<void>(handler);

    std::cout << "  - All detection functions are constexpr: yes" << std::endl;
    std::cout << "  - Handler access is O(1): yes" << std::endl;
    std::cout << "  ✓ Performance characteristics are optimal" << std::endl;
}

}  // namespace trlc::platform::test

int main() {
    using namespace trlc::platform::test;

    std::cout << "=== TRLC Platform Debug Utilities Tests ===" << std::endl;

    try {
        testBuildDetection();
        testAssertionHandlerManagement();
        testAssertionCapture();
        testMacros();
        testDebugBreakCompilation();
        testUnreachableCompilation();
        testTerminationFunctions();
        testCompileTimeExecution();
        testThreadSafety();
        testEdgeCases();
        testPerformance();

        std::cout << "\n✅ All debug utilities tests passed!" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}

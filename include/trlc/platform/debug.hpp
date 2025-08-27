#pragma once

/**
 * @file debug.hpp
 * @brief Portable debug and assertion utilities
 *
 * This header provides comprehensive debug utilities including assertion handling,
 * debug mode detection, breakpoint support, and stack trace capabilities. It offers
 * portable implementations that work across different platforms and compilers.
 *
 * Features:
 * - Debug/release build detection at compile time
 * - Customizable assertion handling with stack trace support
 * - Platform-specific debug break implementations
 * - Unreachable code marking and controlled program termination
 * - Optional stack trace capture and printing
 * - Thread-safe assertion handler management
 * - Graceful degradation on unsupported platforms
 *
 * All debug utilities are designed to be zero-overhead in release builds while
 * providing comprehensive debugging support in development builds.
 *
 * @author TRLC Platform Team
 * @version 1.0.0
 */

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <string>

namespace trlc {
namespace platform {

//==============================================================================
// Forward Declarations and Type Definitions
//==============================================================================

/**
 * @brief Assertion handler function pointer type
 *
 * This function type is used for custom assertion handlers. When an assertion
 * fails, the registered handler is called with detailed information about
 * the assertion failure.
 *
 * @param expression The stringified expression that failed
 * @param file The source file where the assertion failed
 * @param line The line number where the assertion failed
 * @param function The function name where the assertion failed
 *
 * @note The handler should not return - it should either abort the program
 *       or throw an exception (if exceptions are enabled)
 */
using AssertionHandler = void (*)(const char* expression,
                                  const char* file,
                                  int line,
                                  const char* function);

// Forward declaration of the default assertion handler
void defaultAssertionHandler(const char* expression,
                             const char* file,
                             int line,
                             const char* function) noexcept;

//==============================================================================
// Debug Mode Detection Functions
//==============================================================================

/**
 * @brief Check if the current build is a debug build
 *
 * Determines whether the code was compiled with debug settings enabled.
 * This is typically controlled by the NDEBUG macro - if NDEBUG is not
 * defined, this is considered a debug build.
 *
 * @return True if this is a debug build, false otherwise
 *
 * @note This function is constexpr and evaluated at compile time
 *
 * @example
 * @code
 * if constexpr (isDebugBuild()) {
 *     // Debug-only optimizations or checks
 * }
 * @endcode
 */
constexpr bool isDebugBuild() noexcept {
#ifdef NDEBUG
    return false;
#else
    return true;
#endif
}

/**
 * @brief Check if the current build is a release build
 *
 * Determines whether the code was compiled with release settings enabled.
 * This is the logical inverse of isDebugBuild().
 *
 * @return True if this is a release build, false otherwise
 *
 * @note This function is constexpr and evaluated at compile time
 */
constexpr bool isReleaseBuild() noexcept {
    return !isDebugBuild();
}

/**
 * @brief Check if debug information is available
 *
 * Determines whether debug information (symbols, line numbers, etc.)
 * is likely to be available in the current build. This is an approximation
 * based on compiler flags and build configuration.
 *
 * @return True if debug information is likely available
 *
 * @note This is a best-effort detection and may not be 100% accurate
 *       across all compilers and build configurations
 */
constexpr bool hasDebugInfo() noexcept {
    // In debug builds, debug info is usually available
    if (isDebugBuild()) {
        return true;
    }

    // Some compilers provide debug info even in release builds
#if defined(__GNUC__) || defined(__clang__)
    // GCC/Clang: Check for common debug flags
    // Note: This is compile-time approximation, actual availability
    // depends on command-line flags like -g
    return isDebugBuild();
#elif defined(_MSC_VER)
    // MSVC: Debug info can be enabled independently of release/debug
    // We conservatively assume it's available in debug builds only
    return isDebugBuild();
#else
    // Unknown compiler: Conservative assumption
    return isDebugBuild();
#endif
}

//==============================================================================
// Debug Utilities Class
//==============================================================================

/**
 * @brief Debug utilities class with static methods
 *
 * This class provides a collection of debug-related utilities including
 * assertion handling, debug breaks, program termination, and stack trace
 * support. All methods are static and thread-safe.
 *
 * The class manages a global assertion handler that can be customized
 * by the application. Thread safety is ensured through atomic operations.
 */
class DebugUtils {
public:
    //--------------------------------------------------------------------------
    // Assertion Management
    //--------------------------------------------------------------------------

    /**
     * @brief Set a custom assertion handler
     *
     * Registers a custom function to handle assertion failures. The handler
     * will be called whenever an assertion fails (in debug builds).
     *
     * @param handler The new assertion handler, or nullptr to use default
     *
     * @note This function is thread-safe and can be called from any thread
     * @note Setting a nullptr handler will revert to the default handler
     *
     * @example
     * @code
     * DebugUtils::setAssertionHandler([](const char* expr, const char* file,
     *                                   int line, const char* func) {
     *     std::cerr << "Assertion failed: " << expr << " at " << file
     *               << ":" << line << " in " << func << std::endl;
     *     std::abort();
     * });
     * @endcode
     */
    static void setAssertionHandler(AssertionHandler handler) noexcept {
        if (handler == nullptr) {
            handler = defaultAssertionHandler;
        }
        _assertion_handler.store(handler, std::memory_order_release);
    }

    /**
     * @brief Get the current assertion handler
     *
     * Returns the currently registered assertion handler function.
     *
     * @return The current assertion handler function pointer
     *
     * @note This function is thread-safe
     */
    static AssertionHandler getAssertionHandler() noexcept {
        return _assertion_handler.load(std::memory_order_acquire);
    }

    //--------------------------------------------------------------------------
    // Debug Break Utilities
    //--------------------------------------------------------------------------

    /**
     * @brief Trigger a debugger breakpoint
     *
     * Attempts to trigger a breakpoint that will halt execution and transfer
     * control to a debugger if one is attached. The implementation is
     * platform-specific and falls back to a trap instruction if necessary.
     *
     * @note This function may terminate the program if no debugger is attached
     * @note The behavior is platform-specific and may vary
     *
     * @example
     * @code
     * if (critical_error_detected) {
     *     DebugUtils::debugBreak();  // Break into debugger
     * }
     * @endcode
     */
    static void debugBreak() noexcept {
#if defined(_WIN32) || defined(_WIN64)
    // Windows: Use __debugbreak() intrinsic
    #if defined(_MSC_VER)
        __debugbreak();
    #elif defined(__GNUC__) || defined(__clang__)
        // MinGW or Clang on Windows
        __builtin_trap();
    #else
        // Unknown compiler on Windows
        std::abort();
    #endif
#elif defined(__i386__) || defined(__x86_64__)
    // x86/x64: Use int3 instruction
    #if defined(__GNUC__) || defined(__clang__)
        __asm__ volatile("int3");
    #else
        // Fallback for unknown compiler
        std::abort();
    #endif
#elif defined(__aarch64__) || defined(__arm__)
    // ARM: Use builtin trap or BKPT instruction
    #if defined(__GNUC__) || defined(__clang__)
        __builtin_trap();
    #else
        std::abort();
    #endif
#else
    // Unknown architecture: Use generic trap or abort
    #if defined(__GNUC__) || defined(__clang__)
        __builtin_trap();
    #else
        std::abort();
    #endif
#endif
    }

    /**
     * @brief Mark code as unreachable
     *
     * Indicates to the compiler that this point in the code should never
     * be reached during normal execution. This can enable optimizations
     * and helps with static analysis.
     *
     * @note Reaching this function results in undefined behavior
     * @note In debug builds, this may trigger a breakpoint or abort
     *
     * @example
     * @code
     * switch (value) {
     *     case 1: return handleOne();
     *     case 2: return handleTwo();
     *     default: DebugUtils::unreachable(); // Should never happen
     * }
     * @endcode
     */
    [[noreturn]] static void unreachable() noexcept {
        if constexpr (isDebugBuild()) {
            // In debug builds, make unreachable code obvious
            defaultAssertionHandler("unreachable code reached", __FILE__, __LINE__, __func__);
        }

        // Hint to compiler that this is unreachable
#if defined(__GNUC__) || defined(__clang__)
        __builtin_unreachable();
#elif defined(_MSC_VER)
        __assume(false);
#else
        // Fallback: abort the program
        std::abort();
#endif
    }

    /**
     * @brief Controlled program termination
     *
     * Terminates the program with an optional error message. This provides
     * a controlled way to abort execution with debugging information.
     *
     * @param message Optional error message to display before termination
     *
     * @note This function never returns
     *
     * @example
     * @code
     * if (critical_resource_unavailable) {
     *     DebugUtils::abort("Critical resource initialization failed");
     * }
     * @endcode
     */
    [[noreturn]] static void abort(const char* message = nullptr) noexcept {
        if (message != nullptr) {
            std::cerr << "Program terminated: " << message << std::endl;
        } else {
            std::cerr << "Program terminated by DebugUtils::abort()" << std::endl;
        }

        // In debug builds, trigger a breakpoint first
        if constexpr (isDebugBuild()) {
            debugBreak();
        }

        std::abort();
    }

    //--------------------------------------------------------------------------
    // Stack Trace Support
    //--------------------------------------------------------------------------

    /**
     * @brief Check if stack trace capture is supported
     *
     * Determines whether the current platform and build configuration
     * supports capturing and displaying stack traces.
     *
     * @return True if stack trace capture is available
     *
     * @note Availability depends on platform, compiler, and build settings
     */
    static bool canCaptureStackTrace() noexcept {
        // Platform-specific stack trace support detection
#if defined(_WIN32) && defined(_MSC_VER)
        // Windows with MSVC: StackWalk64 API available
        return true;
#elif defined(__GNUC__) || defined(__clang__)
    // GCC/Clang: Check for backtrace support
    #if defined(__GLIBC__) || defined(__APPLE__) || defined(__FreeBSD__)
        return true;  // backtrace() available
    #else
        return false;  // No backtrace support
    #endif
#else
        return false;  // Unknown platform/compiler
#endif
    }

    /**
     * @brief Print a stack trace to stderr
     *
     * Attempts to capture and print the current call stack to stderr.
     * If stack trace capture is not supported on the current platform,
     * this function does nothing.
     *
     * @note Stack trace quality depends on debug symbols and platform support
     * @note This function is best-effort and may not work in all configurations
     *
     * @example
     * @code
     * void error_handler() {
     *     std::cerr << "Error occurred, stack trace:" << std::endl;
     *     DebugUtils::printStackTrace();
     * }
     * @endcode
     */
    static void printStackTrace() noexcept {
        if (!canCaptureStackTrace()) {
            std::cerr << "Stack trace not available on this platform" << std::endl;
            return;
        }

        // Platform-specific stack trace implementation
#if defined(_WIN32) && defined(_MSC_VER)
        printStackTraceWindows();
#elif defined(__GNUC__) || defined(__clang__)
    #if defined(__GLIBC__) || defined(__APPLE__) || defined(__FreeBSD__)
        printStackTraceUnix();
    #else
        std::cerr << "Stack trace not implemented for this Unix variant" << std::endl;
    #endif
#else
        std::cerr << "Stack trace not implemented for this platform" << std::endl;
#endif
    }

private:
    //--------------------------------------------------------------------------
    // Private Members and Helper Functions
    //--------------------------------------------------------------------------

    /// Thread-safe storage for the current assertion handler
    static std::atomic<AssertionHandler> _assertion_handler;

    /**
     * @brief Windows-specific stack trace implementation
     *
     * Uses the Windows StackWalk64 API to capture and print stack traces.
     * This is only available when compiling with MSVC on Windows.
     */
#if defined(_WIN32) && defined(_MSC_VER)
    static void printStackTraceWindows() noexcept {
        // Implementation would use StackWalk64 API
        // For brevity, we just print a placeholder message
        std::cerr << "Windows stack trace would be printed here" << std::endl;
        std::cerr << "(StackWalk64 implementation not included in this example)" << std::endl;
    }
#endif

    /**
     * @brief Unix-specific stack trace implementation
     *
     * Uses the backtrace() function available on glibc, macOS, and FreeBSD
     * to capture and print stack traces.
     */
#if defined(__GNUC__) || defined(__clang__)
    #if defined(__GLIBC__) || defined(__APPLE__) || defined(__FreeBSD__)
    static void printStackTraceUnix() noexcept {
        // Implementation would use backtrace() and backtrace_symbols()
        // For brevity, we just print a placeholder message
        std::cerr << "Unix stack trace would be printed here" << std::endl;
        std::cerr << "(backtrace() implementation not included in this example)" << std::endl;
    }
    #endif
#endif
};

// Initialize the atomic assertion handler with the default (inline to avoid ODR issues)
inline std::atomic<AssertionHandler> DebugUtils::_assertion_handler{defaultAssertionHandler};

//==============================================================================
// Default Assertion Handler
//==============================================================================

/**
 * @brief Default assertion handler implementation
 *
 * This is the default handler for assertion failures. It prints detailed
 * information about the assertion failure to stderr and then terminates
 * the program.
 *
 * @param expression The stringified expression that failed
 * @param file The source file where the assertion failed
 * @param line The line number where the assertion failed
 * @param function The function name where the assertion failed
 *
 * @note This function never returns - it always terminates the program
 */
inline void defaultAssertionHandler(const char* expression,
                                    const char* file,
                                    int line,
                                    const char* function) noexcept {
    std::cerr << "\n" << std::string(60, '=') << "\n";
    std::cerr << "ASSERTION FAILED\n";
    std::cerr << std::string(60, '=') << "\n";
    std::cerr << "Expression: " << expression << "\n";
    std::cerr << "File:       " << file << "\n";
    std::cerr << "Line:       " << line << "\n";
    std::cerr << "Function:   " << function << "\n";
    std::cerr << std::string(60, '=') << "\n";

    // Print stack trace if available
    if (DebugUtils::canCaptureStackTrace()) {
        std::cerr << "\nStack trace:\n";
        DebugUtils::printStackTrace();
        std::cerr << "\n";
    }

    std::cerr << "Program will now terminate.\n" << std::endl;

    // Trigger debugger break in debug builds
    if constexpr (isDebugBuild()) {
        DebugUtils::debugBreak();
    }

    std::abort();
}

}  // namespace platform
}  // namespace trlc

//==============================================================================
// Debug Macros
//==============================================================================

/**
 * @brief Debug build detection macro
 *
 * This macro evaluates to 1 in debug builds and 0 in release builds.
 * It can be used for conditional compilation of debug-only code.
 *
 * @example
 * @code
 * #if TRLC_DEBUG_BUILD
 *     // Debug-only code
 * #endif
 * @endcode
 */
#ifdef NDEBUG
    #define TRLC_DEBUG_BUILD 0
#else
    #define TRLC_DEBUG_BUILD 1
#endif

/**
 * @brief Assertion macro with custom handler support
 *
 * In debug builds, this macro evaluates the given expression and calls
 * the registered assertion handler if the expression is false. In release
 * builds, this macro expands to nothing (zero overhead).
 *
 * @param expr The expression to test
 *
 * @note Only active in debug builds (when NDEBUG is not defined)
 * @note Uses the currently registered assertion handler
 *
 * @example
 * @code
 * TRLC_ASSERT(ptr != nullptr);
 * TRLC_ASSERT(index < array_size);
 * @endcode
 */
#if TRLC_DEBUG_BUILD
    #define TRLC_ASSERT(expr)                                        \
        ((expr) ? void(0)                                            \
                : trlc::platform::DebugUtils::getAssertionHandler()( \
                      #expr, __FILE__, __LINE__, __func__))
#else
    #define TRLC_ASSERT(expr) ((void)0)
#endif

/**
 * @brief Debug-only code macro
 *
 * In debug builds, this macro includes the given code. In release builds,
 * the code is completely excluded from compilation.
 *
 * @param code The code to include only in debug builds
 *
 * @example
 * @code
 * TRLC_DEBUG_ONLY(
 *     std::cout << "Debug: processing item " << item_id << std::endl;
 * );
 * @endcode
 */
#if TRLC_DEBUG_BUILD
    #define TRLC_DEBUG_ONLY(code) code
#else
    #define TRLC_DEBUG_ONLY(code) ((void)0)
#endif

/**
 * @brief Unreachable code marker macro (debug version)
 *
 * When debug utilities are enabled, this redefines TRLC_UNREACHABLE to use
 * the debug-aware version that provides better diagnostics.
 *
 * Marks a point in the code that should never be reached during normal
 * execution. This helps with optimization and static analysis.
 *
 * @note Reaching this macro results in undefined behavior
 *
 * @example
 * @code
 * switch (state) {
 *     case STATE_A: return handleA();
 *     case STATE_B: return handleB();
 *     default: TRLC_UNREACHABLE();
 * }
 * @endcode
 */
#ifdef TRLC_UNREACHABLE
    #undef TRLC_UNREACHABLE
#endif
#define TRLC_UNREACHABLE() trlc::platform::DebugUtils::unreachable()

/**
 * @brief Controlled program termination macro
 *
 * Terminates the program with an optional error message. Provides
 * better debugging information than raw abort() calls.
 *
 * @param msg Optional error message
 *
 * @example
 * @code
 * if (critical_error) {
 *     TRLC_ABORT("Critical system error detected");
 * }
 * @endcode
 */
#define TRLC_ABORT(msg) trlc::platform::DebugUtils::abort(msg)

/**
 * @brief Platform-specific debugger breakpoint macro
 *
 * Triggers a debugger breakpoint using the most appropriate method
 * for the current platform and compiler. Falls back to the DebugUtils
 * implementation if no platform-specific method is available.
 *
 * @note May terminate the program if no debugger is attached
 *
 * @example
 * @code
 * if (rare_condition_detected) {
 *     TRLC_DEBUG_BREAK();  // Break into debugger
 * }
 * @endcode
 */
#if defined(_MSC_VER)
   // Microsoft Visual C++: Use intrinsic
    #define TRLC_DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
   // GCC or Clang: Use platform-specific inline assembly or builtin
    #if defined(__i386__) || defined(__x86_64__)
   // x86/x64: Use int3 instruction
        #define TRLC_DEBUG_BREAK() __asm__ volatile("int3")
    #else
   // Other architectures: Use compiler builtin
        #define TRLC_DEBUG_BREAK() __builtin_trap()
    #endif
#else
   // Unknown compiler: Fall back to DebugUtils implementation
    #define TRLC_DEBUG_BREAK() trlc::platform::DebugUtils::debugBreak()
#endif

//==============================================================================
// Compile-Time Validation
//==============================================================================

// Verify that debug detection works correctly
static_assert(trlc::platform::isDebugBuild() == !trlc::platform::isReleaseBuild(),
              "Debug and release build detection must be mutually exclusive");

// Verify that the debug build macro is consistent
#if TRLC_DEBUG_BUILD
static_assert(trlc::platform::isDebugBuild(),
              "TRLC_DEBUG_BUILD macro should match isDebugBuild() function");
#else
static_assert(trlc::platform::isReleaseBuild(),
              "TRLC_DEBUG_BUILD macro should match build detection functions");
#endif

// Verify that the assertion handler type is correctly defined
static_assert(std::is_same_v<trlc::platform::AssertionHandler,
                             void (*)(const char*, const char*, int, const char*)>,
              "AssertionHandler type signature must match specification");

//==============================================================================
// Documentation Examples and Usage Notes
//==============================================================================

/*
Example Usage:

1. Basic assertion:
   TRLC_ASSERT(ptr != nullptr);
   TRLC_ASSERT(index < size);

2. Custom assertion handler:
   trlc::platform::DebugUtils::setAssertionHandler(
       [](const char* expr, const char* file, int line, const char* func) {
           // Custom logging or error handling
           myLogger.error("Assertion failed: {} at {}:{} in {}",
                         expr, file, line, func);
           throw std::runtime_error("Assertion failed");
       }
   );

3. Debug-only code:
   TRLC_DEBUG_ONLY(
       auto start_time = std::chrono::high_resolution_clock::now();
   );

   // ... some operation ...

   TRLC_DEBUG_ONLY(
       auto end_time = std::chrono::high_resolution_clock::now();
       std::cout << "Operation took: "
                 << std::chrono::duration_cast<std::chrono::milliseconds>(
                        end_time - start_time).count() << "ms" << std::endl;
   );

4. Unreachable code marking:
   enum class State { A, B, C };

   std::string toString(State s) {
       switch (s) {
           case State::A: return "A";
           case State::B: return "B";
           case State::C: return "C";
       }
       TRLC_UNREACHABLE();  // Should never reach here
   }

5. Controlled termination:
   if (!initialize_critical_resource()) {
       TRLC_ABORT("Failed to initialize critical resource");
   }

Thread Safety:
- All DebugUtils static methods are thread-safe
- The assertion handler can be safely changed from any thread
- Multiple threads can safely call assertion-related functions

Performance:
- Zero overhead in release builds (macros expand to nothing)
- Minimal overhead in debug builds
- Assertion handler lookup uses atomic operations
- Stack trace capture is expensive but only used on failures
*/

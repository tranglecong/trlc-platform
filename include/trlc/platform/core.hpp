#pragma once

/**
 * @file core.hpp
 * @brief Main entry point for TRLC platform detection and abstraction
 *
 * This header serves as the primary interface for the TRLC platform library.
 * It includes all platform detection modules in the correct dependency order
 * and provides consolidated reporting and initialization functionality.
 *
 * The core module aggregates information from all other modules into a
 * unified PlatformReport structure that can generate human-readable reports
 * about the detected platform, compiler, architecture, and features.
 *
 * Features:
 * - Consolidated platform information reporting
 * - Optional debug utilities (when enabled)
 * - Runtime feature initialization
 * - Version information and library metadata
 * - Thread-safe initialization for runtime features
 *
 * @author TRLC Platform Team
 * @version 1.0.0
 */

#include <atomic>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

// Include all platform detection headers in dependency order
#include "trlc/platform/architecture.hpp"
#include "trlc/platform/compiler.hpp"
#include "trlc/platform/cpp_standard.hpp"
#include "trlc/platform/endianness.hpp"
#include "trlc/platform/features.hpp"
#include "trlc/platform/macros.hpp"
#include "trlc/platform/platform.hpp"
#include "trlc/platform/typeinfo.hpp"

// Conditionally include debug utilities
#ifdef TRLC_PLATFORM_ENABLE_DEBUG_UTILS
    #include "trlc/platform/debug.hpp"
#endif

namespace trlc {
namespace platform {

//==============================================================================
// Version Information
//==============================================================================

/**
 * @brief Library version information
 *
 * Provides compile-time access to the library version number and
 * version string for compatibility checking and reporting.
 */
struct Version {
    /// Major version number (breaking changes)
    static constexpr int MAJOR = 1;

    /// Minor version number (feature additions)
    static constexpr int MINOR = 0;

    /// Patch version number (bug fixes)
    static constexpr int PATCH = 0;

    /// Version string in semantic versioning format
    static constexpr const char* STRING = "1.0.0";

    /**
     * @brief Get the version as a 32-bit integer
     * @return Version encoded as (major << 16) | (minor << 8) | patch
     */
    static constexpr uint32_t asInteger() noexcept {
        return (static_cast<uint32_t>(MAJOR) << 16) | (static_cast<uint32_t>(MINOR) << 8) |
               static_cast<uint32_t>(PATCH);
    }

    /**
     * @brief Check if this version is at least the specified version
     * @param major Minimum major version
     * @param minor Minimum minor version
     * @param patch Minimum patch version
     * @return true if current version >= specified version
     */
    static constexpr bool isAtLeast(int major, int minor = 0, int patch = 0) noexcept {
        uint32_t current = asInteger();
        uint32_t required = (static_cast<uint32_t>(major) << 16) |
                            (static_cast<uint32_t>(minor) << 8) | static_cast<uint32_t>(patch);
        return current >= required;
    }
};

//==============================================================================
// Consolidated Platform Report
//==============================================================================

/**
 * @brief Consolidated platform information structure
 *
 * This structure aggregates all platform detection results from individual
 * modules into a single convenient package for reporting and analysis.
 *
 * All information is gathered at compile time where possible, with runtime
 * features detected during initialization if needed.
 */
struct PlatformReport {
    /// Compiler information (type, version, capabilities)
    CompilerInfo compiler;

    /// Platform/OS information (type, environment, API support)
    PlatformInfo platform;

    /// CPU architecture information (type, endianness, capabilities)
    ArchitectureInfo architecture;

    /// C++ standard information (version, feature support)
    CppStandardInfo cpp_standard;

    /// Language and runtime feature availability
    FeatureSet features;

    /// Endianness information (byte order, utilities)
    EndiannessInfo endianness;

    /**
     * @brief Generate a human-readable platform report
     *
     * Creates a comprehensive text report containing all detected platform
     * information in a structured, easily readable format. This is useful
     * for logging, debugging, and system analysis.
     *
     * @return Formatted string containing complete platform information
     *
     * @example
     * @code
     * auto report = getPlatformReport();
     * std::cout << report.generateReport() << std::endl;
     * @endcode
     */
    std::string generateReport() const {
        std::stringstream ss;

        ss << "TRLC Platform Detection Report v" << Version::STRING << "\n";
        ss << std::string(60, '=') << "\n\n";

        // Compiler Information
        ss << "COMPILER INFORMATION:\n";
        ss << std::string(25, '-') << "\n";
        ss << "  Type:                " << compiler.name << "\n";
        ss << "  Version:             " << compiler.version.major << "." << compiler.version.minor
           << "." << compiler.version.patch << "\n";
        ss << "  Builtin Attributes:  " << (compiler.has_builtin_attribute ? "Yes" : "No") << "\n";
        ss << "  Inline Assembly:     " << (compiler.has_inline_assembly ? "Yes" : "No") << "\n";
        ss << "  Color Diagnostics:   " << (compiler.has_color_diagnostics ? "Yes" : "No") << "\n";
        ss << "  GCC Compatible:      " << (compiler.isGccCompatible() ? "Yes" : "No") << "\n";
        ss << "  Clang Compatible:    " << (compiler.isClangCompatible() ? "Yes" : "No") << "\n\n";

        // Platform Information
        ss << "PLATFORM INFORMATION:\n";
        ss << std::string(25, '-') << "\n";
        ss << "  Operating System:    " << platform.os_name << "\n";
        ss << "  Kernel Family:       " << platform.kernel_family << "\n";
        ss << "  Environment Type:    ";
        switch (platform.environment) {
            case EnvironmentType::desktop:
                ss << "Desktop";
                break;
            case EnvironmentType::server:
                ss << "Server";
                break;
            case EnvironmentType::embedded:
                ss << "Embedded";
                break;
            case EnvironmentType::mobile:
                ss << "Mobile";
                break;
            default:
                ss << "Unknown";
                break;
        }
        ss << "\n";
        ss << "  POSIX API:           " << (platform.isPosix() ? "Yes" : "No") << "\n";
        ss << "  Windows API:         " << (platform.isWindows() ? "Yes" : "No") << "\n";
        ss << "  Case Sensitive FS:   " << (supportsCaseSensitiveFilesystem() ? "Yes" : "No")
           << "\n\n";

        // Architecture Information
        ss << "ARCHITECTURE INFORMATION:\n";
        ss << std::string(29, '-') << "\n";
        ss << "  CPU Architecture:    " << architecture.arch_name << "\n";
        ss << "  Pointer Size:        " << architecture.pointer_size_bits << " bits\n";
        ss << "  Byte Order:          ";
        switch (architecture.byte_order) {
            case ByteOrder::little_endian:
                ss << "Little Endian";
                break;
            case ByteOrder::big_endian:
                ss << "Big Endian";
                break;
            case ByteOrder::mixed_endian:
                ss << "Mixed Endian";
                break;
            default:
                ss << "Unknown";
                break;
        }
        ss << "\n";
        ss << "  Cache Line Size:     " << architecture.cache_line_size << " bytes\n";
        ss << "  Unaligned Access:    " << (architecture.supportsUnalignedAccess() ? "Yes" : "No")
           << "\n";
        ss << "  SIMD Support:        " << (hasSimdSupport() ? "Yes" : "No") << "\n\n";

        // C++ Standard Information
        ss << "C++ STANDARD INFORMATION:\n";
        ss << std::string(29, '-') << "\n";
        ss << "  Standard Version:    " << cpp_standard.standard_name << "\n";
        ss << "  Version Macro:       " << cpp_standard.version_macro << "\n";
        ss << "  Structured Bindings: " << (hasStructuredBindings() ? "Yes" : "No") << "\n";
        ss << "  If Constexpr:        " << (hasIfConstexpr() ? "Yes" : "No") << "\n";
        ss << "  Concepts:            " << (hasConcepts() ? "Yes" : "No") << "\n";
        ss << "  Coroutines:          " << (hasCoroutines() ? "Yes" : "No") << "\n";
        ss << "  Modules:             " << (hasModules() ? "Yes" : "No") << "\n";
        ss << "  Ranges:              " << (hasRanges() ? "Yes" : "No") << "\n\n";

        // Feature Information
        ss << "FEATURE AVAILABILITY:\n";
        ss << std::string(25, '-') << "\n";
        ss << "  Exceptions:          " << (features.has_exceptions ? "Yes" : "No") << "\n";
        ss << "  RTTI:                " << (features.has_rtti ? "Yes" : "No") << "\n";
        ss << "  Threads:             " << (features.has_threads ? "Yes" : "No") << "\n";
        ss << "  Atomic Operations:   " << (features.has_atomic ? "Yes" : "No") << "\n";
        ss << "  Inline Assembly:     " << (features.has_inline_asm ? "Yes" : "No") << "\n";
        ss << "  SSE Support:         " << (features.has_sse ? "Yes" : "No") << "\n";
        ss << "  AVX Support:         " << (features.has_avx ? "Yes" : "No") << "\n";
        ss << "  NEON Support:        " << (features.has_neon ? "Yes" : "No") << "\n\n";

        // Endianness Information (using data from ArchitectureInfo)
        ss << "ENDIANNESS INFORMATION:\n";
        ss << std::string(27, '-') << "\n";
        ss << "  Byte Order:          ";
        switch (architecture.byte_order) {
            case ByteOrder::little_endian:
                ss << "Little Endian";
                break;
            case ByteOrder::big_endian:
                ss << "Big Endian";
                break;
            case ByteOrder::mixed_endian:
                ss << "Mixed Endian";
                break;
            default:
                ss << "Unknown";
                break;
        }
        ss << "\n";
        ss << "  Little Endian:       " << (architecture.isLittleEndian() ? "Yes" : "No") << "\n";
        ss << "  Big Endian:          "
           << (!architecture.isLittleEndian() && architecture.byte_order == ByteOrder::big_endian
                   ? "Yes"
                   : "No")
           << "\n\n";

        // Debug Information (if available)
#ifdef TRLC_PLATFORM_ENABLE_DEBUG_UTILS
        ss << "DEBUG INFORMATION:\n";
        ss << std::string(22, '-') << "\n";
        ss << "  Debug Build:         " << (isDebugBuild() ? "Yes" : "No") << "\n";
        ss << "  Release Build:       " << (isReleaseBuild() ? "Yes" : "No") << "\n";
        ss << "  Debug Info:          " << (hasDebugInfo() ? "Yes" : "No") << "\n";
        ss << "  Stack Trace:         " << (DebugUtils::canCaptureStackTrace() ? "Yes" : "No")
           << "\n\n";
#endif

        ss << std::string(60, '=') << "\n";
        ss << "Report generated by TRLC Platform v" << Version::STRING << "\n";

        return ss.str();
    }

    /**
     * @brief Print the platform report to stdout
     *
     * Convenience method that generates and prints the platform report
     * to standard output. Equivalent to std::cout << generateReport().
     *
     * @example
     * @code
     * auto report = getPlatformReport();
     * report.printReport();  // Print to console
     * @endcode
     */
    void printReport() const { std::cout << generateReport() << std::endl; }

    /**
     * @brief Get a brief one-line summary of the platform
     * @return Short platform description string
     */
    std::string getBriefSummary() const {
        std::stringstream ss;
        ss << compiler.name << " " << compiler.version.major << "." << compiler.version.minor
           << " on " << platform.os_name << " " << architecture.arch_name << " ("
           << architecture.pointer_size_bits << "-bit)";
        return ss.str();
    }
};

//==============================================================================
// Core Platform Detection Function
//==============================================================================

/**
 * @brief Get comprehensive platform information
 *
 * Collects and returns all platform detection results in a single
 * consolidated structure. This function aggregates information from
 * all detection modules and is the primary interface for obtaining
 * complete platform information.
 *
 * @return PlatformReport structure containing all detected information
 *
 * @note This function is constexpr where possible, but some runtime
 *       features may require initialization via initializePlatform()
 *
 * @example
 * @code
 * constexpr auto report = getPlatformReport();
 * if constexpr (report.compiler.type == CompilerType::gcc) {
 *     // GCC-specific optimizations
 * }
 * @endcode
 */
inline PlatformReport getPlatformReport() noexcept {
    return PlatformReport{
        getCompilerInfo(),
        getPlatformInfo(),
        getArchitectureInfo(),
        getCppStandardInfo(),
        getFeatureSet(),
        getEndiannessInfo()  // Now available from endianness.hpp
    };
}

//==============================================================================
// Runtime Feature Initialization
//==============================================================================

namespace detail {
/// Internal initialization state tracking (inline to avoid ODR violations)
inline std::atomic<bool> g_platform_initialized{false};
inline std::atomic<bool> g_initialization_in_progress{false};
}  // namespace detail

/**
 * @brief Initialize runtime platform features
 *
 * Performs any necessary initialization for runtime feature detection.
 * This is primarily needed for CPU feature detection (SSE, AVX, etc.)
 * that requires runtime checks rather than compile-time detection.
 *
 * This function is thread-safe and idempotent - multiple calls are safe.
 * Only the first call will perform actual initialization.
 *
 * @note Most platform detection is compile-time and doesn't require
 *       initialization. This is only needed for runtime CPU features.
 *
 * @example
 * @code
 * int main() {
 *     trlc::platform::initializePlatform();
 *
 *     // Now runtime features are available
 *     if (trlc::platform::hasAvxSupport()) {
 *         // Use AVX optimizations
 *     }
 * }
 * @endcode
 */
inline void initializePlatform() noexcept {
    // Check if already initialized
    if (detail::g_platform_initialized.load(std::memory_order_acquire)) {
        return;
    }

    // Check if initialization is in progress (prevent recursive calls)
    bool expected = false;
    if (!detail::g_initialization_in_progress.compare_exchange_strong(
            expected, true, std::memory_order_acq_rel)) {
        // Another thread is initializing, wait for completion
        while (!detail::g_platform_initialized.load(std::memory_order_acquire)) {
// Busy wait with yield hint
#if defined(_WIN32)
    #if defined(_MSC_VER)
            __mm_pause();  // Intel x86 pause instruction
    #else
            std::this_thread::yield();
    #endif
#elif defined(__GNUC__) || defined(__clang__)
    #if defined(__x86_64__) || defined(__i386__)
            __builtin_ia32_pause();  // Intel x86 pause instruction
    #else
            std::this_thread::yield();
    #endif
#else
            std::this_thread::yield();
#endif
        }
        return;
    }

    try {
        // Perform runtime feature detection initialization
        // Most features are compile-time, but some CPU features need runtime detection

        // Initialize any runtime feature detection here
        // For now, most features are compile-time detected

        // Mark initialization complete
        detail::g_platform_initialized.store(true, std::memory_order_release);

    } catch (...) {
        // If initialization fails, reset the in-progress flag
        detail::g_initialization_in_progress.store(false, std::memory_order_release);
        // Don't propagate exceptions from initialization
        return;
    }

    // Clear the in-progress flag
    detail::g_initialization_in_progress.store(false, std::memory_order_release);
}

/**
 * @brief Check if platform initialization has been completed
 *
 * Returns whether initializePlatform() has been successfully called
 * and completed. This can be used to verify that runtime features
 * are available before using them.
 *
 * @return true if platform initialization is complete
 *
 * @note Compile-time features are always available regardless of
 *       initialization status. This only affects runtime features.
 *
 * @example
 * @code
 * if (!isPlatformInitialized()) {
 *     initializePlatform();
 * }
 *
 * // Now safe to use runtime features
 * bool has_avx = hasAvxSupport();
 * @endcode
 */
inline bool isPlatformInitialized() noexcept {
    return detail::g_platform_initialized.load(std::memory_order_acquire);
}

//==============================================================================
// Convenience Functions
//==============================================================================

/**
 * @brief Get a brief platform summary string
 *
 * Returns a one-line description of the current platform suitable
 * for logging or display purposes.
 *
 * @return Brief platform description
 *
 * @example
 * @code
 * std::cout << "Running on: " << getBriefPlatformSummary() << std::endl;
 * // Output: "Running on: GCC 11.2 on Linux x86_64 (64-bit)"
 * @endcode
 */
inline std::string getBriefPlatformSummary() {
    auto report = getPlatformReport();
    return report.getBriefSummary();
}

/**
 * @brief Print platform information to stdout
 *
 * Convenience function that generates and prints a complete platform
 * report to standard output. Equivalent to getPlatformReport().printReport().
 *
 * @example
 * @code
 * printPlatformReport();  // Print complete report to console
 * @endcode
 */
inline void printPlatformReport() {
    auto report = getPlatformReport();
    report.printReport();
}

}  // namespace platform
}  // namespace trlc

//==============================================================================
// Master Include Guard and Compatibility Macros
//==============================================================================

/// Master include guard indicating that TRLC platform library has been included
#define TRLC_PLATFORM_INCLUDED

/// Library version macros for preprocessor use
#define TRLC_PLATFORM_VERSION_MAJOR (trlc::platform::Version::MAJOR)
#define TRLC_PLATFORM_VERSION_MINOR (trlc::platform::Version::MINOR)
#define TRLC_PLATFORM_VERSION_PATCH (trlc::platform::Version::PATCH)
#define TRLC_PLATFORM_VERSION_STRING (trlc::platform::Version::STRING)

/// Check if platform version is at least the specified version
#define TRLC_PLATFORM_VERSION_ATLEAST(major, minor, patch) \
    (trlc::platform::Version::isAtLeast(major, minor, patch))

//==============================================================================
// Compile-Time Validation
//==============================================================================

// Verify that all required modules are available
static_assert(std::is_default_constructible_v<trlc::platform::CompilerInfo>,
              "CompilerInfo must be default constructible");
static_assert(std::is_default_constructible_v<trlc::platform::PlatformInfo>,
              "PlatformInfo must be default constructible");
static_assert(std::is_default_constructible_v<trlc::platform::ArchitectureInfo>,
              "ArchitectureInfo must be default constructible");
static_assert(std::is_default_constructible_v<trlc::platform::CppStandardInfo>,
              "CppStandardInfo must be default constructible");
static_assert(std::is_default_constructible_v<trlc::platform::FeatureSet>,
              "FeatureSet must be default constructible");

// Verify version information
static_assert(trlc::platform::Version::MAJOR >= 1, "Library major version must be at least 1");
static_assert(trlc::platform::Version::isAtLeast(1, 0, 0),
              "Version comparison must work correctly");

// Verify that the PlatformReport can be created
static_assert(std::is_default_constructible_v<trlc::platform::PlatformReport>,
              "PlatformReport must be constructible");

//==============================================================================
// Documentation Examples and Usage Notes
//==============================================================================

/*
Example Usage:

1. Basic platform detection:
   #include "trlc/platform/core.hpp"

   constexpr auto platform = trlc::platform::getPlatformReport();
   if constexpr (platform.compiler.type == trlc::platform::CompilerType::gcc) {
       // GCC-specific code
   }

2. Runtime initialization for CPU features:
   #include "trlc/platform/core.hpp"

   int main() {
       trlc::platform::initializePlatform();

       if (trlc::platform::hasAvxSupport()) {
           // Use AVX optimizations
       }
   }

3. Platform reporting:
   #include "trlc/platform/core.hpp"

   void logPlatformInfo() {
       auto report = trlc::platform::getPlatformReport();
       std::cout << report.generateReport() << std::endl;
   }

4. Brief platform summary:
   #include "trlc/platform/core.hpp"

   std::string getPlatformSummary() {
       return trlc::platform::getBriefPlatformSummary();
   }

5. Conditional compilation based on platform:
   #include "trlc/platform/core.hpp"

   void optimizedFunction() {
       constexpr auto platform = trlc::platform::getPlatformReport();

       if constexpr (platform.architecture.is64Bit() &&
                     platform.features.has_sse) {
           // 64-bit SSE optimized path
       } else {
           // Generic fallback
       }
   }

Thread Safety:
- All compile-time detection is inherently thread-safe
- Runtime initialization is thread-safe and idempotent
- Platform report generation is thread-safe
- Multiple threads can safely call all functions concurrently

Performance:
- Compile-time detection has zero runtime overhead
- Runtime initialization has minimal one-time cost
- Report generation has moderate cost (string formatting)
- Brief summaries are lightweight for frequent use

Integration:
- Header-only design requires no separate compilation
- All dependencies are standard library or self-contained
- Can be included in any C++17 or later project
- Works with all major compilers and platforms
*/

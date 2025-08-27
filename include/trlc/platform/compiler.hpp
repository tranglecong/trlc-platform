#pragma once

/**
 * @file compiler.hpp
 * @brief Compiler detection and capability utilities
 *
 * @details This header provides comprehensive compile-time compiler detection, 
 * version parsing, and capability checking for cross-platform C++ development.
 * It enables code to adapt to different compiler capabilities and optimize
 * for specific compiler features.
 *
 * ## Features
 * - **Compiler Identification**: Detect GCC, Clang, MSVC, Intel compilers
 * - **Version Detection**: Parse major.minor.patch version numbers
 * - **Capability Checking**: Test for specific compiler features and optimizations
 * - **Optimization Hints**: Compiler-specific optimization directives
 * - **Warning Control**: Compiler-specific warning management
 * - **Standard Compliance**: C++ standard support detection
 *
 * ## Supported Compilers
 * - **GCC**: GNU Compiler Collection (all versions)
 * - **Clang**: LLVM Clang compiler (all versions)
 * - **MSVC**: Microsoft Visual C++ (VS2015+)
 * - **Intel Classic**: Legacy Intel C++ Compiler
 * - **Intel LLVM**: New Intel C++ Compiler based on LLVM
 * - **MinGW**: MinGW-w64 and legacy MinGW
 *
 * ## Performance Characteristics
 * - **Zero Runtime Overhead**: All detection occurs at compile time
 * - **Header-Only**: No linking required
 * - **Optimizable**: Enables compiler-specific optimizations
 * - **Thread-Safe**: Purely compile-time evaluation
 *
 * ## Thread Safety
 * All functions in this header are thread-safe as they perform only compile-time
 * evaluation and access no mutable state.
 *
 * @example Basic compiler detection
 * @code
 * #include "trlc/platform/compiler.hpp"
 * 
 * constexpr auto compiler = trlc::platform::getCompilerType();
 * if constexpr (compiler == trlc::platform::CompilerType::gcc) {
 *     // GCC-specific optimizations
 * } else if constexpr (compiler == trlc::platform::CompilerType::clang) {
 *     // Clang-specific features
 * }
 * @endcode
 *
 * @example Version-dependent features
 * @code
 * constexpr auto version = trlc::platform::getCompilerVersion();
 * if constexpr (trlc::platform::getCompilerType() == trlc::platform::CompilerType::gcc &&
 *               version >= trlc::platform::CompilerVersion{9, 0}) {
 *     // Use GCC 9+ features
 * }
 * @endcode
 *
 * @author TRLC Platform Team
 * @version 1.0.0
 * @since 1.0.0
 * @see architecture.hpp for CPU architecture detection
 * @see features.hpp for language feature detection
 *
 * @copyright Copyright (c) 2025 TRLC Platform
 */

namespace trlc {
namespace platform {

/**
 * @brief Compiler identification enumeration
 *
 * @details Represents the detected compiler type at compile time. This enumeration
 * covers all major C++ compilers used in modern development, enabling
 * compiler-specific optimizations and feature detection.
 *
 * ## Compiler Detection Strategy
 * Detection uses compiler-specific predefined macros in order of specificity:
 * 1. **Intel compilers** (checked first due to macro conflicts)
 * 2. **Clang** (checked before GCC as it often defines GCC macros)
 * 3. **GCC** (including GCC-compatible compilers)
 * 4. **MSVC** (Microsoft Visual C++)
 * 5. **MinGW** (GCC-based compiler for Windows)
 *
 * ## Compiler Characteristics
 * 
 * ### GNU Compiler Collection (GCC)
 * - **Platforms**: Linux, Windows (MinGW), macOS, embedded systems
 * - **Standards**: Excellent C++11/14/17/20/23 support
 * - **Optimization**: Strong optimization capabilities
 * - **Extensions**: GNU-specific language extensions
 *
 * ### Clang/LLVM
 * - **Platforms**: Linux, macOS, Windows, embedded systems
 * - **Standards**: Leading C++ standard compliance
 * - **Diagnostics**: Superior error messages and warnings
 * - **Tooling**: Excellent static analysis and tooling support
 *
 * ### Microsoft Visual C++ (MSVC)
 * - **Platforms**: Windows (primary), some cross-platform support
 * - **Standards**: Good C++11/14/17/20 support, improving C++23
 * - **Integration**: Tight Visual Studio integration
 * - **Optimization**: Excellent Windows-specific optimizations
 *
 * ### Intel Compilers
 * - **Intel Classic**: Legacy Intel C++ Compiler (being phased out)
 * - **Intel LLVM**: New Intel compiler based on LLVM infrastructure
 * - **Optimization**: Excellent performance on Intel hardware
 * - **Vectorization**: Superior auto-vectorization capabilities
 *
 * @note Compiler detection is performed using predefined macros and is
 * evaluated at compile time for zero runtime overhead.
 *
 * @see getCompilerType() for detection function
 * @see getCompilerVersion() for version information
 * @see CompilerInfo for comprehensive compiler details
 * @since 1.0.0
 */
enum class CompilerType : int {
    unknown = 0,    ///< Unknown or unsupported compiler
    gcc,            ///< GNU Compiler Collection - Cross-platform, open-source
    clang,          ///< Clang/LLVM compiler - Modern, standards-compliant
    msvc,           ///< Microsoft Visual C++ compiler - Windows-focused
    intel_classic,  ///< Legacy Intel C++ Compiler - Performance-optimized
    intel_llvm,     ///< Intel C++ Compiler based on LLVM - Modern Intel toolchain
    mingw           ///< MinGW compiler (GCC-based for Windows) - Windows cross-compilation
};

/**
 * @brief Compiler version representation
 *
 * @details Stores version information with comparison operators for version checking.
 * This structure enables version-dependent feature detection and compatibility
 * checking across different compiler versions.
 *
 * ## Version Format
 * Follows semantic versioning principles with three components:
 * - **Major**: Significant changes, potential breaking changes
 * - **Minor**: New features, backward compatible
 * - **Patch**: Bug fixes, maintenance updates
 *
 * ## Version Comparison
 * Provides standard comparison operators for version checking:
 * - Equality: `==`, `!=`
 * - Ordering: `<`, `<=`, `>`, `>=`
 * - Lexicographic ordering: major → minor → patch
 *
 * ## Common Compiler Versions
 * - **GCC**: 4.9+ (C++14), 7+ (C++17), 10+ (C++20)
 * - **Clang**: 3.5+ (C++14), 5+ (C++17), 10+ (C++20)
 * - **MSVC**: 19.0+ (VS2015), 19.1+ (VS2017), 19.2+ (VS2019)
 * - **Intel**: 18+ (C++14), 19+ (C++17), 2021+ (C++20)
 *
 * @example Version-dependent feature detection
 * @code
 * constexpr auto version = trlc::platform::getCompilerVersion();
 * constexpr auto compiler = trlc::platform::getCompilerType();
 * 
 * if constexpr (compiler == trlc::platform::CompilerType::gcc &&
 *               version >= trlc::platform::CompilerVersion{7, 0}) {
 *     // Use C++17 features available in GCC 7+
 *     if constexpr (std::has_include(<string_view>)) {
 *         // Use string_view
 *     }
 * }
 * @endcode
 *
 * @note Version comparison is constexpr and can be used in static_assert
 * and if constexpr expressions for compile-time feature selection.
 * @see getCompilerVersion() for version detection
 * @see CompilerInfo for comprehensive compiler information
 * @since 1.0.0
 */
struct CompilerVersion {
    int major;  ///< Major version number (e.g., 9 in GCC 9.3.0)
    int minor;  ///< Minor version number (e.g., 3 in GCC 9.3.0)
    int patch;  ///< Patch version number (e.g., 0 in GCC 9.3.0)

    /**
     * @brief Constructs a compiler version
     * 
     * @details Creates a CompilerVersion with the specified version components.
     * The patch version defaults to 0 if not specified, which is common
     * for version checks that only care about major.minor versions.
     * 
     * @param maj Major version number
     * @param min Minor version number  
     * @param pat Patch version number (defaults to 0)
     * 
     * @note This constructor is constexpr and can be used in compile-time contexts
     * @since 1.0.0
     */
    constexpr CompilerVersion(int maj, int min, int pat = 0) noexcept
        : major(maj), minor(min), patch(pat) {}

    /**
     * @brief Default constructor for unknown version
     * 
     * @details Creates a CompilerVersion with all components set to 0,
     * indicating an unknown or undetectable compiler version.
     * 
     * @note This constructor is constexpr and can be used in compile-time contexts
     * @since 1.0.0
     */
    constexpr CompilerVersion() noexcept : major(0), minor(0), patch(0) {}

    /**
     * @brief Greater than or equal comparison
     * 
     * @details Compares two compiler versions using lexicographic ordering.
     * Useful for checking if a compiler meets minimum version requirements
     * for specific language features or optimizations.
     * 
     * @param other Version to compare against
     * @return true if this version >= other (major.minor.patch ordering)
     * 
     * @note This operator is constexpr and can be used in compile-time contexts
     * @see operator<() for less-than comparison
     * @since 1.0.0
     * 
     * @example Version requirement checking
     * @code
     * constexpr auto version = trlc::platform::getCompilerVersion();
     * if constexpr (version >= trlc::platform::CompilerVersion{7, 0}) {
     *     // Use features available in version 7.0+
     * }
     * @endcode
     */
    constexpr bool operator>=(const CompilerVersion& other) const noexcept {
        if (major != other.major)
            return major > other.major;
        if (minor != other.minor)
            return minor > other.minor;
        return patch >= other.patch;
    }

    /**
     * @brief Less than comparison
     * 
     * @details Compares two compiler versions using lexicographic ordering.
     * Useful for checking if a compiler version is below a certain threshold
     * or for compatibility with older compiler versions.
     * 
     * @param other Version to compare against
     * @return true if this version < other (major.minor.patch ordering)
     * 
     * @note This operator is constexpr and can be used in compile-time contexts
     * @see operator>=() for greater-than-or-equal comparison
     * @since 1.0.0
     */
    constexpr bool operator<(const CompilerVersion& other) const noexcept {
        if (major != other.major)
            return major < other.major;
        if (minor != other.minor)
            return minor < other.minor;
        return patch < other.patch;
    }

    /**
     * @brief Equality comparison
     * 
     * @details Checks if two compiler versions are exactly equal in all
     * components (major, minor, patch). Useful for compiler-specific
     * workarounds or exact version matching.
     * 
     * @param other Version to compare against
     * @return true if all version components are equal
     * 
     * @note This operator is constexpr and can be used in compile-time contexts
     * @see operator!=() for inequality comparison
     * @since 1.0.0
     */
    constexpr bool operator==(const CompilerVersion& other) const noexcept {
        return major == other.major && minor == other.minor && patch == other.patch;
    }

    /**
     * @brief Inequality comparison
     * 
     * @details Checks if two compiler versions are not equal in any component.
     * This is the logical negation of operator==().
     * 
     * @param other Version to compare against
     * @return true if any version component differs
     * 
     * @note This operator is constexpr and can be used in compile-time contexts
     * @see operator==() for equality comparison
     * @since 1.0.0
     */
    constexpr bool operator!=(const CompilerVersion& other) const noexcept {
        return !(*this == other);
    }

    /**
     * @brief Greater than comparison
     * @param other Version to compare against
     * @return true if this version > other
     */
    constexpr bool operator>(const CompilerVersion& other) const noexcept {
        return !(*this < other) && !(*this == other);
    }

    /**
     * @brief Less than or equal comparison
     * @param other Version to compare against
     * @return true if this version <= other
     */
    constexpr bool operator<=(const CompilerVersion& other) const noexcept {
        return (*this < other) || (*this == other);
    }
};

/**
 * @brief Detects the current compiler type at compile time
 *
 * Uses compiler-specific predefined macros to identify the compiler.
 * Detection is performed entirely at compile time with zero runtime cost.
 *
 * @return CompilerType enum value representing the detected compiler
 * @note Returns CompilerType::unknown for unrecognized compilers
 */
constexpr CompilerType getCompilerType() noexcept {
// Check for Intel compilers first (they may also define GCC/Clang macros)
#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC)
    #if defined(__clang__)
    return CompilerType::intel_llvm;  // Intel C++ Compiler based on LLVM
    #else
    return CompilerType::intel_classic;  // Legacy Intel C++ Compiler
    #endif
// Check for Clang (must be before GCC since Clang may define __GNUC__)
#elif defined(__clang__)
    return CompilerType::clang;
// Check for GCC
#elif defined(__GNUC__)
    #if defined(__MINGW32__) || defined(__MINGW64__)
    return CompilerType::mingw;
    #else
    return CompilerType::gcc;
    #endif
// Check for MSVC
#elif defined(_MSC_VER)
    return CompilerType::msvc;
#else
    return CompilerType::unknown;
#endif
}

/**
 * @brief Gets the compiler version at compile time
 *
 * Extracts version information from compiler-specific macros.
 *
 * @return CompilerVersion structure with major, minor, and patch versions
 * @note Returns version (0, 0, 0) for unknown compilers
 */
constexpr CompilerVersion getCompilerVersion() noexcept {
#if defined(__INTEL_COMPILER)
    // Intel compiler version format: VVRR (VV = major, RR = minor)
    return CompilerVersion(
        __INTEL_COMPILER / 100, (__INTEL_COMPILER % 100) / 10, __INTEL_COMPILER % 10);
#elif defined(__clang__)
    return CompilerVersion(__clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(__GNUC__)
    #if defined(__GNUC_PATCHLEVEL__)
    return CompilerVersion(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    #else
    return CompilerVersion(__GNUC__, __GNUC_MINOR__, 0);
    #endif
#elif defined(_MSC_VER)
    // MSVC version mapping (simplified)
    #if _MSC_VER >= 1940    // Visual Studio 2022 17.10+
    return CompilerVersion(19, 4, (_MSC_VER - 1940));
    #elif _MSC_VER >= 1930  // Visual Studio 2022 17.0-17.9
    return CompilerVersion(19, 3, (_MSC_VER - 1930));
    #elif _MSC_VER >= 1920  // Visual Studio 2019
    return CompilerVersion(19, 2, (_MSC_VER - 1920));
    #elif _MSC_VER >= 1910  // Visual Studio 2017
    return CompilerVersion(19, 1, (_MSC_VER - 1910));
    #elif _MSC_VER >= 1900  // Visual Studio 2015
    return CompilerVersion(19, 0, (_MSC_VER - 1900));
    #else
    return CompilerVersion(_MSC_VER / 100, (_MSC_VER % 100) / 10, _MSC_VER % 10);
    #endif
#else
    return CompilerVersion(0, 0, 0);
#endif
}

/**
 * @brief Gets the compiler name as a string
 *
 * Returns a human-readable string representation of the compiler name.
 *
 * @return Compiler name as a C-style string
 * @note Returns "unknown" for unrecognized compilers
 */
constexpr const char* getCompilerName() noexcept {
    switch (getCompilerType()) {
        case CompilerType::gcc:
            return "gcc";
        case CompilerType::clang:
            return "clang";
        case CompilerType::msvc:
            return "msvc";
        case CompilerType::intel_classic:
            return "intel_classic";
        case CompilerType::intel_llvm:
            return "intel_llvm";
        case CompilerType::mingw:
            return "mingw";
        case CompilerType::unknown:
        default:
            return "unknown";
    }
}

/**
 * @brief Checks if compiler supports __builtin attributes
 *
 * Determines if the current compiler supports GCC-style __builtin functions
 * for the specified attribute.
 *
 * @param attribute_name Name of the attribute to check (unused in constexpr context)
 * @return true if compiler supports builtin attributes
 * @note This is a simplified check based on compiler type
 */
constexpr bool hasBuiltinAttribute([[maybe_unused]] const char* attribute_name) noexcept {
    constexpr auto compiler = getCompilerType();
    return compiler == CompilerType::gcc || compiler == CompilerType::clang ||
           compiler == CompilerType::mingw || compiler == CompilerType::intel_llvm;
}

/**
 * @brief Checks if compiler supports inline assembly
 *
 * Determines if the current compiler supports inline assembly syntax.
 *
 * @return true if inline assembly is supported
 */
constexpr bool supportsInlineAssembly() noexcept {
    constexpr auto compiler = getCompilerType();
    // MSVC has different inline assembly syntax and limitations
    return compiler == CompilerType::gcc || compiler == CompilerType::clang ||
           compiler == CompilerType::mingw || compiler == CompilerType::intel_classic ||
           compiler == CompilerType::intel_llvm;
}

/**
 * @brief Checks if compiler supports colored diagnostics
 *
 * Determines if the compiler can output colored error/warning messages.
 *
 * @return true if colored diagnostics are supported
 */
constexpr bool hasColorDiagnostics() noexcept {
    constexpr auto compiler = getCompilerType();
    return compiler == CompilerType::gcc || compiler == CompilerType::clang ||
           compiler == CompilerType::mingw || compiler == CompilerType::intel_llvm;
}

//==============================================================================
// Consolidated Compiler Information
//==============================================================================

/**
 * @brief Consolidated compiler information structure
 *
 * This structure aggregates all compiler detection results into a single
 * convenient structure for reporting and analysis purposes.
 */
struct CompilerInfo {
    CompilerType type;           ///< Detected compiler type
    CompilerVersion version;     ///< Compiler version information
    const char* name;            ///< Human-readable compiler name
    bool has_builtin_attribute;  ///< Supports __has_builtin attributes
    bool has_inline_assembly;    ///< Supports inline assembly
    bool has_color_diagnostics;  ///< Supports colored diagnostic output

    /**
     * @brief Check if compiler is at least the specified version
     * @param min_version Minimum version to check against
     * @return true if current compiler version is >= min_version
     */
    constexpr bool isAtLeast(const CompilerVersion& min_version) const noexcept {
        return version >= min_version;
    }

    /**
     * @brief Check if this is a GCC-compatible compiler
     * @return true if compiler is GCC or GCC-compatible
     */
    constexpr bool isGccCompatible() const noexcept {
        return type == CompilerType::gcc || type == CompilerType::mingw ||
               type == CompilerType::intel_classic;
    }

    /**
     * @brief Check if this is a Clang-compatible compiler
     * @return true if compiler is Clang or Clang-compatible
     */
    constexpr bool isClangCompatible() const noexcept {
        return type == CompilerType::clang || type == CompilerType::intel_llvm;
    }
};

/**
 * @brief Get consolidated compiler information
 *
 * Returns a CompilerInfo structure containing all detected compiler
 * information in a single convenient package.
 *
 * @return CompilerInfo structure with all detected information
 *
 * @note This function is constexpr and can be evaluated at compile time
 */
constexpr CompilerInfo getCompilerInfo() noexcept {
    return CompilerInfo{getCompilerType(),
                        getCompilerVersion(),
                        getCompilerName(),
                        hasBuiltinAttribute(""),  // Generic builtin support check
                        supportsInlineAssembly(),
                        hasColorDiagnostics()};
}

}  // namespace platform
}  // namespace trlc

// Convenience macros for conditional compilation

/**
 * @brief Check if current compiler is GCC (preprocessor-compatible)
 */
#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC)
    #define TRLC_COMPILER_GCC 0
#elif defined(__clang__)
    #define TRLC_COMPILER_GCC 0
#elif defined(__GNUC__)
    #if defined(__MINGW32__) || defined(__MINGW64__)
        #define TRLC_COMPILER_GCC 0  // MinGW is separate
    #else
        #define TRLC_COMPILER_GCC 1
    #endif
#else
    #define TRLC_COMPILER_GCC 0
#endif

/**
 * @brief Check if current compiler is Clang (preprocessor-compatible)
 */
#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC)
    #if defined(__clang__)
        #define TRLC_COMPILER_CLANG 0  // Intel LLVM-based, not pure Clang
    #else
        #define TRLC_COMPILER_CLANG 0
    #endif
#elif defined(__clang__)
    #define TRLC_COMPILER_CLANG 1
#else
    #define TRLC_COMPILER_CLANG 0
#endif

/**
 * @brief Check if current compiler is MSVC (preprocessor-compatible)
 */
#if defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
    #define TRLC_COMPILER_MSVC 1
#else
    #define TRLC_COMPILER_MSVC 0
#endif

/**
 * @brief Check if current compiler is Intel Classic (preprocessor-compatible)
 */
#if defined(__INTEL_COMPILER) && !defined(__clang__)
    #define TRLC_COMPILER_INTEL 1
#else
    #define TRLC_COMPILER_INTEL 0
#endif

/**
 * @brief Check if current compiler is Intel LLVM-based (preprocessor-compatible)
 */
#if (defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC)) && defined(__clang__)
    #define TRLC_COMPILER_INTEL_LLVM 1
#else
    #define TRLC_COMPILER_INTEL_LLVM 0
#endif

/**
 * @brief Check if current compiler is MinGW (preprocessor-compatible)
 */
#if defined(__GNUC__) && (defined(__MINGW32__) || defined(__MINGW64__))
    #define TRLC_COMPILER_MINGW 1
#else
    #define TRLC_COMPILER_MINGW 0
#endif

/**
 * @brief Get compiler version components as preprocessor macros
 */
#if defined(__INTEL_COMPILER)
    #define TRLC_COMPILER_VERSION_MAJOR (__INTEL_COMPILER / 100)
    #define TRLC_COMPILER_VERSION_MINOR ((__INTEL_COMPILER % 100) / 10)
    #define TRLC_COMPILER_VERSION_PATCH (__INTEL_COMPILER % 10)
#elif defined(__clang__)
    #define TRLC_COMPILER_VERSION_MAJOR __clang_major__
    #define TRLC_COMPILER_VERSION_MINOR __clang_minor__
    #define TRLC_COMPILER_VERSION_PATCH __clang_patchlevel__
#elif defined(__GNUC__)
    #define TRLC_COMPILER_VERSION_MAJOR __GNUC__
    #define TRLC_COMPILER_VERSION_MINOR __GNUC_MINOR__
    #ifdef __GNUC_PATCHLEVEL__
        #define TRLC_COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__
    #else
        #define TRLC_COMPILER_VERSION_PATCH 0
    #endif
#elif defined(_MSC_VER)
    #if _MSC_VER >= 1940
        #define TRLC_COMPILER_VERSION_MAJOR 19
        #define TRLC_COMPILER_VERSION_MINOR 4
        #define TRLC_COMPILER_VERSION_PATCH (_MSC_VER - 1940)
    #elif _MSC_VER >= 1930
        #define TRLC_COMPILER_VERSION_MAJOR 19
        #define TRLC_COMPILER_VERSION_MINOR 3
        #define TRLC_COMPILER_VERSION_PATCH (_MSC_VER - 1930)
    #elif _MSC_VER >= 1920
        #define TRLC_COMPILER_VERSION_MAJOR 19
        #define TRLC_COMPILER_VERSION_MINOR 2
        #define TRLC_COMPILER_VERSION_PATCH (_MSC_VER - 1920)
    #elif _MSC_VER >= 1910
        #define TRLC_COMPILER_VERSION_MAJOR 19
        #define TRLC_COMPILER_VERSION_MINOR 1
        #define TRLC_COMPILER_VERSION_PATCH (_MSC_VER - 1910)
    #elif _MSC_VER >= 1900
        #define TRLC_COMPILER_VERSION_MAJOR 19
        #define TRLC_COMPILER_VERSION_MINOR 0
        #define TRLC_COMPILER_VERSION_PATCH (_MSC_VER - 1900)
    #else
        #define TRLC_COMPILER_VERSION_MAJOR (_MSC_VER / 100)
        #define TRLC_COMPILER_VERSION_MINOR ((_MSC_VER % 100) / 10)
        #define TRLC_COMPILER_VERSION_PATCH (_MSC_VER % 10)
    #endif
#else
    #define TRLC_COMPILER_VERSION_MAJOR 0
    #define TRLC_COMPILER_VERSION_MINOR 0
    #define TRLC_COMPILER_VERSION_PATCH 0
#endif

/**
 * @brief Check if compiler version is at least the specified version (preprocessor-compatible)
 * @param major Minimum major version
 * @param minor Minimum minor version
 * @param patch Minimum patch version
 */
#define TRLC_COMPILER_VERSION_ATLEAST(major, minor, patch)                                \
    ((TRLC_COMPILER_VERSION_MAJOR > (major)) ||                                           \
     (TRLC_COMPILER_VERSION_MAJOR == (major) && TRLC_COMPILER_VERSION_MINOR > (minor)) || \
     (TRLC_COMPILER_VERSION_MAJOR == (major) && TRLC_COMPILER_VERSION_MINOR == (minor) && \
      TRLC_COMPILER_VERSION_PATCH >= (patch)))

/**
 * @brief Get current compiler version as a CompilerVersion object (constexpr function)
 */
#define TRLC_COMPILER_VERSION (trlc::platform::getCompilerVersion())

/**
 * @brief Get current compiler name as a string (constexpr function)
 */
#define TRLC_COMPILER_NAME (trlc::platform::getCompilerName())

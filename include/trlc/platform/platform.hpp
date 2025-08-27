#pragma once

/**
 * @file platform.hpp
 * @brief Operating system and environment detection utilities
 *
 * This header provides compile-time platform detection for operating systems,
 * environments, and platform-specific feature detection for cross-platform C++ development.
 *
 * @copyright Copyright (c) 2025 TRLC Platform
 */

namespace trlc {
namespace platform {

/**
 * @brief Operating system identification enumeration
 *
 * Represents the detected operating system type at compile time.
 */
enum class OperatingSystem : int {
    unknown = 0,    ///< Unknown or unsupported operating system
    windows,        ///< Microsoft Windows (all versions)
    linux_generic,  ///< Generic Linux distribution
    ubuntu,         ///< Ubuntu Linux distribution
    debian,         ///< Debian Linux distribution
    redhat,         ///< Red Hat Enterprise Linux / CentOS / Fedora
    macos,          ///< Apple macOS / Mac OS X
    freebsd,        ///< FreeBSD operating system
    openbsd,        ///< OpenBSD operating system
    netbsd,         ///< NetBSD operating system
    android,        ///< Android operating system
    ios             ///< Apple iOS operating system
};

/**
 * @brief Environment type identification enumeration
 *
 * Represents the target environment type for the application.
 */
enum class EnvironmentType : int {
    unknown = 0,  ///< Unknown environment type
    desktop,      ///< Desktop/workstation environment
    server,       ///< Server environment
    embedded,     ///< Embedded system environment
    mobile        ///< Mobile device environment
};

/**
 * @brief Platform information structure
 *
 * Contains comprehensive information about the detected platform,
 * including operating system, environment, and kernel family.
 */
struct PlatformInfo {
    OperatingSystem os;           ///< Detected operating system
    EnvironmentType environment;  ///< Detected environment type
    const char* os_name;          ///< Human-readable OS name
    const char* kernel_family;    ///< Kernel family: "nt", "posix", "unix"

    /**
     * @brief Checks if the platform supports POSIX APIs
     * @return true if platform is POSIX-compliant
     */
    constexpr bool isPosix() const noexcept {
        return (kernel_family[0] == 'p' && kernel_family[1] == 'o') ||  // "posix"
               (kernel_family[0] == 'u' && kernel_family[1] == 'n');    // "unix"
    }

    /**
     * @brief Checks if the platform is Unix-like
     * @return true if platform is Unix-based
     */
    constexpr bool isUnix() const noexcept {
        return os == OperatingSystem::linux_generic || os == OperatingSystem::ubuntu ||
               os == OperatingSystem::debian || os == OperatingSystem::redhat ||
               os == OperatingSystem::macos || os == OperatingSystem::freebsd ||
               os == OperatingSystem::openbsd || os == OperatingSystem::netbsd ||
               os == OperatingSystem::android;
    }

    /**
     * @brief Checks if the platform is Windows-based
     * @return true if platform is Windows
     */
    constexpr bool isWindows() const noexcept { return os == OperatingSystem::windows; }
};

/**
 * @brief Detects the current operating system at compile time
 *
 * Uses platform-specific predefined macros to identify the operating system.
 * Detection is performed entirely at compile time with zero runtime cost.
 *
 * @return OperatingSystem enum value representing the detected OS
 * @note Returns OperatingSystem::unknown for unrecognized platforms
 */
constexpr OperatingSystem getOperatingSystem() noexcept {
// Check for Android first (it defines __linux__ but is not generic Linux)
#if defined(__ANDROID__)
    return OperatingSystem::android;
// Check for iOS
#elif defined(__APPLE__) && defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
    return OperatingSystem::ios;
// Check for macOS
#elif defined(__APPLE__) && defined(__MACH__)
    return OperatingSystem::macos;
// Check for Windows
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    return OperatingSystem::windows;
// Check for specific BSD variants
#elif defined(__FreeBSD__)
    return OperatingSystem::freebsd;
#elif defined(__OpenBSD__)
    return OperatingSystem::openbsd;
#elif defined(__NetBSD__)
    return OperatingSystem::netbsd;
// Check for Linux distributions
#elif defined(__linux__) || defined(__gnu_linux__)
    // For now, return generic Linux
    // In a real implementation, we might read /etc/os-release at runtime
    // or use additional compile-time detection methods
    return OperatingSystem::linux_generic;
#else
    return OperatingSystem::unknown;
#endif
}

/**
 * @brief Detects the current environment type at compile time
 *
 * Determines the target environment based on platform characteristics
 * and available compile-time information.
 *
 * @return EnvironmentType enum value representing the detected environment
 * @note Detection is based on platform type and common patterns
 */
constexpr EnvironmentType getEnvironmentType() noexcept {
    constexpr auto os = getOperatingSystem();

    // Mobile platforms
    if (os == OperatingSystem::android || os == OperatingSystem::ios) {
        return EnvironmentType::mobile;
    }

    // Desktop platforms
    if (os == OperatingSystem::windows || os == OperatingSystem::macos) {
        return EnvironmentType::desktop;
    }

    // Unix-like systems - typically server environments
    if (os == OperatingSystem::linux_generic || os == OperatingSystem::ubuntu ||
        os == OperatingSystem::debian || os == OperatingSystem::redhat ||
        os == OperatingSystem::freebsd || os == OperatingSystem::openbsd ||
        os == OperatingSystem::netbsd) {
        // Default to server for Unix-like systems
        // In practice, this might be refined based on additional detection
        return EnvironmentType::server;
    }

    return EnvironmentType::unknown;
}

/**
 * @brief Gets comprehensive platform information
 *
 * Returns a complete PlatformInfo structure with operating system,
 * environment type, human-readable names, and kernel family.
 *
 * @return PlatformInfo structure with all detected platform details
 */
constexpr PlatformInfo getPlatformInfo() noexcept {
    constexpr auto os = getOperatingSystem();
    constexpr auto env = getEnvironmentType();

    // Determine OS name and kernel family based on detected OS
    const char* os_name = nullptr;
    const char* kernel_family = nullptr;

    switch (os) {
        case OperatingSystem::windows:
            os_name = "Windows";
            kernel_family = "nt";
            break;
        case OperatingSystem::linux_generic:
            os_name = "Linux";
            kernel_family = "posix";
            break;
        case OperatingSystem::ubuntu:
            os_name = "Ubuntu";
            kernel_family = "posix";
            break;
        case OperatingSystem::debian:
            os_name = "Debian";
            kernel_family = "posix";
            break;
        case OperatingSystem::redhat:
            os_name = "Red Hat";
            kernel_family = "posix";
            break;
        case OperatingSystem::macos:
            os_name = "macOS";
            kernel_family = "unix";
            break;
        case OperatingSystem::freebsd:
            os_name = "FreeBSD";
            kernel_family = "unix";
            break;
        case OperatingSystem::openbsd:
            os_name = "OpenBSD";
            kernel_family = "unix";
            break;
        case OperatingSystem::netbsd:
            os_name = "NetBSD";
            kernel_family = "unix";
            break;
        case OperatingSystem::android:
            os_name = "Android";
            kernel_family = "posix";
            break;
        case OperatingSystem::ios:
            os_name = "iOS";
            kernel_family = "unix";
            break;
        case OperatingSystem::unknown:
        default:
            os_name = "Unknown";
            kernel_family = "unknown";
            break;
    }

    return PlatformInfo{os, env, os_name, kernel_family};
}

/**
 * @brief Checks if the platform supports POSIX APIs
 *
 * Determines if the current platform provides POSIX-compliant APIs
 * based on platform detection and standard macros.
 *
 * @return true if POSIX APIs are available
 */
constexpr bool hasPosixApi() noexcept {
// Check for POSIX version macro
#if defined(_POSIX_VERSION)
    return true;
// Check for common Unix/POSIX indicators
#elif defined(__unix__) || defined(__unix) || defined(unix)
    return true;
// Platform-specific checks
#elif defined(__linux__) || defined(__gnu_linux__)
    return true;
#elif defined(__APPLE__) && defined(__MACH__)
    return true;
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
    return true;
#elif defined(__ANDROID__)
    return true;
#else
    return false;
#endif
}

/**
 * @brief Checks if the platform supports Win32 APIs
 *
 * Determines if the current platform provides Win32 APIs
 * based on Windows-specific macros.
 *
 * @return true if Win32 APIs are available
 */
constexpr bool hasWin32Api() noexcept {
#if defined(_WIN32) || defined(_WIN64)
    return true;
#elif defined(__CYGWIN__)
    return true;  // Cygwin provides Win32 API access
#else
    return false;
#endif
}

/**
 * @brief Checks if the platform supports case-sensitive filesystems
 *
 * Determines if the filesystem is case-sensitive based on platform
 * characteristics and common filesystem behavior.
 *
 * @return true if filesystem is typically case-sensitive
 * @note This is a compile-time approximation; actual filesystem behavior may vary
 */
constexpr bool supportsCaseSensitiveFilesystem() noexcept {
    constexpr auto os = getOperatingSystem();

    // Unix-like systems typically have case-sensitive filesystems
    if (os == OperatingSystem::linux_generic || os == OperatingSystem::ubuntu ||
        os == OperatingSystem::debian || os == OperatingSystem::redhat ||
        os == OperatingSystem::freebsd || os == OperatingSystem::openbsd ||
        os == OperatingSystem::netbsd || os == OperatingSystem::android) {
        return true;
    }

    // Windows and macOS typically have case-insensitive filesystems by default
    // (though they can be configured to be case-sensitive)
    if (os == OperatingSystem::windows || os == OperatingSystem::macos ||
        os == OperatingSystem::ios) {
        return false;
    }

    // Default to case-sensitive for unknown platforms
    return true;
}

}  // namespace platform
}  // namespace trlc

// Convenience macros for conditional compilation

/**
 * @brief Check if current platform is Windows (preprocessor-compatible)
 */
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    #define TRLC_PLATFORM_WINDOWS 1
#else
    #define TRLC_PLATFORM_WINDOWS 0
#endif

/**
 * @brief Check if current platform is Linux (preprocessor-compatible)
 */
#if defined(__linux__) || defined(__gnu_linux__)
    #define TRLC_PLATFORM_LINUX 1
#else
    #define TRLC_PLATFORM_LINUX 0
#endif

/**
 * @brief Check if current platform is macOS (preprocessor-compatible)
 */
#if defined(__APPLE__) && defined(__MACH__) && !defined(TARGET_OS_IPHONE)
    #define TRLC_PLATFORM_MACOS 1
#else
    #define TRLC_PLATFORM_MACOS 0
#endif

/**
 * @brief Check if current platform supports POSIX (preprocessor-compatible)
 */
#if defined(_POSIX_VERSION) || defined(__unix__) || defined(__unix) || defined(unix) ||          \
    defined(__linux__) || defined(__gnu_linux__) || (defined(__APPLE__) && defined(__MACH__)) || \
    defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__ANDROID__)
    #define TRLC_PLATFORM_POSIX 1
#else
    #define TRLC_PLATFORM_POSIX 0
#endif

/**
 * @brief Check if current platform is BSD-based (preprocessor-compatible)
 */
#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || \
    (defined(__APPLE__) && defined(__MACH__))
    #define TRLC_PLATFORM_BSD 1
#else
    #define TRLC_PLATFORM_BSD 0
#endif

/**
 * @brief Check if current platform is Unix-like (preprocessor-compatible)
 */
#if TRLC_PLATFORM_LINUX || TRLC_PLATFORM_MACOS || TRLC_PLATFORM_BSD || defined(__ANDROID__)
    #define TRLC_PLATFORM_UNIX 1
#else
    #define TRLC_PLATFORM_UNIX 0
#endif

/**
 * @brief Check if current platform is mobile (preprocessor-compatible)
 */
#if defined(__ANDROID__) || (defined(__APPLE__) && defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE)
    #define TRLC_PLATFORM_MOBILE 1
#else
    #define TRLC_PLATFORM_MOBILE 0
#endif

/**
 * @brief Check if current platform has case-sensitive filesystem (preprocessor-compatible)
 */
#if TRLC_PLATFORM_UNIX && !TRLC_PLATFORM_MACOS
    #define TRLC_PLATFORM_CASE_SENSITIVE_FS 1
#else
    #define TRLC_PLATFORM_CASE_SENSITIVE_FS 0
#endif

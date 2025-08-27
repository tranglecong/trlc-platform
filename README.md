# TRLC Platform Library v1.0.0

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/tranglecong/platform/actions)
[![C++ Version](https://img.shields.io/badge/C%2B%2B-17%2F20%2F23-blue)](#requirements)
[![License](https://img.shields.io/badge/license-MIT-green)](https://github.com/tranglecong/platform/blob/main/LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)](#platform-support)
[![Header Only](https://img.shields.io/badge/header--only-yes-success)](#installation)
[![CMake](https://img.shields.io/badge/CMake-3.15+-blue)](https://cmake.org/)

A modern, header-only C++ library for compile-time platform detection and abstraction. TRLC Platform provides zero-overhead platform detection, feature-based conditional compilation, and adaptive code selection for cross-platform C++ development.

> **🚀 Replace traditional `#ifdef` macros with modern, type-safe, compile-time API calls**

## Why TRLC Platform?

Traditional platform detection relies on fragile preprocessor macros:

```cpp
// ❌ Traditional approach - fragile and error-prone
#if defined(__GNUC__) || defined(__clang__)
    result = __builtin_popcount(value);
#elif defined(_MSC_VER)
    result = __popcnt(value);
#else
    // Fallback implementation
#endif
```

TRLC Platform provides a modern, type-safe alternative:

```cpp
// ✅ TRLC Platform approach - type-safe and intuitive
if constexpr (trlc::platform::getCompilerType() == trlc::platform::CompilerType::gcc ||
              trlc::platform::getCompilerType() == trlc::platform::CompilerType::clang) {
    result = __builtin_popcount(value);
} else if constexpr (trlc::platform::getCompilerType() == trlc::platform::CompilerType::msvc) {
    result = __popcnt(value);
}
```

## Table of Contents

- [Why TRLC Platform?](#why-trlc-platform)
- [Features](#features)
- [Quick Start](#quick-start)
- [Platform Support](#platform-support)
- [Requirements](#requirements)
- [Installation](#installation)
  - [Header-Only Installation](#header-only-installation)
  - [CMake Integration](#cmake-integration)
  - [Package Managers](#package-managers)
- [Usage Examples](#usage-examples)
  - [Basic Platform Detection](#basic-platform-detection)
  - [Advanced Feature Detection](#advanced-feature-detection)
  - [Integration Patterns](#integration-patterns)
- [API Reference](#api-reference)
- [Performance](#performance)
- [Building and Testing](#building-and-testing)
- [Troubleshooting](#troubleshooting)
- [Examples](#examples)
- [Contributing](#contributing)
- [License](#license)

## Features

### ✨ Core Capabilities

- **🔍 Comprehensive Platform Detection**
  - Operating system detection (Windows, Linux, macOS, BSD variants)
  - Compiler detection (GCC, Clang, MSVC, Intel)
  - Architecture detection (x86, x64, ARM, ARM64, MIPS, PowerPC)
  - C++ standard detection (C++17/20/23)

- **⚡ Zero Runtime Overhead**
  - Compile-time detection with `constexpr` functions
  - Template-based conditional compilation
  - Dead code elimination for unused platform paths
  - Assembly output verification of optimizations

- **🧬 Advanced Template Metaprogramming**
  - SFINAE-based conditional template instantiation
  - Variadic template functions for multi-feature requirements
  - Type traits for platform-specific behavior
  - Template specializations for optimal code generation

- **🛠️ Feature Detection**
  - Language features (exceptions, RTTI, threads, atomic operations)
  - Runtime CPU features (SSE, AVX, NEON, hardware acceleration)
  - Compiler features (intrinsics, attributes, diagnostics)
  - Build configuration detection (debug/release, sanitizers)

- **📦 Header-Only Design**
  - No external dependencies beyond standard library
  - Easy integration into any C++ project
  - CMake integration with feature detection
  - Cross-compiler compatibility

### 🎯 Adaptive Programming Support

- **Conditional Compilation**: Write code that adapts automatically to target platform
- **Feature-Based Selection**: Choose algorithms based on available hardware/software features
- **Compiler Optimization**: Leverage compiler-specific intrinsics and optimizations
- **Memory Management**: Platform-optimized alignment and allocation strategies
- **Error Handling**: Adaptive error handling based on available features

## Quick Start

### 1. Include the Library

```cpp
#include "trlc/platform/core.hpp"

int main() {
    // Get comprehensive platform information
    auto platform = trlc::platform::getPlatformReport();
    std::cout << platform.getBriefSummary() << std::endl;
    // Output: "gcc 11.4 on Linux x86_64 (64-bit)"
    
    return 0;
}
```

### 2. Compile-Time Platform Detection

```cpp
#include "trlc/platform/core.hpp"

void optimizedFunction() {
    // All detection happens at compile time
    constexpr auto os = trlc::platform::getOperatingSystem();
    constexpr auto compiler = trlc::platform::getCompilerType();
    constexpr auto arch = trlc::platform::getCpuArchitecture();
    
    if constexpr (os == trlc::platform::OperatingSystem::windows) {
        // Windows-specific implementation
        useWindowsAPI();
    } else if constexpr (os == trlc::platform::OperatingSystem::linux_generic) {
        // Linux-specific implementation
        useLinuxAPI();
    }
    
    if constexpr (arch == trlc::platform::CpuArchitecture::x86_64) {
        // Use x86_64 optimizations
        useSimdInstructions();
    }
}
```

### 3. Feature-Based Conditional Compilation

```cpp
#include "trlc/platform/core.hpp"

template <typename T>
auto safeOperation(T&& operation) {
    if constexpr (trlc::platform::hasFeature<trlc::platform::LanguageFeature::exceptions>()) {
        try {
            return operation();
        } catch (...) {
            return handleError();
        }
    } else {
        // No exception support - use error codes
        return operation(); // Assume operation handles errors internally
    }
}
```

## Platform Support

### Operating Systems

| Platform | Status | Variants Detected |
|----------|--------|-------------------|
| **Windows** | ✅ Full | All versions, MinGW |
| **Linux** | ✅ Full | Ubuntu, Debian, Red Hat, Generic |
| **macOS** | ✅ Full | All versions |
| **FreeBSD** | ✅ Full | All versions |
| **OpenBSD** | ✅ Full | All versions |
| **NetBSD** | ✅ Full | All versions |
| **Android** | ✅ Basic | NDK support |
| **iOS** | ✅ Basic | Xcode support |

### Compilers

| Compiler | Status | Versions Supported |
|----------|--------|--------------------|
| **GCC** | ✅ Full | 7.0+ |
| **Clang** | ✅ Full | 6.0+ |
| **MSVC** | ✅ Full | 2017+ |
| **Intel C++** | ✅ Basic | 19.0+ |
| **MinGW** | ✅ Full | GCC-based |

### Architectures

| Architecture | Status | Detection Level |
|--------------|--------|----------------|
| **x86** | ✅ Full | 32-bit |
| **x86_64** | ✅ Full | 64-bit, SIMD features |
| **ARM** | ✅ Full | ARMv7+ |
| **ARM64** | ✅ Full | ARMv8+, NEON |
| **MIPS** | ✅ Basic | 32/64-bit |
| **PowerPC** | ✅ Basic | 32/64-bit |

## Requirements

### Minimum Requirements
- **C++ Standard**: C++17 or later
- **Compiler**: GCC 7+, Clang 6+, MSVC 2017+, Intel C++ 19+
- **Build System**: CMake 3.15+ (optional, for testing)

### Recommended
- **C++ Standard**: C++20 for enhanced template features
- **Compiler**: Latest stable version for best optimization
- **Build Type**: Release mode for zero-overhead performance

## Installation

### Header-Only Installation

1. **Download/Clone**: Get the library source code
2. **Include Path**: Add `include/` directory to your compiler's include path
3. **Include Header**: Add `#include "trlc/platform/core.hpp"` to your code

```bash
git clone https://github.com/tranglecong/platform.git
cd platform
# Add include/ to your project's include path
```

### CMake Integration

#### As a Subproject

```cmake
# Add to your CMakeLists.txt
add_subdirectory(third_party/trlc-platform)
target_link_libraries(your_target PRIVATE trlc::platform)
```

#### Using FetchContent (Recommended)

```cmake
cmake_minimum_required(VERSION 3.15)

include(FetchContent)
FetchContent_Declare(
    trlc-platform
    GIT_REPOSITORY https://github.com/yourusername/trlc-flatform.git
    GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(trlc-platform)

# Link to your target
target_link_libraries(your_target PRIVATE trlc::platform)

# Optional: Enable debug utilities in Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(your_target PRIVATE TRLC_PLATFORM_ENABLE_DEBUG_UTILS=1)
endif()
```

#### As an Installed Package

```cmake
# Install the library first
find_package(trlc-platform REQUIRED)
target_link_libraries(your_target PRIVATE trlc::platform)
```

#### Advanced CMake Configuration

```cmake
# Configure build options
set(TRLC_PLATFORM_ENABLE_ASSERTS ON CACHE BOOL "Enable assertions")
set(TRLC_PLATFORM_BUILD_TESTS OFF CACHE BOOL "Skip building tests")

# Use specific C++ standard
target_compile_features(your_target PRIVATE cxx_std_20)

# Platform-specific optimizations
if(TRLC_PLATFORM_LINUX)
    target_compile_options(your_target PRIVATE -march=native)
endif()
```

### Package Managers

```bash
# vcpkg
vcpkg install trlc-platform

# Conan
conan install trlc-platform/1.0.0@

# CPM
CPMAddPackage("gh:trlc/platform@1.0.0")
```

## Usage Examples

### Basic Platform Detection

#### Simple Platform Check

```cpp
#include "trlc/platform/core.hpp"

void platformSpecificFunction() {
    constexpr auto os = trlc::platform::getOperatingSystem();
    
    if constexpr (os == trlc::platform::OperatingSystem::windows) {
        // Windows-specific code
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
    } else if constexpr (os == trlc::platform::OperatingSystem::linux_generic) {
        // Linux-specific code
        nice(-10);  // Increase priority
    } else if constexpr (os == trlc::platform::OperatingSystem::macos) {
        // macOS-specific code
        pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
    }
}
```

#### Compiler-Specific Optimizations

```cpp
#include "trlc/platform/core.hpp"

template<typename T>
int countBits(T value) {
    constexpr auto compiler = trlc::platform::getCompilerType();
    
    if constexpr (compiler == trlc::platform::CompilerType::gcc ||
                  compiler == trlc::platform::CompilerType::clang) {
        if constexpr (sizeof(T) == sizeof(unsigned int)) {
            return __builtin_popcount(static_cast<unsigned int>(value));
        } else if constexpr (sizeof(T) == sizeof(unsigned long)) {
            return __builtin_popcountl(static_cast<unsigned long>(value));
        } else {
            return __builtin_popcountll(static_cast<unsigned long long>(value));
        }
    } else if constexpr (compiler == trlc::platform::CompilerType::msvc) {
        // MSVC intrinsics
        #include <intrin.h>
        if constexpr (sizeof(T) == sizeof(unsigned int)) {
            return __popcnt(static_cast<unsigned int>(value));
        } else {
            return __popcnt64(static_cast<unsigned __int64>(value));
        }
    } else {
        // Portable fallback
        int count = 0;
        while (value) {
            count += value & 1;
            value >>= 1;
        }
        return count;
    }
}
```

### Advanced Feature Detection

#### Multi-Feature Requirements

```cpp
#include "trlc/platform/core.hpp"

template<typename Container>
void parallelSort(Container& container) {
    using namespace trlc::platform;
    
    // Check if we have threading and atomic operations
    if constexpr (hasFeature<LanguageFeature::threads>() && 
                  hasFeature<LanguageFeature::atomic_operations>()) {
        
        constexpr auto arch = getCpuArchitecture();
        
        // Architecture-specific thread optimization
        size_t thread_count;
        if constexpr (arch == CpuArchitecture::x86_64) {
            // x86_64 typically benefits from more threads
            thread_count = std::thread::hardware_concurrency();
        } else {
            // ARM/other architectures may benefit from fewer threads
            thread_count = std::max(2u, std::thread::hardware_concurrency() / 2);
        }
        
        // Use parallel sort with optimal thread count
        std::sort(std::execution::par, container.begin(), container.end());
    } else {
        // Fallback to sequential sort
        std::sort(container.begin(), container.end());
    }
}
```

#### SIMD-Optimized Operations

```cpp
#include "trlc/platform/core.hpp"

void vectorMultiply(const float* a, const float* b, float* result, size_t count) {
    using namespace trlc::platform;
    
    // Runtime feature detection for SIMD
    if (hasRuntimeFeature(RuntimeFeature::avx)) {
        // AVX implementation (8 floats at once)
        #include <immintrin.h>
        const size_t simd_count = count / 8;
        for (size_t i = 0; i < simd_count; ++i) {
            __m256 va = _mm256_load_ps(&a[i * 8]);
            __m256 vb = _mm256_load_ps(&b[i * 8]);
            __m256 vr = _mm256_mul_ps(va, vb);
            _mm256_store_ps(&result[i * 8], vr);
        }
        // Handle remaining elements
        for (size_t i = simd_count * 8; i < count; ++i) {
            result[i] = a[i] * b[i];
        }
    } else if (hasRuntimeFeature(RuntimeFeature::sse)) {
        // SSE implementation (4 floats at once)
        #include <xmmintrin.h>
        const size_t simd_count = count / 4;
        for (size_t i = 0; i < simd_count; ++i) {
            __m128 va = _mm_load_ps(&a[i * 4]);
            __m128 vb = _mm_load_ps(&b[i * 4]);
            __m128 vr = _mm_mul_ps(va, vb);
            _mm_store_ps(&result[i * 4], vr);
        }
        // Handle remaining elements
        for (size_t i = simd_count * 4; i < count; ++i) {
            result[i] = a[i] * b[i];
        }
    } else {
        // Scalar fallback
        for (size_t i = 0; i < count; ++i) {
            result[i] = a[i] * b[i];
        }
    }
}
```

### Integration Patterns

#### Library Abstraction Layer

```cpp
// platform_layer.hpp
#pragma once
#include "trlc/platform/core.hpp"

namespace mylib {

class PlatformLayer {
public:
    // High-precision timer
    static double getCurrentTime() {
        constexpr auto os = trlc::platform::getOperatingSystem();
        
        if constexpr (os == trlc::platform::OperatingSystem::windows) {
            LARGE_INTEGER freq, counter;
            QueryPerformanceFrequency(&freq);
            QueryPerformanceCounter(&counter);
            return static_cast<double>(counter.QuadPart) / freq.QuadPart;
        } else {
            // POSIX implementation
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC, &ts);
            return ts.tv_sec + ts.tv_nsec * 1e-9;
        }
    }
    
    // Memory alignment
    template<size_t Alignment>
    static void* alignedAlloc(size_t size) {
        constexpr auto os = trlc::platform::getOperatingSystem();
        
        if constexpr (os == trlc::platform::OperatingSystem::windows) {
            return _aligned_malloc(size, Alignment);
        } else {
            return aligned_alloc(Alignment, size);
        }
    }
    
    template<size_t Alignment>
    static void alignedFree(void* ptr) {
        constexpr auto os = trlc::platform::getOperatingSystem();
        
        if constexpr (os == trlc::platform::OperatingSystem::windows) {
            _aligned_free(ptr);
        } else {
            free(ptr);
        }
    }
};

} // namespace mylib
```

#### Template Metaprogramming Integration

```cpp
#include "trlc/platform/core.hpp"

namespace mylib {

// SFINAE helper for platform-specific template specializations
template<trlc::platform::OperatingSystem OS>
using enable_if_platform_t = std::enable_if_t<
    trlc::platform::getOperatingSystem() == OS, int>;

// Platform-specific implementations
template<typename T, enable_if_platform_t<trlc::platform::OperatingSystem::windows> = 0>
class FileHandler {
    // Windows-specific implementation using Win32 API
    HANDLE file_handle_;
public:
    bool open(const std::string& path) {
        file_handle_ = CreateFileA(path.c_str(), GENERIC_READ, 
                                   FILE_SHARE_READ, nullptr, 
                                   OPEN_EXISTING, 0, nullptr);
        return file_handle_ != INVALID_HANDLE_VALUE;
    }
};

template<typename T, enable_if_platform_t<trlc::platform::OperatingSystem::linux_generic> = 0>
class FileHandler {
    // Linux-specific implementation using POSIX API
    int file_descriptor_;
public:
    bool open(const std::string& path) {
        file_descriptor_ = ::open(path.c_str(), O_RDONLY);
        return file_descriptor_ != -1;
    }
};

} // namespace mylib
```

## API Reference

### Core Detection Functions

The main API is located in [`include/trlc/platform/core.hpp`](include/trlc/platform/core.hpp):

```cpp
namespace trlc::platform {
    
    //==============================================================================
    // Platform Detection (compile-time)
    //==============================================================================
    
    /// Get the current operating system
    constexpr OperatingSystem getOperatingSystem() noexcept;
    
    /// Get the current compiler type
    constexpr CompilerType getCompilerType() noexcept;
    
    /// Get the CPU architecture
    constexpr CpuArchitecture getCpuArchitecture() noexcept;
    
    /// Get the C++ standard version
    constexpr CppStandard getCppStandard() noexcept;
    
    /// Get pointer size in bits (32 or 64)
    constexpr int getPointerSize() noexcept;
    
    /// Check if system is little endian
    constexpr bool isLittleEndian() noexcept;
    
    //==============================================================================
    // Feature Detection
    //==============================================================================
    
    /// Check for language features (compile-time)
    template <LanguageFeature TFeature>
    constexpr bool hasFeature() noexcept;
    
    /// Check for runtime CPU features (runtime)
    bool hasRuntimeFeature(RuntimeFeature feature) noexcept;
    
    /// Check for multiple language features (in traits namespace)
    // Note: These functions are in trlc::platform::traits namespace
    // Use: trlc::platform::traits::hasAllFeatures<LanguageFeature::exceptions, LanguageFeature::threads>()
    // Use: trlc::platform::traits::hasAnyFeature<LanguageFeature::exceptions, LanguageFeature::rtti>()
    
    //==============================================================================
    // Platform Information Structures
    //==============================================================================
    
    /// Comprehensive platform information
    PlatformReport getPlatformReport() noexcept;
    
    /// Brief platform summary string
    std::string getBriefPlatformSummary();
    
    /// Print detailed platform report
    void printPlatformReport();
    
    //==============================================================================
    // Runtime Initialization
    //==============================================================================
    
    /// Initialize runtime features (call once at startup)
    void initializePlatform() noexcept;
    
    /// Check if platform is initialized
    bool isPlatformInitialized() noexcept;
}
```

### Individual Header APIs

Each header provides specialized functionality:

#### [`compiler.hpp`](include/trlc/platform/compiler.hpp) - Compiler Detection

```cpp
namespace trlc::platform {
    enum class CompilerType { gcc, clang, msvc, intel, unknown };
    
    constexpr CompilerType getCompilerType() noexcept;
    constexpr CompilerInfo getCompilerInfo() noexcept;
    
    // Compiler capability queries
    constexpr bool isGccCompatible() noexcept;
    constexpr bool isClangCompatible() noexcept;
    constexpr bool hasBuiltinFunctions() noexcept;
}
```

#### [`platform.hpp`](include/trlc/platform/platform.hpp) - OS Detection

```cpp
namespace trlc::platform {
    enum class OperatingSystem { 
        windows, linux_generic, macos, 
        freebsd, openbsd, netbsd, android, ios, unknown 
    };
    
    constexpr OperatingSystem getOperatingSystem() noexcept;
    constexpr PlatformInfo getPlatformInfo() noexcept;
    
    // Platform capability queries
    constexpr bool isPosix() noexcept;
    constexpr bool isUnixLike() noexcept;
    constexpr bool supportsCaseSensitiveFilesystem() noexcept;
}
```

#### [`architecture.hpp`](include/trlc/platform/architecture.hpp) - CPU Architecture

```cpp
namespace trlc::platform {
    enum class CpuArchitecture { 
        x86_64, arm_v8_64, arm_v7_32, 
        mips_64, powerpc_64, unknown 
    };
    
    constexpr CpuArchitecture getCpuArchitecture() noexcept;
    constexpr ArchitectureInfo getArchitectureInfo() noexcept;
    
    // Architecture queries
    constexpr bool is64Bit() noexcept;
    constexpr bool supportsUnalignedAccess() noexcept;
    constexpr bool hasSimdSupport() noexcept;
}
```

#### [`features.hpp`](include/trlc/platform/features.hpp) - Feature Detection

```cpp
namespace trlc::platform {
    enum class LanguageFeature {
        exceptions, rtti, threads, atomic_operations,
        concepts, ranges, coroutines, modules
    };
    
    enum class RuntimeFeature {
        sse, sse2, sse3, sse4_1, sse4_2,
        avx, avx2, avx512f,
        neon, hardware_aes, hardware_random
    };
    
    template <LanguageFeature TFeature>
    constexpr bool hasFeature() noexcept;
    
    bool hasRuntimeFeature(RuntimeFeature feature) noexcept;
    FeatureSet getFeatureSet() noexcept;
}
```

#### [`debug.hpp`](include/trlc/platform/debug.hpp) - Debug Utilities

```cpp
namespace trlc::platform {
    // Debug build detection
    bool isDebugBuild() noexcept;
    bool isReleaseBuild() noexcept;
    bool hasDebugInfo() noexcept;
    
    // Debug utilities
    class DebugUtils {
    public:
        static bool canCaptureStackTrace() noexcept;
        static void debugBreak() noexcept;
        static void unreachable() noexcept;
    };
}
```

### Template Metaprogramming API

Advanced template utilities in [`traits.hpp`](include/trlc/platform/traits.hpp):

```cpp
namespace trlc::platform::traits {
    
    //==============================================================================
    // SFINAE Helpers
    //==============================================================================
    
    /// Enable template if feature is available
    template <LanguageFeature TFeature>
    using enable_if_feature_t = std::enable_if_t<hasFeature<TFeature>(), int>;
    
    /// Enable template if feature is NOT available
    template <LanguageFeature TFeature>
    using enable_if_no_feature_t = std::enable_if_t<!hasFeature<TFeature>(), int>;
    
    /// Enable template if ALL features are available
    template <LanguageFeature... Features>
    using enable_if_all_features_t = std::enable_if_t<(hasFeature<Features>() && ...), int>;
    
    //==============================================================================
    // Variadic Feature Testing
    //==============================================================================
    
    /// Check if all specified features are available
    template <LanguageFeature... Features>
    constexpr bool hasAllFeatures() noexcept;
    
    /// Check if any of the specified features are available
    template <LanguageFeature... Features>
    constexpr bool hasAnyFeature() noexcept;
    
    /// Count how many features are available
    template <LanguageFeature... Features>
    constexpr size_t countFeatures() noexcept;
    
    //==============================================================================
    // Type Traits
    //==============================================================================
    
    /// Compile-time feature availability trait
    template <LanguageFeature TFeature>
    struct FeatureAvailable : std::bool_constant<hasFeature<TFeature>()> {};
    
    template <LanguageFeature TFeature>
    constexpr bool feature_available_v = FeatureAvailable<TFeature>::value;
    
    /// Platform matching trait
    template <OperatingSystem TOS>
    struct PlatformMatches : std::bool_constant<getOperatingSystem() == TOS> {};
    
    template <OperatingSystem TOS>
    constexpr bool platform_matches_v = PlatformMatches<TOS>::value;
}
```

### Convenient Macros

Preprocessor macros in [`macros.hpp`](include/trlc/platform/macros.hpp):

```cpp
//==============================================================================
// Platform Detection Macros
//==============================================================================

#define TRLC_PLATFORM_WINDOWS    /* 1 if Windows, 0 otherwise */
#define TRLC_PLATFORM_LINUX      /* 1 if Linux, 0 otherwise */
#define TRLC_PLATFORM_MACOS      /* 1 if macOS, 0 otherwise */
#define TRLC_COMPILER_GCC         /* 1 if GCC, 0 otherwise */
#define TRLC_COMPILER_CLANG       /* 1 if Clang, 0 otherwise */
#define TRLC_ARCH_X86_64          /* 1 if x86_64, 0 otherwise */

//==============================================================================
// Feature Testing Macros
//==============================================================================

/// Test for language feature availability
#define TRLC_HAS_FEATURE(feature) \
    (trlc::platform::hasFeature<trlc::platform::LanguageFeature::feature>())

/// Test for runtime feature availability  
#define TRLC_HAS_RUNTIME_FEATURE(feature) \
    (trlc::platform::hasRuntimeFeature(trlc::platform::RuntimeFeature::feature))

/// Convenient feature shortcuts
#define TRLC_HAS_EXCEPTIONS       TRLC_HAS_FEATURE(exceptions)
#define TRLC_HAS_THREADS          TRLC_HAS_FEATURE(threads)
#define TRLC_HAS_RTTI             TRLC_HAS_FEATURE(rtti)

//==============================================================================
// Conditional Compilation Macros
//==============================================================================

/// Execute code only if feature is available  
/// Note: Use TRLC_IF for general conditional compilation
/// Example: TRLC_IF(TRLC_HAS_FEATURE(exceptions), { /* exception code */ })

/// Require feature at compile time
#define TRLC_REQUIRE_FEATURE(feature) \
    static_assert(TRLC_HAS_FEATURE(feature), \
                  "Required feature '" #feature "' not available")

/// Platform-specific code blocks (using actual implementation)
/// Note: Use TRLC_ON_WINDOWS, TRLC_ON_POSIX for conditional compilation
/// Example: TRLC_ON_WINDOWS({ /* Windows code */ })
```

### Usage Patterns

#### Template Specialization Based on Platform

```cpp
#include "trlc/platform/platform.hpp"

// Platform-specific implementations using if constexpr
template<typename T>
class NetworkSocket {
public:
    bool connect(const std::string& address) {
        constexpr auto os = trlc::platform::getOperatingSystem();
        
        if constexpr (os == trlc::platform::OperatingSystem::windows) {
            // Windows-specific implementation using WinSock
            std::cout << "Windows socket implementation" << std::endl;
            return true;
        } else if constexpr (os == trlc::platform::OperatingSystem::linux_generic) {
            // Linux-specific implementation using POSIX sockets
            std::cout << "Linux socket implementation" << std::endl;
            return true;
        } else {
            // Generic POSIX implementation
            std::cout << "Generic POSIX socket implementation" << std::endl;
            return true;
        }
    }
};
```

#### Feature-Conditional Template Instantiation

```cpp
#include "trlc/platform/traits.hpp"

template<typename Container,
         trlc::platform::traits::enable_if_feature_t<
             trlc::platform::LanguageFeature::threads> = 0>
void parallelProcess(Container& data) {
    // Multi-threaded implementation
    std::cout << "Using parallel processing" << std::endl;
    std::sort(data.begin(), data.end());
}

template<typename Container,
         trlc::platform::traits::enable_if_no_feature_t<
             trlc::platform::LanguageFeature::threads> = 0>
void parallelProcess(Container& data) {
    // Single-threaded fallback
    std::cout << "Using single-threaded fallback" << std::endl;
    std::sort(data.begin(), data.end());
}
}
```

## Performance

### Zero Runtime Overhead

The TRLC Platform Library is designed for zero runtime overhead:

- **Compile-Time Detection**: All platform detection happens at compile time
- **Template Metaprogramming**: Conditional code paths selected at compile time
- **Dead Code Elimination**: Unused platform code is completely removed
- **Constexpr Everything**: Core functions are `constexpr` and fully optimized

### Benchmark Results

| Operation | Execution Time | Overhead |
|-----------|----------------|----------|
| Platform detection | 0 ns | **0%** |
| Feature checking | 0 ns | **0%** |
| Template specialization | 0 ns | **0%** |
| Conditional compilation | 0 ns | **0%** |

*Benchmarks performed with GCC 11.4 -O2 on x86_64 Linux*

### Assembly Verification

With optimizations enabled (`-O2` or higher), all TRLC platform detection calls are:
- Inlined to compile-time constants
- Generate no function calls in assembly output
- Produce optimal machine code equivalent to hand-written constants

## Building and Testing

### Prerequisites

```bash
# Ubuntu/Debian
sudo apt install build-essential cmake

# macOS
brew install cmake

# Windows (vcpkg)
vcpkg install cmake
```

### Build Commands

```bash
# Clone repository
git clone https://github.com/tranglecong/platform.git
cd platform

# Configure and build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Run tests
ctest --verbose
```

### Build Options

```bash
# Debug build with all assertions
cmake .. -DCMAKE_BUILD_TYPE=Debug -DTRLC_ENABLE_ASSERTS=ON

# Enable experimental features
cmake .. -DTRLC_ENABLE_EXPERIMENTAL=ON

# Force portable mode (disable platform-specific optimizations)
cmake .. -DTRLC_FORCE_PORTABLE=ON

# Disable testing
cmake .. -DTRLC_BUILD_TESTS=OFF
```

### Test Suite

The library includes comprehensive tests covering:

- ✅ **Platform Detection**: All supported platforms, compilers, architectures
- ✅ **Feature Detection**: Language features, runtime features, capabilities
- ✅ **Template Metaprogramming**: SFINAE, variadic templates, type traits
- ✅ **Cross-Platform Compatibility**: Multiple compiler/platform combinations
- ✅ **Performance**: Zero-overhead verification, assembly output analysis
- ✅ **Integration**: Real-world usage scenarios, sample applications

```bash

# Run tests with verbose output
ctest --verbose --parallel 4

# Generate coverage report (if configured)
make coverage
```

### Continuous Integration

The project uses GitHub Actions for CI/CD with testing on:
- Multiple compilers (GCC, Clang, MSVC)
- Multiple platforms (Ubuntu, Windows, macOS)
- Multiple C++ standards (C++17, C++20, C++23)
- Multiple build configurations (Debug, Release, MinSizeRel)

## Troubleshooting

### Common Issues

#### Compilation Errors

**Problem**: `error: 'constexpr' does not name a type`
```cpp
// ❌ Incorrect usage
auto platform = trlc::platform::getOperatingSystem();  // Runtime call

// ✅ Correct usage
constexpr auto platform = trlc::platform::getOperatingSystem();  // Compile-time
```

**Problem**: `undefined reference` errors when linking
```bash
# ❌ Missing threading library
g++ main.cpp -ltrlc-platform

# ✅ Include required system libraries
g++ main.cpp -ltrlc-platform -pthread
```

**Problem**: `hasRuntimeFeature` not available at compile time
```cpp
// ❌ Runtime features cannot be constexpr
if constexpr (trlc::platform::hasRuntimeFeature(RuntimeFeature::avx)) {

// ✅ Use runtime checks for runtime features
if (trlc::platform::hasRuntimeFeature(RuntimeFeature::avx)) {
```

#### CMake Integration Issues

**Problem**: `target 'trlc::platform' not found`
```cmake
# ❌ Missing dependency
target_link_libraries(my_target trlc::platform)

# ✅ Ensure library is available first
find_package(trlc-platform REQUIRED)  # or add_subdirectory()
target_link_libraries(my_target PRIVATE trlc::platform)
```

**Problem**: Debug utilities not available
```cmake
# ✅ Enable debug utilities explicitly
target_compile_definitions(my_target PRIVATE TRLC_PLATFORM_ENABLE_DEBUG_UTILS=1)

# ✅ Or use Debug build type
set(CMAKE_BUILD_TYPE Debug)
```

#### Runtime Issues

**Problem**: Runtime feature detection returns false positives/negatives
```cpp
// ✅ Initialize platform before using runtime features
int main() {
    trlc::platform::initializePlatform();  // Required for runtime features
    
    if (trlc::platform::hasRuntimeFeature(RuntimeFeature::avx)) {
        // Now safe to use AVX
    }
}
```

**Problem**: Performance overhead in Debug builds
```bash
# ✅ Use Release builds for performance testing
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### Debug Tips

#### Verify Compile-Time Detection

```cpp
#include "trlc/platform/core.hpp"
#include <iostream>

int main() {
    // Print detected platform information
    auto report = trlc::platform::getPlatformReport();
    std::cout << report.generateReport() << std::endl;
    
    // Verify specific detections
    constexpr auto os = trlc::platform::getOperatingSystem();
    constexpr auto compiler = trlc::platform::getCompilerType();
    constexpr auto arch = trlc::platform::getCpuArchitecture();
    
    std::cout << "OS: " << static_cast<int>(os) << std::endl;
    std::cout << "Compiler: " << static_cast<int>(compiler) << std::endl;
    std::cout << "Architecture: " << static_cast<int>(arch) << std::endl;
}
```

#### Assembly Output Verification

```bash
# Verify zero overhead with assembly output
g++ -I./include -std=c++17 -O2 -S your_code.cpp -o output.s

# Look for platform detection function calls (should be none)
grep -i "platform\|detect" output.s

# Check that conditionals are optimized away
grep -i "branch\|jump\|call" output.s
```

#### Template Instantiation Debugging

```cpp
// Use static_assert to verify compile-time conditions
static_assert(trlc::platform::getOperatingSystem() == trlc::platform::OperatingSystem::linux_generic,
              "This code requires Linux");

static_assert(trlc::platform::hasFeature<trlc::platform::LanguageFeature::threads>(),
              "Threading support required");
```

### Platform-Specific Notes

#### Windows with MinGW

```cmake
# Ensure proper Windows detection with MinGW
if(MINGW)
    target_compile_definitions(your_target PRIVATE TRLC_MINGW_DETECTED=1)
endif()
```

#### macOS with Xcode

```bash
# Use proper C++ standard library
export MACOSX_DEPLOYMENT_TARGET=10.15
cmake .. -DCMAKE_CXX_STANDARD=17
```

#### ARM Cross-Compilation

```cmake
# Set proper architecture detection for cross-compilation
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_CROSSCOMPILING TRUE)
```

### Getting Help

If you encounter issues not covered here:

1. **Check the [Examples](examples/)** - Real-world usage patterns
2. **Review [API Documentation](docs/)** - Comprehensive API reference
3. **Search [GitHub Issues](https://github.com/tranglecong/platform/issues)** - Known issues and solutions
4. **Create a [New Issue](https://github.com/tranglecong/platform/issues/new)** - Report bugs or request features

Include in your issue report:
- Compiler version and platform
- CMake version and configuration
- Minimal reproducible example
- Full error messages and stack traces

## Examples

See [`examples/portable_library_example.cpp`](examples/portable_library_example.cpp) for a comprehensive demonstration.

```bash
# Build and run the demo
mkdir build && cd build
cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DTRLC_PLATFORM_BUILD_TESTS=ON \
    -DTRLC_PLATFORM_ENABLE_ASSERTS=ON \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

make -j$(nproc)
./portable_library_example
```

## Contributing

We welcome contributions to the TRLC Platform Library! Whether you're fixing bugs, adding features, improving documentation, or enhancing platform support, your help is appreciated.

### Development Setup

#### Prerequisites

```bash
# Ubuntu/Debian
sudo apt install build-essential cmake git clang-format

# macOS
brew install cmake git clang-format

# Windows (using vcpkg)
vcpkg install cmake
```

#### Building for Development

```bash
git clone https://github.com/tranglecong/platform.git
cd trlc-platform

# Create development build
mkdir build && cd build
cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DTRLC_PLATFORM_BUILD_TESTS=ON \
    -DTRLC_PLATFORM_ENABLE_ASSERTS=ON \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

make -j$(nproc)
```

#### Running Tests

```bash
# Run all tests
cd build
ctest --output-on-failure --verbose
```

### Adding New Platform Support

#### 1. Platform Detection

Add detection logic to [`include/trlc/platform/platform.hpp`](include/trlc/platform/platform.hpp):

```cpp
// Add new enum value
enum class OperatingSystem : int {
    // ... existing values ...
    your_new_platform,
};

// Add detection logic
constexpr OperatingSystem detectOperatingSystem() noexcept {
#if defined(YOUR_PLATFORM_MACRO)
    return OperatingSystem::your_new_platform;
#elif defined(EXISTING_PLATFORM)
    // ... existing detection ...
```

#### 2. Add Platform Information

Update the platform info structure:

```cpp
constexpr PlatformInfo getPlatformInfo() noexcept {
    PlatformInfo info{};
    // ... existing code ...
    
#if defined(YOUR_PLATFORM_MACRO)
    info.os_name = "Your Platform Name";
    info.kernel_family = "your_kernel";
    info.environment = EnvironmentType::your_env_type;
    // Set other platform-specific properties
#endif
    
    return info;
}
```

#### 3. Add Tests

Create tests in [`tests/test_platform_detection.cpp`](tests/test_platform_detection.cpp):

```cpp
TEST_CASE("Your Platform Detection", "[platform]") {
#if defined(YOUR_PLATFORM_MACRO)
    REQUIRE(getOperatingSystem() == OperatingSystem::your_new_platform);
    REQUIRE(getPlatformInfo().os_name == "Your Platform Name");
#endif
}
```
### Adding New Compiler Support

#### 1. Compiler Detection

Add to [`include/trlc/platform/compiler.hpp`](include/trlc/platform/compiler.hpp):

```cpp
enum class CompilerType : int {
    // ... existing values ...
    your_new_compiler,
};

constexpr CompilerType detectCompilerType() noexcept {
#if defined(YOUR_COMPILER_MACRO)
    return CompilerType::your_new_compiler;
#elif defined(EXISTING_COMPILER)
    // ... existing detection ...
```

#### 2. Add Compiler Capabilities

```cpp
constexpr CompilerInfo getCompilerInfo() noexcept {
    CompilerInfo info{};
    
#if defined(YOUR_COMPILER_MACRO)
    info.type = CompilerType::your_new_compiler;
    info.name = "Your Compiler Name";
    info.version = detectCompilerVersion();
    info.has_builtin_attribute = true;  // Set capabilities
    info.has_inline_assembly = true;
    // ... other capabilities
#endif
    
    return info;
}
```

### Adding New Features

#### 1. Language Features

Add to [`include/trlc/platform/features.hpp`](include/trlc/platform/features.hpp):

```cpp
enum class LanguageFeature : int {
    // ... existing features ...
    your_new_feature,
};

template<>
constexpr bool hasLanguageFeature<LanguageFeature::your_new_feature>() noexcept {
#if defined(__cpp_your_feature) && __cpp_your_feature >= REQUIRED_VERSION
    return true;
#else
    return false;
#endif
}
```

#### 2. Runtime Features

```cpp
enum class RuntimeFeature : int {
    // ... existing features ...
    your_runtime_feature,
};

// Add detection logic in FeatureDetector class
bool hasRuntimeFeature(RuntimeFeature feature) const noexcept {
    switch (feature) {
        case RuntimeFeature::your_runtime_feature:
            return detectYourRuntimeFeature();
        // ... other cases
    }
}
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- C++ standardization committee for modern language features
- Platform vendors for comprehensive documentation
- Open source community for testing and feedback
- Contributors and maintainers

---

**TRLC Platform Library** - Write once, optimize everywhere. 🚀

For more information, visit our [documentation](docs/) or check out the [examples](examples/).

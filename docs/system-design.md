# TRLC Platform Design Document

## 📋 Overview

**trlc-platform** is a foundational utility library that provides compile-time platform detection and abstraction for the `trlc-*` ecosystem. It standardizes compiler, platform, architecture, and feature detection while providing portable macros for modern C++ development.

**Core Objectives:**
- Standardize compile-time detection mechanisms across all trlc-* components
- Provide portable macros and utilities for C++17/20 code
- Separate platform-specific concerns from business logic  
- Establish a clean, reusable platform abstraction layer

---

## 🏗️ Architecture Overview

### Namespace Design
```cpp
namespace trlc {
namespace platform {
    // All platform detection and abstraction utilities
}
}
```

### Module Dependencies
```
core.hpp (main entry)
    ├── compiler.hpp
    ├── platform.hpp  
    ├── architecture.hpp
    ├── cpp_standard.hpp
    ├── features.hpp
    ├── macros.hpp
    ├── endianness.hpp
    ├── typeinfo.hpp
    └── debug.hpp (optional)
```

---

## 🔧 Component Detailed Designs

### 1. Compiler Detection (`include/trlc/platform/compiler.hpp`)

#### Purpose
Detect and provide information about the current compiler at compile time.

#### Interface Design
```cpp
namespace trlc {
namespace platform {

// Compiler identification enumeration
enum class CompilerType : int {
    unknown = 0,
    gcc,
    clang,
    msvc,
    intel_classic,  // Legacy Intel C++ Compiler
    intel_llvm,     // Intel C++ Compiler based on LLVM
    mingw
};

// Compiler version representation
struct CompilerVersion {
    int major;
    int minor;
    int patch;
    
    constexpr CompilerVersion(int maj, int min, int pat = 0) noexcept;
    constexpr bool operator>=(const CompilerVersion& other) const noexcept;
    constexpr bool operator<(const CompilerVersion& other) const noexcept;
    constexpr bool operator==(const CompilerVersion& other) const noexcept;
};

// Core detection functions
constexpr CompilerType getCompilerType() noexcept;
constexpr CompilerVersion getCompilerVersion() noexcept;
constexpr const char* getCompilerName() noexcept;

// Compiler capability queries
constexpr bool hasBuiltinAttribute(const char* attribute_name) noexcept;
constexpr bool supportsInlineAssembly() noexcept;
constexpr bool hasColorDiagnostics() noexcept;

} // namespace platform
} // namespace trlc

// Convenience macros for conditional compilation
#define TRLC_COMPILER_GCC     (trlc::platform::getCompilerType() == trlc::platform::CompilerType::gcc)
#define TRLC_COMPILER_CLANG   (trlc::platform::getCompilerType() == trlc::platform::CompilerType::clang)
#define TRLC_COMPILER_MSVC    (trlc::platform::getCompilerType() == trlc::platform::CompilerType::msvc)
#define TRLC_COMPILER_INTEL   (trlc::platform::getCompilerType() == trlc::platform::CompilerType::intel_classic)

#define TRLC_COMPILER_VERSION_ATLEAST(major, minor, patch) \
    (trlc::platform::getCompilerVersion() >= trlc::platform::CompilerVersion{major, minor, patch})
```

#### Implementation Strategy
- Use predefined compiler macros (`__GNUC__`, `__clang__`, `_MSC_VER`, etc.)
- Extract version information from compiler-specific macros
- Provide fallbacks for unknown compilers

---

### 2. Platform Detection (`include/trlc/platform/platform.hpp`)

#### Purpose
Detect operating system and environment at compile time.

#### Interface Design
```cpp
namespace trlc {
namespace platform {

// Operating system enumeration
enum class OperatingSystem : int {
    unknown = 0,
    windows,
    linux_generic,
    ubuntu,
    debian, 
    redhat,
    macos,
    freebsd,
    openbsd,
    netbsd,
    android,
    ios
};

// Environment type
enum class EnvironmentType : int {
    unknown = 0,
    desktop,
    server,
    embedded,
    mobile
};

// Platform information structure
struct PlatformInfo {
    OperatingSystem os;
    EnvironmentType environment;
    const char* os_name;
    const char* kernel_family;  // "nt", "posix", "unix"
    
    constexpr bool isPosix() const noexcept;
    constexpr bool isUnix() const noexcept;
    constexpr bool isWindows() const noexcept;
};

// Core detection functions
constexpr OperatingSystem getOperatingSystem() noexcept;
constexpr EnvironmentType getEnvironmentType() noexcept;
constexpr PlatformInfo getPlatformInfo() noexcept;

// Feature queries
constexpr bool hasPosixApi() noexcept;
constexpr bool hasWin32Api() noexcept;
constexpr bool supportsCaseSensitiveFilesystem() noexcept;

} // namespace platform
} // namespace trlc

// Convenience macros
#define TRLC_PLATFORM_WINDOWS  (trlc::platform::getOperatingSystem() == trlc::platform::OperatingSystem::windows)
#define TRLC_PLATFORM_LINUX    (trlc::platform::getOperatingSystem() == trlc::platform::OperatingSystem::linux_generic)
#define TRLC_PLATFORM_MACOS    (trlc::platform::getOperatingSystem() == trlc::platform::OperatingSystem::macos)
#define TRLC_PLATFORM_POSIX    (trlc::platform::hasPosixApi())
```

---

### 3. Architecture Detection (`include/trlc/platform/architecture.hpp`)

#### Purpose
Detect CPU architecture and provide architecture-specific information.

#### Interface Design
```cpp
namespace trlc {
namespace platform {

// CPU architecture enumeration
enum class CpuArchitecture : int {
    unknown = 0,
    x86,
    x86_64,
    arm_v6,
    arm_v7,
    arm_v8_32,  // AArch32
    arm_v8_64,  // AArch64/ARM64
    mips,
    mips_64,
    powerpc,
    powerpc_64,
    risc_v_32,
    risc_v_64,
    sparc,
    sparc_64
};

// Endianness enumeration  
enum class ByteOrder : int {
    unknown = 0,
    little_endian,
    big_endian,
    mixed_endian  // Some rare architectures
};

// Architecture information structure
struct ArchitectureInfo {
    CpuArchitecture architecture;
    ByteOrder byte_order;
    int pointer_size_bits;
    int cache_line_size;
    const char* arch_name;
    
    constexpr bool is64Bit() const noexcept;
    constexpr bool is32Bit() const noexcept;
    constexpr bool isLittleEndian() const noexcept;
    constexpr bool supportsUnalignedAccess() const noexcept;
};

// Core detection functions
constexpr CpuArchitecture getCpuArchitecture() noexcept;
constexpr ByteOrder getByteOrder() noexcept;
constexpr ArchitectureInfo getArchitectureInfo() noexcept;
constexpr int getPointerSize() noexcept;

// Architecture capability queries
constexpr bool hasSimdSupport() noexcept;
constexpr bool hasVectorInstructions() noexcept;
constexpr bool supportsCacheLineAlignment() noexcept;

} // namespace platform  
} // namespace trlc

// Convenience macros
#define TRLC_ARCH_X86_64       (trlc::platform::getCpuArchitecture() == trlc::platform::CpuArchitecture::x86_64)
#define TRLC_ARCH_ARM64        (trlc::platform::getCpuArchitecture() == trlc::platform::CpuArchitecture::arm_v8_64)
#define TRLC_ARCH_32BIT        (trlc::platform::getPointerSize() == 4)
#define TRLC_ARCH_64BIT        (trlc::platform::getPointerSize() == 8)
```

---

### 4. C++ Standard Detection (`include/trlc/platform/cpp_standard.hpp`)

#### Purpose
Detect active C++ standard version and provide standard-specific utilities.

#### Interface Design
```cpp
namespace trlc {
namespace platform {

// C++ standard enumeration
enum class CppStandard : int {
    cpp_pre_17 = 0,
    cpp_17 = 201703L,
    cpp_20 = 202002L,
    cpp_23 = 202302L,
    cpp_26 = 202600L,  // Tentative
    cpp_unknown = -1
};

// Standard information structure
struct CppStandardInfo {
    CppStandard standard;
    long version_macro;
    const char* standard_name;
    
    constexpr bool isAtLeast(CppStandard min_standard) const noexcept;
    constexpr bool supportsFeature(const char* feature_name) const noexcept;
};

// Core detection functions
constexpr CppStandard getCppStandard() noexcept;
constexpr CppStandardInfo getCppStandardInfo() noexcept;
constexpr long getCppVersionMacro() noexcept;

// Standard feature queries
constexpr bool hasStructuredBindings() noexcept;    // C++17
constexpr bool hasIfConstexpr() noexcept;          // C++17
constexpr bool hasConcepts() noexcept;             // C++20
constexpr bool hasCoroutines() noexcept;           // C++20
constexpr bool hasModules() noexcept;              // C++20
constexpr bool hasRanges() noexcept;               // C++20

} // namespace platform
} // namespace trlc

// Convenience macros
#define TRLC_CPP17      (trlc::platform::getCppStandard() >= trlc::platform::CppStandard::cpp_17)
#define TRLC_CPP20      (trlc::platform::getCppStandard() >= trlc::platform::CppStandard::cpp_20)
#define TRLC_CPP23      (trlc::platform::getCppStandard() >= trlc::platform::CppStandard::cpp_23)
#define TRLC_CPP_VERSION (trlc::platform::getCppVersionMacro())

// Conditional compilation helpers
#define TRLC_IF_CPP17(code)     TRLC_CPP17 ? (code) : void(0)
#define TRLC_IF_CPP20(code)     TRLC_CPP20 ? (code) : void(0)
```

---

### 5. Feature Detection (`include/trlc/platform/features.hpp`)

#### Purpose
Detect runtime and compile-time language features availability.

#### Interface Design
```cpp
namespace trlc {
namespace platform {

// Feature flags enumeration
enum class LanguageFeature : int {
    exceptions = 0,
    rtti,
    threads,
    atomic_operations,
    inline_assembly,
    vector_intrinsics,
    stack_protection,
    address_sanitizer,
    thread_sanitizer,
    memory_sanitizer,
    undefined_behavior_sanitizer
};

// Runtime feature flags
enum class RuntimeFeature : int {
    sse = 0,
    sse2,
    sse3,
    sse4_1,
    sse4_2,
    avx,
    avx2,
    avx512f,
    neon,
    hardware_aes,
    hardware_random
};

// Feature detection structure
struct FeatureSet {
    // Language features (compile-time)
    bool has_exceptions;
    bool has_rtti;
    bool has_threads;
    bool has_atomic;
    bool has_inline_asm;
    
    // Runtime features (may require runtime detection)
    bool has_sse;
    bool has_avx;
    bool has_neon;
    
    constexpr bool hasLanguageFeature(LanguageFeature feature) const noexcept;
    bool hasRuntimeFeature(RuntimeFeature feature) const noexcept;  // Non-constexpr for runtime detection
};

// Core detection functions
constexpr FeatureSet getFeatureSet() noexcept;
constexpr bool hasLanguageFeature(LanguageFeature feature) noexcept;
bool hasRuntimeFeature(RuntimeFeature feature) noexcept;

// Specific feature queries (compile-time)
constexpr bool hasExceptions() noexcept;
constexpr bool hasRtti() noexcept;
constexpr bool hasThreads() noexcept;
constexpr bool hasAtomicOperations() noexcept;

// Runtime feature detection (CPU features)
bool hasSseSupport() noexcept;
bool hasAvxSupport() noexcept;
bool hasNeonSupport() noexcept;

// Generic feature test macro
template<typename TFeature>
constexpr bool hasFeature() noexcept;

} // namespace platform
} // namespace trlc

// Convenience macros for compile-time features
#define TRLC_HAS_EXCEPTIONS    (trlc::platform::hasExceptions())
#define TRLC_HAS_RTTI         (trlc::platform::hasRtti())
#define TRLC_HAS_THREADS      (trlc::platform::hasThreads())
#define TRLC_HAS_ATOMIC       (trlc::platform::hasAtomicOperations())

// Generic feature test
#define TRLC_HAS_FEATURE(X)   (trlc::platform::hasFeature<trlc::platform::LanguageFeature::X>())
```

---

### 6. Macro Utilities (`include/trlc/platform/macros.hpp`)

#### Purpose
Provide portable, cross-platform macros for common C++ idioms and optimizations.

#### Interface Design
```cpp
namespace trlc {
namespace platform {

// Attribute wrapper functions (for constexpr contexts)
constexpr bool supportsNodecard() noexcept;
constexpr bool supportsDeprecated() noexcept;
constexpr bool supportsFallthrough() noexcept;

} // namespace platform
} // namespace trlc

// Core attribute macros
#ifdef __has_cpp_attribute
#  define TRLC_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#  define TRLC_HAS_CPP_ATTRIBUTE(x) 0
#endif

// Portable attributes
#if TRLC_HAS_CPP_ATTRIBUTE(nodiscard) >= 201603L
#  define TRLC_NODISCARD [[nodiscard]]
#elif defined(__GNUC__) || defined(__clang__)
#  define TRLC_NODISCARD __attribute__((warn_unused_result))
#else
#  define TRLC_NODISCARD
#endif

#if TRLC_HAS_CPP_ATTRIBUTE(deprecated) >= 201309L
#  define TRLC_DEPRECATED [[deprecated]]
#  define TRLC_DEPRECATED_MSG(msg) [[deprecated(msg)]]
#elif defined(__GNUC__) || defined(__clang__)
#  define TRLC_DEPRECATED __attribute__((deprecated))
#  define TRLC_DEPRECATED_MSG(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#  define TRLC_DEPRECATED __declspec(deprecated)
#  define TRLC_DEPRECATED_MSG(msg) __declspec(deprecated(msg))
#else
#  define TRLC_DEPRECATED
#  define TRLC_DEPRECATED_MSG(msg)
#endif

// Inlining control
#if defined(__GNUC__) || defined(__clang__)
#  define TRLC_FORCE_INLINE __attribute__((always_inline)) inline
#  define TRLC_NEVER_INLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#  define TRLC_FORCE_INLINE __forceinline
#  define TRLC_NEVER_INLINE __declspec(noinline)
#else
#  define TRLC_FORCE_INLINE inline
#  define TRLC_NEVER_INLINE
#endif

#define TRLC_INLINE inline

// Branch prediction hints
#if defined(__GNUC__) || defined(__clang__)
#  define TRLC_LIKELY(x)    __builtin_expect(!!(x), 1)
#  define TRLC_UNLIKELY(x)  __builtin_expect(!!(x), 0)
#else
#  define TRLC_LIKELY(x)    (x)
#  define TRLC_UNLIKELY(x)  (x)
#endif

// Fallthrough annotation
#if TRLC_HAS_CPP_ATTRIBUTE(fallthrough) >= 201603L
#  define TRLC_FALLTHROUGH [[fallthrough]]
#elif defined(__GNUC__) && __GNUC__ >= 7
#  define TRLC_FALLTHROUGH __attribute__((fallthrough))
#else
#  define TRLC_FALLTHROUGH ((void)0)
#endif

// Exception safety
#if TRLC_HAS_EXCEPTIONS
#  define TRLC_NOEXCEPT noexcept
#  define TRLC_NOEXCEPT_IF(cond) noexcept(cond)
#else
#  define TRLC_NOEXCEPT
#  define TRLC_NOEXCEPT_IF(cond)
#endif

// Constexpr utilities
#if TRLC_CPP17
#  define TRLC_CONSTEXPR_IF constexpr
#else
#  define TRLC_CONSTEXPR_IF
#endif

// Symbol visibility (for shared libraries)
#if defined(_WIN32) || defined(_WIN64)
#  define TRLC_API_EXPORT __declspec(dllexport)
#  define TRLC_API_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
#  define TRLC_API_EXPORT __attribute__((visibility("default")))
#  define TRLC_API_IMPORT __attribute__((visibility("default")))
#else
#  define TRLC_API_EXPORT
#  define TRLC_API_IMPORT
#endif

// Conditional API macro
#ifdef TRLC_BUILDING_SHARED
#  define TRLC_API TRLC_API_EXPORT
#else
#  define TRLC_API TRLC_API_IMPORT
#endif
```

---

### 7. Endianness Detection (`include/trlc/platform/endianness.hpp`)

#### Purpose
Detect and provide utilities for byte order handling.

#### Interface Design
```cpp
namespace trlc {
namespace platform {

// Byte order enumeration (reused from architecture.hpp)
using ByteOrder = trlc::platform::ByteOrder;

// Endianness detection and utilities
struct EndiannessInfo {
    ByteOrder byte_order;
    bool is_little_endian;
    bool is_big_endian;
    
    constexpr bool isNativeOrder(ByteOrder order) const noexcept;
    constexpr bool needsByteSwap(ByteOrder target_order) const noexcept;
};

// Core detection functions
constexpr ByteOrder getByteOrder() noexcept;
constexpr EndiannessInfo getEndiannessInfo() noexcept;
constexpr bool isLittleEndian() noexcept;
constexpr bool isBigEndian() noexcept;

// Byte swap utilities
template<typename TInt>
constexpr TInt byteSwap(TInt value) noexcept;

constexpr uint16_t byteSwap16(uint16_t value) noexcept;
constexpr uint32_t byteSwap32(uint32_t value) noexcept;
constexpr uint64_t byteSwap64(uint64_t value) noexcept;

// Network/host byte order conversion
template<typename TInt>
constexpr TInt hostToNetwork(TInt value) noexcept;

template<typename TInt>
constexpr TInt networkToHost(TInt value) noexcept;

} // namespace platform
} // namespace trlc

// Convenience macros
#define TRLC_ENDIAN_LITTLE   (trlc::platform::isLittleEndian())
#define TRLC_ENDIAN_BIG      (trlc::platform::isBigEndian())
#define TRLC_BYTE_SWAP(x)    (trlc::platform::byteSwap(x))
```

---

### 8. Type Information (`include/trlc/platform/typeinfo.hpp`)

#### Purpose
Provide compile-time type and alignment information utilities.

#### Interface Design
```cpp
namespace trlc {
namespace platform {

// Alignment utilities
template<typename TType>
constexpr size_t getTypeSize() noexcept;

template<typename TType>
constexpr size_t getTypeAlignment() noexcept;

constexpr size_t getCacheLineSize() noexcept;
constexpr size_t getPageSize() noexcept;

// Type trait extensions
template<typename TType>
struct TypeInfo {
    static constexpr size_t size = sizeof(TType);
    static constexpr size_t alignment = alignof(TType);
    static constexpr bool is_cache_line_aligned = (alignment >= getCacheLineSize());
    static constexpr bool is_trivially_copyable = std::is_trivially_copyable_v<TType>;
    static constexpr bool is_standard_layout = std::is_standard_layout_v<TType>;
};

// Padding analysis utilities
template<typename TType>
constexpr size_t calculatePadding() noexcept;

template<typename TType>
constexpr bool hasInternalPadding() noexcept;

// Alignment helpers
template<size_t TAlignment>
struct AlignedType {
    alignas(TAlignment) char data[TAlignment];
};

using CacheLineAligned = AlignedType<64>;  // Most common cache line size
using PageAligned = AlignedType<4096>;     // Most common page size

// Memory layout verification
template<typename TType, size_t TExpectedSize>
constexpr bool verifyTypeSize() noexcept;

template<typename TType, size_t TExpectedAlignment>
constexpr bool verifyTypeAlignment() noexcept;

} // namespace platform
} // namespace trlc

// Utility macros
#define TRLC_ALIGNOF(type)           (alignof(type))
#define TRLC_SIZEOF(type)            (sizeof(type))
#define TRLC_CACHE_LINE_SIZE         (trlc::platform::getCacheLineSize())
#define TRLC_ALIGN_TO_CACHE_LINE     alignas(trlc::platform::getCacheLineSize())
```

---

### 9. Debug Utilities (`include/trlc/platform/debug.hpp`)

#### Purpose
Provide portable debug and assertion utilities.

#### Interface Design
```cpp
namespace trlc {
namespace platform {

// Debug mode detection
constexpr bool isDebugBuild() noexcept;
constexpr bool isReleaseBuild() noexcept;
constexpr bool hasDebugInfo() noexcept;

// Assertion handler type
using AssertionHandler = void(*)(const char* expression, 
                               const char* file,
                               int line,
                               const char* function);

// Debug utilities
class DebugUtils {
public:
    // Assertion management
    static void setAssertionHandler(AssertionHandler handler) noexcept;
    static AssertionHandler getAssertionHandler() noexcept;
    
    // Debug break utilities
    static void debugBreak() noexcept;
    static void unreachable() noexcept;
    static void abort(const char* message = nullptr) noexcept;
    
    // Stack trace (if available)
    static bool canCaptureStackTrace() noexcept;
    static void printStackTrace() noexcept;

private:
    static AssertionHandler _assertion_handler;
};

// Default assertion handler
void defaultAssertionHandler(const char* expression,
                           const char* file, 
                           int line,
                           const char* function) noexcept;

} // namespace platform
} // namespace trlc

// Debug macros
#ifdef NDEBUG
#  define TRLC_DEBUG_BUILD 0
#else
#  define TRLC_DEBUG_BUILD 1
#endif

#if TRLC_DEBUG_BUILD
#  define TRLC_ASSERT(expr) \
    ((expr) ? void(0) : trlc::platform::DebugUtils::getAssertionHandler()(#expr, __FILE__, __LINE__, __func__))
#  define TRLC_DEBUG_ONLY(code) code
#else
#  define TRLC_ASSERT(expr) ((void)0)
#  define TRLC_DEBUG_ONLY(code) ((void)0)
#endif

#define TRLC_UNREACHABLE() trlc::platform::DebugUtils::unreachable()
#define TRLC_ABORT(msg)    trlc::platform::DebugUtils::abort(msg)

// Debug break
#if defined(_MSC_VER)
#  define TRLC_DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#  if defined(__i386__) || defined(__x86_64__)
#    define TRLC_DEBUG_BREAK() __asm__ volatile("int3")
#  else
#    define TRLC_DEBUG_BREAK() __builtin_trap()
#  endif
#else
#  define TRLC_DEBUG_BREAK() trlc::platform::DebugUtils::debugBreak()
#endif
```

---

### 10. Core Header (`include/trlc/platform/core.hpp`)

#### Purpose
Main entry point that includes all platform detection headers.

#### Interface Design
```cpp
#pragma once

// Include all platform detection headers
#include "trlc/platform/compiler.hpp"
#include "trlc/platform/platform.hpp" 
#include "trlc/platform/architecture.hpp"
#include "trlc/platform/cpp_standard.hpp"
#include "trlc/platform/features.hpp"
#include "trlc/platform/macros.hpp"
#include "trlc/platform/endianness.hpp"
#include "trlc/platform/typeinfo.hpp"

#ifdef TRLC_PLATFORM_ENABLE_DEBUG_UTILS
#  include "trlc/platform/debug.hpp"
#endif

namespace trlc {
namespace platform {

// Consolidated platform information
struct PlatformReport {
    CompilerInfo compiler;
    PlatformInfo platform;
    ArchitectureInfo architecture;
    CppStandardInfo cpp_standard;
    FeatureSet features;
    EndiannessInfo endianness;
    
    // Generate human-readable report
    std::string generateReport() const;
    void printReport() const;
};

// Core platform detection function
constexpr PlatformReport getPlatformReport() noexcept;

// Initialization (if needed for runtime features)
void initializePlatform() noexcept;
bool isPlatformInitialized() noexcept;

// Version information
struct Version {
    static constexpr int MAJOR = 1;
    static constexpr int MINOR = 0; 
    static constexpr int PATCH = 0;
    static constexpr const char* STRING = "1.0.0";
};

} // namespace platform
} // namespace trlc

// Master include guard
#define TRLC_PLATFORM_INCLUDED
```

---

## 🔨 CMake Integration Design

### CMakeLists.txt Structure
```cmake
cmake_minimum_required(VERSION 3.16)
project(trlc-platform 
    VERSION 1.0.0
    DESCRIPTION "Platform detection and abstraction layer"
    LANGUAGES CXX)

# Options
option(TRLC_PLATFORM_ENABLE_ASSERTS "Enable assertion macros" OFF)
option(TRLC_PLATFORM_ENABLE_EXPERIMENTAL "Enable experimental features" OFF)
option(TRLC_PLATFORM_FORCE_PORTABLE "Force portable implementations" OFF)
option(TRLC_PLATFORM_BUILD_TESTS "Build unit tests" ON)

# C++ standard requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform detection modules
include(cmake/CheckCompiler.cmake)
include(cmake/CheckPlatform.cmake) 
include(cmake/CheckFeatures.cmake)

# Main library target (header-only)
add_library(trlc-platform INTERFACE)
add_library(trlc::platform ALIAS trlc-platform)

# Include directories
target_include_directories(trlc-platform
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

# Compiler requirements
target_compile_features(trlc-platform INTERFACE cxx_std_17)

# Configuration
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/trlc/platform/config.hpp"
    @ONLY)

target_include_directories(trlc-platform
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>)

# Tests
if(TRLC_PLATFORM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation
include(cmake/Install.cmake)
```

### CMake Detection Modules

#### `cmake/CheckCompiler.cmake`
```cmake
# Detect compiler and version
function(trlc_detect_compiler)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(TRLC_COMPILER_TYPE "gcc" CACHE STRING "Detected compiler")
        set(TRLC_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}" CACHE STRING "Compiler version")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(TRLC_COMPILER_TYPE "clang" CACHE STRING "Detected compiler")
        set(TRLC_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}" CACHE STRING "Compiler version")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(TRLC_COMPILER_TYPE "msvc" CACHE STRING "Detected compiler")
        set(TRLC_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}" CACHE STRING "Compiler version")
    else()
        set(TRLC_COMPILER_TYPE "unknown" CACHE STRING "Detected compiler")
    endif()
    
    message(STATUS "Detected compiler: ${TRLC_COMPILER_TYPE} ${TRLC_COMPILER_VERSION}")
endfunction()
```

#### `cmake/CheckPlatform.cmake`  
```cmake
# Detect platform and environment
function(trlc_detect_platform)
    if(WIN32)
        set(TRLC_PLATFORM_TYPE "windows" CACHE STRING "Detected platform")
    elseif(APPLE)
        set(TRLC_PLATFORM_TYPE "macos" CACHE STRING "Detected platform") 
    elseif(UNIX)
        set(TRLC_PLATFORM_TYPE "linux" CACHE STRING "Detected platform")
    else()
        set(TRLC_PLATFORM_TYPE "unknown" CACHE STRING "Detected platform")
    endif()
    
    message(STATUS "Detected platform: ${TRLC_PLATFORM_TYPE}")
endfunction()
```

---

## 📝 Usage Examples

### Basic Usage
```cpp
#include "trlc/platform/core.hpp"

void example_basic_usage() {
    // Check compiler
    if constexpr (TRLC_COMPILER_GCC) {
        // GCC-specific code
    }
    
    // Check platform  
    if constexpr (TRLC_PLATFORM_LINUX) {
        // Linux-specific code
    }
    
    // Check architecture
    if constexpr (TRLC_ARCH_X86_64) {
        // x64-specific optimizations
    }
    
    // Use modern C++ features conditionally
    if constexpr (TRLC_CPP20) {
        // Use C++20 features like concepts
    }
}
```

### Advanced Feature Detection
```cpp
#include "trlc/platform/core.hpp"

class OptimizedProcessor {
public:
    void processData(const std::vector<float>& data) {
        // Runtime feature detection for SIMD
        if (trlc::platform::hasAvxSupport()) {
            processWithAvx(data);
        } else if (trlc::platform::hasSseSupport()) {
            processWithSse(data);  
        } else {
            processGeneric(data);
        }
    }
    
private:
    TRLC_FORCE_INLINE void processWithAvx(const std::vector<float>& data) {
        // AVX-optimized implementation
    }
    
    void processWithSse(const std::vector<float>& data) {
        // SSE-optimized implementation  
    }
    
    void processGeneric(const std::vector<float>& data) {
        // Portable fallback
    }
};
```

### Exception-Safe Code
```cpp
class SafeResource {
public:
    SafeResource() TRLC_NOEXCEPT_IF(!TRLC_HAS_EXCEPTIONS) {
        if constexpr (TRLC_HAS_EXCEPTIONS) {
            // Exception-enabled path
        } else {
            // Exception-disabled path with error codes
        }
    }
    
    TRLC_NODISCARD bool tryAcquire() TRLC_NOEXCEPT {
        // Always noexcept implementation
        return true;
    }
};
```

---

## 🧪 Testing Strategy

### Unit Test Coverage
- **Compile-time tests**: Static assertions for detection accuracy
- **Runtime tests**: Feature detection verification
- **Cross-platform tests**: Behavior consistency across platforms
- **Integration tests**: CMake configuration verification

### Test Structure
```cpp
// tests/test_compiler_detection.cpp
#include "trlc/platform/compiler.hpp"
#include <cassert>

void test_compiler_detection() {
    // Verify compiler detection
    static_assert(trlc::platform::getCompilerType() != trlc::platform::CompilerType::unknown);
    
    // Verify version parsing
    auto version = trlc::platform::getCompilerVersion();
    assert(version.major > 0);
}
```

---

## 📋 Implementation Requirements

### Mandatory Features
1. **Header-only implementation** - No compiled libraries required
2. **Constexpr everything** - All detection must be compile-time when possible
3. **Zero runtime overhead** - All macros resolve at compile time
4. **C++17 compatibility** - Minimum standard requirement
5. **No external dependencies** - Only standard library usage

### Performance Requirements  
- **Compile-time cost**: < 50ms additional compilation time
- **Memory overhead**: Zero runtime memory footprint
- **Binary size**: No increase in final binary size

### Portability Requirements
- **Compilers**: GCC 7+, Clang 6+, MSVC 2017+, Intel C++ 19+
- **Platforms**: Windows, Linux, macOS, BSD variants
- **Architectures**: x86, x86_64, ARM, ARM64, basic RISC-V support

---

## 🔄 Error Handling Strategy

### Compile-Time Errors
```cpp
// Static assertions for unsupported configurations
static_assert(TRLC_CPP17, "trlc-platform requires C++17 or later");
static_assert(trlc::platform::getCompilerType() != trlc::platform::CompilerType::unknown, 
              "Unsupported compiler detected");
```

### Runtime Error Handling
```cpp
// Graceful degradation for runtime feature detection
bool feature_available = false;
try {
    feature_available = trlc::platform::hasRuntimeFeature(RuntimeFeature::avx);
} catch (...) {
    // Fall back to safe defaults
    feature_available = false;
}
```

---

## 📚 API Documentation Standards

### Documentation Format
- **Doxygen comments** for all public APIs
- **Usage examples** in header comments  
- **Platform-specific notes** where relevant
- **Performance implications** documented

### Example Documentation
```cpp
/**
 * @brief Detects the current compiler type at compile time.
 * 
 * This function uses compiler-specific predefined macros to identify
 * the compiler being used to build the code. The detection is performed
 * entirely at compile time with zero runtime cost.
 * 
 * @return CompilerType enum value representing the detected compiler
 * 
 * @note Returns CompilerType::unknown for unrecognized compilers
 * @note This is a constexpr function - result is available at compile time
 * 
 * @example
 * @code
 * if constexpr (getCompilerType() == CompilerType::gcc) {
 *     // GCC-specific optimizations
 * }
 * @endcode
 */
constexpr CompilerType getCompilerType() noexcept;
```

---

## 🚀 Future Extensions

### Phase 2 Features
1. **Standard Library Detection** - libstdc++, libc++, MSVC STL detection
2. **Build System Integration** - Deeper CMake, Bazel, Meson integration  
3. **Runtime Environment Detection** - Container, VM, bare metal detection
4. **Performance Profiling Integration** - Built-in benchmark utilities

### Phase 3 Features
1. **Dynamic Feature Detection** - Runtime capability discovery
2. **Platform-Specific APIs** - Win32, POSIX, native API detection
3. **Cross-Compilation Support** - Target vs host platform detection
4. **Hardware Information** - CPU count, memory size, cache hierarchy

---

## ✅ Acceptance Criteria

### For Release 1.0
- [x] All 9 core components implemented and tested
- [x] CMake integration working with major build systems
- [x] Documentation complete with examples
- [x] Cross-platform compatibility verified
- [x] Zero runtime overhead confirmed
- [x] Thread-safe constexpr implementations
- [x] Integration tests passing on CI/CD pipeline

### Quality Gates
- **Code Coverage**: >95% for testable components
- **Static Analysis**: Clean reports from Clang-Tidy, Cppcheck
- **Performance**: <50ms additional compile time
- **Compatibility**: Support matrix verified for all target platforms

This design provides a complete specification for implementing trlc-platform as a robust, efficient, and portable platform abstraction layer.
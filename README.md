# TRLC Platform
![Linux GCC](https://img.shields.io/github/actions/workflow/status/tranglecong/trlc-platform/ci.yml?branch=main&job=Test%20on%20Ubuntu%2022.04%20GCC%2012&label=Linux%20GCC)
![macOS Clang](https://img.shields.io/github/actions/workflow/status/tranglecong/trlc-platform/ci.yml?branch=main&job=Test%20on%20macOS%2013%20Clang&label=macOS%20Clang)
![Windows MSVC](https://img.shields.io/github/actions/workflow/status/tranglecong/trlc-platform/ci.yml?branch=main&job=Test%20on%20Windows%202022%20MSVC%202022&label=Windows%20MSVC)
[![C++ Version](https://img.shields.io/badge/C%2B%2B-17%2F20%2F23-blue)](#requirements)
[![License](https://img.shields.io/badge/license-MIT-green)](LICENSE)
[![Header Only](https://img.shields.io/badge/header--only-yes-success)](#installation)

Modern C++ library for compile-time platform detection and abstraction. Replace fragile `#ifdef` macros with type-safe, zero-overhead API calls.

```cpp
// ❌ Traditional approach
#if defined(__GNUC__) || defined(__clang__)
    result = __builtin_popcount(value);
#elif defined(_MSC_VER)
    result = __popcnt(value);
#endif

// ✅ TRLC Platform approach  
if constexpr (getCompilerType() == CompilerType::gcc) {
    result = __builtin_popcount(value);
} else if constexpr (getCompilerType() == CompilerType::msvc) {
    result = __popcnt(value);
}
```

## Features

- **Zero Runtime Overhead** - All detection happens at compile time using `constexpr`
- **Type-Safe APIs** - Replace error-prone `#ifdef` with modern C++ templates  
- **Comprehensive Detection** - OS, compiler, architecture, CPU features, C++ standard
- **Header-Only** - No compilation required, just include and use
- **Cross-Platform** - Windows, Linux, macOS, BSD on GCC, Clang, MSVC

## Quick Start

### Install
```cpp
// Just include the header
#include "trlc/platform/core.hpp"
```

### Basic Usage
```cpp
#include "trlc/platform/core.hpp"
using namespace trlc::platform;

int main() {
    // Platform detection (compile-time)
    constexpr auto os = getOperatingSystem();
    constexpr auto compiler = getCompilerType();
    constexpr auto arch = getCpuArchitecture();
    
    if constexpr (os == OperatingSystem::windows) {
        // Windows-specific code
    } else if constexpr (os == OperatingSystem::linux_generic) {
        // Linux-specific code  
    }
    
    // Feature detection
    if constexpr (hasFeature<LanguageFeature::exceptions>()) {
        try {
            riskyOperation();
        } catch (...) {
            handleError();
        }
    } else {
        // No exception support - use error codes
        auto result = safeOperation();
    }
    
    // Runtime CPU features
    initializePlatform(); // Call once at startup
    if (hasRuntimeFeature(RuntimeFeature::avx)) {
        // Use AVX optimizations
    }
    
    return 0;
}
```

## Installation

### Header-Only Setup
```bash
git clone https://github.com/tranglecong/trlc-flatform.git
# Add include/ directory to your compiler's include path
g++ -I./trlc-flatform/include your_code.cpp
```

### CMake Integration
```cmake
# Method 1: FetchContent (Recommended)
include(FetchContent)
FetchContent_Declare(
    trlc-platform
    GIT_REPOSITORY https://github.com/tranglecong/trlc-flatform.git
    GIT_TAG main
)
FetchContent_MakeAvailable(trlc-platform)
target_link_libraries(your_target PRIVATE trlc::platform)

# Method 2: Submodule
add_subdirectory(third_party/trlc-platform)
target_link_libraries(your_target PRIVATE trlc::platform)
```

## API Reference

### Core Detection Functions
```cpp
namespace trlc::platform {
    // Platform detection (compile-time)
    constexpr OperatingSystem getOperatingSystem() noexcept;
    constexpr CompilerType getCompilerType() noexcept;
    constexpr CpuArchitecture getCpuArchitecture() noexcept;
    constexpr CppStandard getCppStandard() noexcept;
    constexpr bool isLittleEndian() noexcept;
    constexpr int getPointerSize() noexcept;  // 32 or 64 bits
    
    // Feature detection
    template <LanguageFeature TFeature>
    constexpr bool hasFeature() noexcept;
    
    bool hasRuntimeFeature(RuntimeFeature feature) noexcept; // Runtime
    
    // Platform information
    PlatformReport getPlatformReport() noexcept;
    void initializePlatform() noexcept; // Call once for runtime features
}
```

### Available Enums
```cpp
enum class OperatingSystem {
    windows, linux_generic, macos, freebsd, android, ios, unknown
};

enum class CompilerType {
    gcc, clang, msvc, intel, unknown  
};

enum class CpuArchitecture {
    x86_64, arm_v8_64, arm_v7_32, unknown
};

enum class LanguageFeature {
    exceptions, rtti, threads, atomic_operations, concepts, ranges
};

enum class RuntimeFeature {
    sse, sse2, avx, avx2, neon, hardware_aes
};
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
2. **Review [API Documentation](https://tranglecong.github.io/trlc-platform)** - Comprehensive API reference
3. **Search [GitHub Issues](https://github.com/tranglecong/trlc-platform/issues)** - Known issues and solutions
4. **Create a [New Issue](https://github.com/tranglecong/trlc-platform/issues/new)** - Report bugs or request features

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

For more information, visit our [documentation](https://tranglecong.github.io/trlc-platform) or check out the [examples](examples/).

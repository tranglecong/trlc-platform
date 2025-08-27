# Advanced Template Specializations and Metaprogramming Utilities

This document describes the advanced template specializations and metaprogramming utilities implemented for the TRLC Platform Library in the `traits.hpp` header.

## Overview

The TRLC Platform Library now includes sophisticated template metaprogramming capabilities that extend the basic feature detection system with type-safe, compile-time template specializations and SFINAE-based conditional compilation helpers. These utilities are located in the `trlc::platform::traits` namespace.

## Key Components

### 1. Template Specializations for Feature Detection

#### Enhanced `hasFeature<>()` Templates

The library provides explicit template specializations for all language and runtime features:

```cpp
// Language feature specializations (constexpr)
template <>
constexpr bool hasFeature<LanguageFeature::exceptions>() noexcept {
    return hasExceptions();
}

template <>
constexpr bool hasFeature<LanguageFeature::threads>() noexcept {
    return hasThreads();
}

// Runtime feature specializations (using hasRuntimeFeature template)
bool has_sse = hasRuntimeFeature<RuntimeFeature::sse>();
bool has_avx = hasRuntimeFeature<RuntimeFeature::avx>();
```

#### Benefits:
- **Direct function calls**: Optimized specializations call specific detection functions directly
- **Better optimization**: Compilers can inline and optimize individual feature checks
- **Type safety**: Template parameter ensures only valid features can be queried
- **Compile-time evaluation**: Language features are fully constexpr

### 2. SFINAE-Based Conditional Compilation Helpers

#### Template Aliases for SFINAE

```cpp
// Enable function only if feature is available
template <LanguageFeature TFeature>
using enable_if_feature_t = trlc::platform::traits::enable_if_t<hasFeature<TFeature>()>;

// Enable function only if feature is NOT available
template <LanguageFeature TFeature>
using enable_if_no_feature_t = trlc::platform::traits::enable_if_t<!hasFeature<TFeature>()>;

// Enable function only if ALL features are available
template <LanguageFeature... Features>
using enable_if_all_features_t = trlc::platform::traits::enable_if_t<trlc::platform::traits::hasAllFeatures<Features...>()>;
```

#### Usage Examples:

```cpp
// Function only available if exceptions are enabled
template <typename T>
trlc::platform::traits::enable_if_feature_t<LanguageFeature::exceptions>
safe_function(T value) {
    try {
        return risky_operation(value);
    } catch (...) {
        return default_value<T>();
    }
}

// Alternative implementation when exceptions are disabled
template <typename T>
trlc::platform::traits::enable_if_no_feature_t<LanguageFeature::exceptions>
safe_function(T value) {
    return risky_operation_no_throw(value);
}
```

### 3. Variadic Template Functions

#### Multi-Feature Requirements

```cpp
// Check if ALL specified features are available
template <LanguageFeature... Features>
constexpr bool hasAllFeatures() noexcept {
    return (hasFeature<Features>() && ...);
}

// Check if ANY of the specified features are available
template <LanguageFeature... Features>
constexpr bool hasAnyFeature() noexcept {
    return (hasFeature<Features>() || ...);
}

// Count how many features are available
template <LanguageFeature... Features>
constexpr std::size_t countAvailableFeatures() noexcept {
    return (static_cast<std::size_t>(hasFeature<Features>()) + ...);
}
```

#### Usage Examples:

```cpp
// Require multiple features for advanced functionality
if constexpr (trlc::platform::traits::hasAllFeatures<LanguageFeature::exceptions,
                             LanguageFeature::threads,
                             LanguageFeature::atomic_operations>()) {
    // Use advanced implementation
    return advanced_parallel_algorithm();
} else {
    // Fall back to simple implementation  
    return simple_sequential_algorithm();
}
```

### 4. Type Traits for Metaprogramming

#### Compile-Time Feature Traits

```cpp
// Type trait for feature availability
template <LanguageFeature TFeature>
struct FeatureAvailable : trlc::platform::traits::bool_constant<hasFeature<TFeature>()> {};

// Variable template (C++14+)
template <LanguageFeature TFeature>
constexpr bool feature_available_v = FeatureAvailable<TFeature>::value;

// Multi-feature traits
template <LanguageFeature... Features>
struct AllFeaturesAvailable : trlc::platform::traits::bool_constant<trlc::platform::traits::hasAllFeatures<Features...>()> {};

template <LanguageFeature... Features>
constexpr bool all_features_available_v = AllFeaturesAvailable<Features...>::value;
```

#### Usage in Template Metaprogramming:

```cpp
// Conditional type selection based on feature availability
template <typename T>
using SafeContainer = std::conditional_t<
    trlc::platform::traits::feature_available_v<LanguageFeature::exceptions>,
    std::vector<T>,      // Use vector with exception safety
    std::array<T, 1024>  // Use fixed array without exceptions
>;
```

### 5. Platform-Specific Template Utilities

#### Platform Detection Templates

```cpp
// Platform tag types for template specialization
struct WindowsTag {};
struct LinuxTag {};
struct MacOsTag {};

// Template functions for platform detection
template <typename PlatformTag>
constexpr bool is_platform() noexcept;

// Usage
if constexpr (is_platform<LinuxTag>()) {
    // Linux-specific code
} else if constexpr (is_platform<WindowsTag>()) {
    // Windows-specific code
}
```

#### Compiler-Specific Templates

```cpp
struct GccTag {};
struct ClangTag {};
struct MsvcTag {};

template <typename CompilerTag>
constexpr bool is_compiler() noexcept;

// Usage for compiler-specific optimizations
if constexpr (is_compiler<GccTag>()) {
    // GCC-specific intrinsics
    return __builtin_popcount(value);
} else if constexpr (is_compiler<MsvcTag>()) {
    // MSVC-specific intrinsics
    return __popcnt(value);
}
```

### 6. Optimal Type Selection

#### Platform-Optimized Types

```cpp
// Optimal type selection for current platform
template <typename DataType>
struct OptimalType {
    using type = DataType; // Default: no change
};

// Specialization for pointer-sized integers
template <>
struct OptimalType<void*> {
    using type = std::conditional_t<
        getCpuArchitecture() == CpuArchitecture::x86_64 ||
        getCpuArchitecture() == CpuArchitecture::arm_v8_64,
        std::uint64_t, std::uint32_t>;
};

template <typename DataType>
using optimal_type_t = typename OptimalType<DataType>::type;
```

#### Cache-Line Aligned Types

```cpp
// Template wrapper for cache-line alignment
template <typename DataType>
struct TemplateCacheLineAligned {
    static constexpr std::size_t alignment = 64;
    alignas(alignment) DataType data;
    
    // Convenient access methods
    constexpr DataType& get() noexcept { return data; }
    constexpr const DataType& get() const noexcept { return data; }
};

// Usage
TemplateCacheLineAligned<int> cache_aligned_counter{0};
cache_aligned_counter.get()++;
```

## Convenience Macros

### Feature-Based Macros

```cpp
// Test specific language features
#define TRLC_HAS_LANGUAGE_FEATURE(feature) \
    (trlc::platform::hasFeature<trlc::platform::LanguageFeature::feature>())

// Test specific runtime features
#define TRLC_HAS_RUNTIME_FEATURE(feature) \
    (trlc::platform::hasFeature<trlc::platform::RuntimeFeature::feature>())

// Conditional compilation based on features
#define TRLC_IF_LANGUAGE_FEATURE(feature) \
    if constexpr (trlc::platform::hasFeature<trlc::platform::LanguageFeature::feature>())

// Template requirement macros
#define TRLC_REQUIRE_FEATURE(feature) \
    template <typename = trlc::platform::enable_if_feature_t<trlc::platform::LanguageFeature::feature>>

#define TRLC_REQUIRE_NO_FEATURE(feature) \
    template <typename = trlc::platform::enable_if_no_feature_t<trlc::platform::LanguageFeature::feature>>
```

### Platform-Specific Macros

```cpp
// Test multiple feature requirements
#define TRLC_HAS_ALL_FEATURES(...) \
    (trlc::platform::hasAllFeatures<__VA_ARGS__>())

#define TRLC_HAS_ANY_FEATURES(...) \
    (trlc::platform::hasAnyFeature<__VA_ARGS__>())
```

## Advanced Usage Examples

### 1. Feature-Dependent Algorithm Selection

```cpp
template <typename Iterator, typename T>
auto optimized_find(Iterator first, Iterator last, const T& value) {
    if constexpr (hasAllFeatures<LanguageFeature::vector_intrinsics,
                                 LanguageFeature::atomic_operations>()) {
        // Use SIMD-optimized parallel search
        return simd_parallel_find(first, last, value);
    } else if constexpr (hasFeature<LanguageFeature::threads>()) {
        // Use multi-threaded search without SIMD
        return threaded_find(first, last, value);
    } else {
        // Fall back to standard library
        return std::find(first, last, value);
    }
}
```

### 2. Platform-Specific Memory Management

```cpp
template <typename T>
class PlatformOptimizedAllocator {
private:
    static constexpr std::size_t get_alignment() {
        if constexpr (is_architecture<X86_64Tag>()) {
            return 64; // Cache line aligned for x86_64
        } else if constexpr (is_architecture<ArmTag>()) {
            return 32; // Different alignment for ARM
        } else {
            return alignof(T);
        }
    }

public:
    using value_type = T;
    static constexpr std::size_t alignment = get_alignment();
    
    T* allocate(std::size_t n) {
        if constexpr (is_platform<WindowsTag>()) {
            return static_cast<T*>(_aligned_malloc(n * sizeof(T), alignment));
        } else if constexpr (is_platform<LinuxTag>()) {
            return static_cast<T*>(aligned_alloc(alignment, n * sizeof(T)));
        } else {
            return std::allocator<T>{}.allocate(n);
        }
    }
};
```

### 3. Conditional API Exposure

```cpp
class AdvancedPlatformAPI {
public:
    // Basic API always available
    void basic_operation();
    
    // Advanced APIs only available with specific features
    TRLC_REQUIRE_FEATURE(exceptions)
    void exception_safe_operation() {
        try {
            risky_operation();
        } catch (...) {
            handle_error();
        }
    }
    
    TRLC_REQUIRE_FEATURE(threads)
    auto async_operation() -> std::future<int> {
        return std::async(std::launch::async, []() {
            return compute_result();
        });
    }
    
    // Multi-feature requirements
    template <typename = enable_if_all_features_t<
        LanguageFeature::exceptions,
        LanguageFeature::threads,
        LanguageFeature::atomic_operations>>
    void advanced_parallel_operation() {
        // Implementation requiring all three features
    }
};
```

## Performance Characteristics

### Compile-Time Benefits

1. **Zero Runtime Overhead**: All template selections happen at compile time
2. **Optimal Code Generation**: Direct function calls instead of virtual dispatch
3. **Dead Code Elimination**: Unused feature paths are completely removed
4. **Constexpr Evaluation**: Language features evaluated at compile time

### Memory Benefits

1. **Cache-Line Alignment**: Template wrappers ensure optimal memory layout
2. **Platform-Specific Optimization**: Types sized optimally for target architecture
3. **Minimal Template Instantiation**: SFINAE prevents invalid instantiations

## C++ Standard Compatibility

### C++17 Minimum Requirements

- `constexpr` functions for compile-time evaluation
- `if constexpr` for conditional compilation
- Variadic template expansion with fold expressions
- SFINAE with `std::enable_if`

### C++20 Enhanced Features

When C++20 is available, additional concepts can be defined:

```cpp
#if __cplusplus >= 202002L
template <typename PlatformTag>
concept PlatformTagType = std::is_same_v<PlatformTag, WindowsTag> ||
                         std::is_same_v<PlatformTag, LinuxTag> ||
                         std::is_same_v<PlatformTag, MacOsTag>;

template <PlatformTagType PlatformTag>
constexpr bool is_platform_concept() noexcept {
    return is_platform<PlatformTag>();
}
#endif
```

## Integration with Existing System

The template utilities integrate seamlessly with the existing TRLC Platform Library:

1. **Header Dependencies**: `traits.hpp` depends only on core headers
2. **No Breaking Changes**: All existing APIs remain unchanged
3. **Opt-In Usage**: Template utilities are additive, not replacing existing functions
4. **Test Coverage**: Comprehensive test suite validates all template functionality

## Best Practices

1. **Use `constexpr` templates for language features**: Enables compile-time optimization
2. **Prefer SFINAE over runtime checks**: Better performance and type safety
3. **Combine features judiciously**: Don't over-constrain template requirements
4. **Document template requirements**: Make feature dependencies clear
5. **Test on target platforms**: Verify template instantiations work correctly

## Future Extensions

The template system is designed for extensibility:

1. **Additional Platform Tags**: Easy to add new platform-specific specializations
2. **Feature Categories**: Group related features for easier template constraints
3. **Optimization Hints**: Template parameters could guide code generation
4. **Plugin Architecture**: Feature-based template selection for optional components

#pragma once

#include <cstddef>      // For size_t
#include <type_traits>  // For standard type traits

// Forward declarations to avoid circular dependencies
namespace trlc {
namespace platform {
namespace detail {
// detectCacheLineSize() is defined in architecture.hpp
constexpr size_t detectPageSize() noexcept;
}  // namespace detail
}  // namespace platform
}  // namespace trlc

/**
 * @file typeinfo.hpp
 * @brief Compile-time type and alignment information utilities
 *
 * This header provides comprehensive type introspection and memory layout utilities
 * for C++ types. It includes compile-time type size and alignment queries, padding
 * analysis, cache line and page alignment helpers, and type verification utilities.
 *
 * Features:
 * - Compile-time type size and alignment information
 * - System cache line and page size detection
 * - Type trait extensions with standard layout analysis
 * - Padding calculation and internal padding detection
 * - Alignment helper types for cache lines and pages
 * - Type verification functions for expected layouts
 * - Comprehensive macro interface for common operations
 *
 * All utilities are designed to work at compile time when possible, providing
 * zero runtime overhead for type introspection and memory layout analysis.
 *
 * @author TRLC Platform Team
 * @version 1.0.0
 */

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace trlc {
namespace platform {

//==============================================================================
// Forward Declarations
//==============================================================================

namespace detail {
// Helper functions for compile-time calculations
template <typename Type>
constexpr size_t calculateTypePadding() noexcept;

template <typename Type>
constexpr bool hasTypePadding() noexcept;

// Platform-specific page size detection (cache line size is in architecture.hpp)
constexpr size_t detectPageSize() noexcept;
}  // namespace detail

//==============================================================================
// Core Type Information Functions
//==============================================================================

/**
 * @brief Get the size of a type at compile time
 *
 * This function template provides a constexpr interface to sizeof().
 * While equivalent to sizeof(Type), it's provided for API consistency
 * and potential future extensions.
 *
 * @tparam Type The type to query
 * @return The size of Type in bytes
 *
 * @example
 * @code
 * constexpr auto int_size = getTypeSize<int>();  // Usually 4
 * constexpr auto ptr_size = getTypeSize<void*>(); // 8 on 64-bit systems
 * @endcode
 */
template <typename Type>
constexpr size_t getTypeSize() noexcept {
    return sizeof(Type);
}

/**
 * @brief Get the alignment requirement of a type at compile time
 *
 * This function template provides a constexpr interface to alignof().
 * The alignment is the strictest alignment requirement for the type.
 *
 * @tparam Type The type to query
 * @return The alignment requirement of Type in bytes
 *
 * @example
 * @code
 * constexpr auto char_align = getTypeAlignment<char>();    // Usually 1
 * constexpr auto double_align = getTypeAlignment<double>(); // Usually 8
 * @endcode
 */
template <typename Type>
constexpr size_t getTypeAlignment() noexcept {
    return alignof(Type);
}

/**
 * @brief Get the cache line size for the current platform
 *
 * Returns the cache line size in bytes. This is used for optimizing
 * memory access patterns and avoiding false sharing in concurrent code.
 *
 * @return Cache line size in bytes (typically 64 bytes)
 *
 * @note This function attempts to detect the actual cache line size
 *       but falls back to common defaults if detection is not possible
 */
constexpr size_t getCacheLineSize() noexcept {
    // Inline implementation to avoid circular dependency with architecture.hpp
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    return 64;  // x86/x64 typical cache line size
#elif defined(__aarch64__) || defined(_M_ARM64) || defined(__arm__) || defined(_M_ARM)
    return 64;  // ARM typical cache line size
#elif defined(__powerpc__) || defined(__ppc__) || defined(__PPC__)
    return 128;  // PowerPC typical cache line size
#else
    return 64;  // Conservative default
#endif
}

/**
 * @brief Get the page size for the current platform
 *
 * Returns the memory page size in bytes. This is useful for memory
 * allocation optimizations and system-level memory management.
 *
 * @return Page size in bytes (typically 4096 bytes)
 *
 * @note This function attempts to detect the actual page size
 *       but falls back to common defaults if detection is not possible
 */
constexpr size_t getPageSize() noexcept {
    return detail::detectPageSize();
}

//==============================================================================
// Type Information Structure
//==============================================================================

/**
 * @brief Comprehensive type information structure
 *
 * This template provides detailed compile-time information about a type,
 * including size, alignment, and various type trait checks that are
 * commonly needed for optimized memory layout and type safety.
 *
 * @tparam Type The type to analyze
 *
 * @example
 * @code
 * using info = TypeInfo<MyStruct>;
 * static_assert(info::size == sizeof(MyStruct));
 * static_assert(info::is_trivially_copyable);
 * @endcode
 */
template <typename Type>
struct TypeInfo {
    /// Size of the type in bytes
    static constexpr size_t size = sizeof(Type);

    /// Alignment requirement of the type in bytes
    static constexpr size_t alignment = alignof(Type);

    /// True if the type's alignment is at least as strict as cache line alignment
    static constexpr bool is_cache_line_aligned = (alignment >= getCacheLineSize());

    /// True if the type is trivially copyable (can use memcpy)
    static constexpr bool is_trivially_copyable = std::is_trivially_copyable_v<Type>;

    /// True if the type has standard layout (C-compatible layout)
    static constexpr bool is_standard_layout = std::is_standard_layout_v<Type>;

    /// True if the type is POD (Plain Old Data)
    static constexpr bool is_pod = is_trivially_copyable && is_standard_layout;

    /// True if the type is empty (size == 1 but no data members)
    static constexpr bool is_empty = std::is_empty_v<Type>;

    /// True if the type is a fundamental type
    static constexpr bool is_fundamental = std::is_fundamental_v<Type>;

    /// True if the type is an integral type
    static constexpr bool is_integral = std::is_integral_v<Type>;

    /// True if the type is a floating point type
    static constexpr bool is_floating_point = std::is_floating_point_v<Type>;

    /// True if the type is a pointer type
    static constexpr bool is_pointer = std::is_pointer_v<Type>;

    /// True if the type is an array type
    static constexpr bool is_array = std::is_array_v<Type>;
};

//==============================================================================
// Padding Analysis Functions
//==============================================================================

/**
 * @brief Calculate the internal padding of a type
 *
 * Computes the number of padding bytes within a type's layout.
 * This includes padding between members and trailing padding.
 *
 * @tparam Type The type to analyze
 * @return Number of padding bytes within the type
 *
 * @note For fundamental types, this typically returns 0
 * @note For structs/classes, this calculates the difference between
 *       the sum of member sizes and the actual struct size
 *
 * @example
 * @code
 * struct Example { char a; int b; };  // Likely has 3 bytes padding
 * constexpr auto padding = calculatePadding<Example>();
 * @endcode
 */
template <typename Type>
constexpr size_t calculatePadding() noexcept {
    return detail::calculateTypePadding<Type>();
}

/**
 * @brief Check if a type has internal padding
 *
 * Determines whether a type contains any padding bytes in its layout.
 * This is useful for understanding memory efficiency and serialization
 * considerations.
 *
 * @tparam Type The type to check
 * @return True if the type contains padding bytes
 *
 * @example
 * @code
 * struct Packed { char a; char b; };      // No padding
 * struct Padded { char a; int b; };       // Has padding
 *
 * static_assert(!hasInternalPadding<Packed>());
 * static_assert(hasInternalPadding<Padded>());
 * @endcode
 */
template <typename Type>
constexpr bool hasInternalPadding() noexcept {
    return detail::hasTypePadding<Type>();
}

//==============================================================================
// Alignment Helper Types
//==============================================================================

/**
 * @brief Aligned storage type with specified alignment
 *
 * Provides storage with a specific alignment requirement. The storage
 * is large enough to hold the alignment value and properly aligned.
 *
 * @tparam TAlignment The required alignment in bytes
 *
 * @example
 * @code
 * AlignedType<32> aligned_storage;  // 32-byte aligned storage
 * @endcode
 */
template <size_t TAlignment>
struct AlignedType {
    static_assert(TAlignment > 0, "Alignment must be greater than zero");
    static_assert((TAlignment & (TAlignment - 1)) == 0, "Alignment must be a power of two");

    /// The aligned storage data
    alignas(TAlignment) char data[TAlignment];

    /// Get a pointer to the aligned storage
    void* get() noexcept { return data; }

    /// Get a const pointer to the aligned storage
    const void* get() const noexcept { return data; }

    /// Get a typed pointer to the aligned storage
    template <typename T>
    T* as() noexcept {
        static_assert(alignof(T) <= TAlignment, "Type alignment exceeds storage alignment");
        return reinterpret_cast<T*>(data);
    }

    /// Get a const typed pointer to the aligned storage
    template <typename T>
    const T* as() const noexcept {
        static_assert(alignof(T) <= TAlignment, "Type alignment exceeds storage alignment");
        return reinterpret_cast<const T*>(data);
    }
};

/**
 * @brief Cache line aligned storage type
 *
 * Provides storage aligned to cache line boundaries. This is useful
 * for avoiding false sharing in concurrent code and optimizing
 * memory access patterns.
 *
 * @example
 * @code
 * CacheLineAligned cache_aligned_data;
 * auto* ptr = cache_aligned_data.as<MyType>();
 * @endcode
 */
using CacheLineAligned = AlignedType<64>;  // Most common cache line size

/**
 * @brief Page aligned storage type
 *
 * Provides storage aligned to page boundaries. This is useful for
 * memory mapping operations and system-level memory management.
 *
 * @example
 * @code
 * PageAligned page_aligned_data;
 * auto* ptr = page_aligned_data.as<MyType>();
 * @endcode
 */
using PageAligned = AlignedType<4096>;  // Most common page size

//==============================================================================
// Memory Layout Verification Functions
//==============================================================================

/**
 * @brief Verify that a type has the expected size
 *
 * Compile-time verification that a type's size matches expectations.
 * This is useful for ensuring consistent memory layouts across platforms
 * and compiler versions.
 *
 * @tparam Type The type to verify
 * @tparam TExpectedSize The expected size in bytes
 * @return True if the type size matches the expected size
 *
 * @example
 * @code
 * static_assert(verifyTypeSize<int, 4>());     // Verify int is 4 bytes
 * static_assert(verifyTypeSize<void*, 8>());   // Verify pointer is 8 bytes (64-bit)
 * @endcode
 */
template <typename Type, size_t TExpectedSize>
constexpr bool verifyTypeSize() noexcept {
    return sizeof(Type) == TExpectedSize;
}

/**
 * @brief Verify that a type has the expected alignment
 *
 * Compile-time verification that a type's alignment matches expectations.
 * This is useful for ensuring optimal memory layout and performance.
 *
 * @tparam Type The type to verify
 * @tparam TExpectedAlignment The expected alignment in bytes
 * @return True if the type alignment matches the expected alignment
 *
 * @example
 * @code
 * static_assert(verifyTypeAlignment<double, 8>());    // Verify double is 8-byte aligned
 * static_assert(verifyTypeAlignment<char, 1>());      // Verify char is 1-byte aligned
 * @endcode
 */
template <typename Type, size_t TExpectedAlignment>
constexpr bool verifyTypeAlignment() noexcept {
    return alignof(Type) == TExpectedAlignment;
}

/**
 * @brief Verify that a type is suitably aligned for cache line optimization
 *
 * Checks whether a type's alignment is sufficient for cache line optimization.
 *
 * @tparam Type The type to verify
 * @return True if the type is cache line aligned
 */
template <typename Type>
constexpr bool isCacheLineAligned() noexcept {
    return alignof(Type) >= getCacheLineSize();
}

/**
 * @brief Verify that a type is suitably aligned for page boundaries
 *
 * Checks whether a type's alignment is sufficient for page boundary alignment.
 *
 * @tparam Type The type to verify
 * @return True if the type is page aligned
 */
template <typename Type>
constexpr bool isPageAligned() noexcept {
    return alignof(Type) >= getPageSize();
}

//==============================================================================
// Size and Alignment Calculation Utilities
//==============================================================================

/**
 * @brief Calculate the aligned size for a given alignment
 *
 * Rounds up a size to the next boundary that satisfies the alignment requirement.
 *
 * @param size The original size
 * @param alignment The required alignment (must be power of 2)
 * @return The aligned size
 */
constexpr size_t alignedSize(size_t size, size_t alignment) noexcept {
    return (size + alignment - 1) & ~(alignment - 1);
}

/**
 * @brief Calculate the aligned address for a given alignment
 *
 * Rounds up an address to the next boundary that satisfies the alignment requirement.
 *
 * @param addr The original address
 * @param alignment The required alignment (must be power of 2)
 * @return The aligned address
 */
constexpr uintptr_t alignedAddress(uintptr_t addr, size_t alignment) noexcept {
    return (addr + alignment - 1) & ~(alignment - 1);
}

/**
 * @brief Check if an address is aligned to the specified boundary
 *
 * @param addr The address to check
 * @param alignment The alignment boundary
 * @return True if the address is properly aligned
 */
constexpr bool isAligned(uintptr_t addr, size_t alignment) noexcept {
    return (addr & (alignment - 1)) == 0;
}

/**
 * @brief Check if a pointer is aligned to the specified boundary
 *
 * @param ptr The pointer to check
 * @param alignment The alignment boundary
 * @return True if the pointer is properly aligned
 *
 * @note This function is not constexpr due to reinterpret_cast limitations
 */
inline bool isAligned(const void* ptr, size_t alignment) noexcept {
    return isAligned(reinterpret_cast<uintptr_t>(ptr), alignment);
}

//==============================================================================
// Implementation Details
//==============================================================================

namespace detail {

/**
 * @brief Calculate padding for a type (implementation detail)
 *
 * This is a simplified implementation that works for fundamental types
 * and some simple cases. For complex types, the calculation may be
 * approximate due to the difficulty of determining exact member layouts
 * at compile time without reflection.
 */
template <typename Type>
constexpr size_t calculateTypePadding() noexcept {
    if constexpr (std::is_fundamental_v<Type> || std::is_pointer_v<Type>) {
        // Fundamental types and pointers typically have no internal padding
        return 0;
    } else if constexpr (std::is_empty_v<Type>) {
        // Empty types have no padding
        return 0;
    } else {
        // For other types, we can only estimate
        // The actual padding calculation would require reflection or compiler intrinsics
        // For now, we assume minimal padding based on alignment
        constexpr size_t type_size = sizeof(Type);
        constexpr size_t type_alignment = alignof(Type);

        // If size is not a multiple of alignment, there's likely trailing padding
        return (type_alignment - (type_size % type_alignment)) % type_alignment;
    }
}

/**
 * @brief Check if a type has padding (implementation detail)
 */
template <typename Type>
constexpr bool hasTypePadding() noexcept {
    return calculateTypePadding<Type>() > 0;
}

// Note: detectCacheLineSize() is defined in architecture.hpp to avoid duplication

/**
 * @brief Detect page size (implementation detail)
 *
 * Attempts to detect the actual page size using platform-specific methods.
 * Falls back to common defaults.
 */
constexpr size_t detectPageSize() noexcept {
    // Platform-specific detection
#if defined(_WIN32) || defined(_WIN64)
    // Windows typically uses 4KB pages (though large pages are possible)
    return 4096;
#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__) || \
    defined(__OpenBSD__)
    // Most Unix-like systems use 4KB pages by default
    return 4096;
#elif defined(__sparc__)
    // SPARC systems often use 8KB pages
    return 8192;
#elif defined(__ia64__)
    // Itanium systems often use 16KB pages
    return 16384;
#else
    // Conservative default for unknown platforms
    return 4096;
#endif
}

}  // namespace detail

}  // namespace platform
}  // namespace trlc

//==============================================================================
// Utility Macros
//==============================================================================

/**
 * @brief Get the alignment of a type
 *
 * This macro provides a convenient way to get type alignment.
 * It's equivalent to alignof(type) but matches the naming convention.
 */
#define TRLC_ALIGNOF(type) (alignof(type))

/**
 * @brief Get the size of a type
 *
 * This macro provides a convenient way to get type size.
 * It's equivalent to sizeof(type) but matches the naming convention.
 */
#define TRLC_SIZEOF(type) (sizeof(type))

/**
 * @brief Get the cache line size for the current platform
 *
 * This macro provides compile-time access to the cache line size.
 */
#define TRLC_CACHE_LINE_SIZE (trlc::platform::getCacheLineSize())

/**
 * @brief Get the page size for the current platform
 *
 * This macro provides compile-time access to the page size.
 */
#define TRLC_PAGE_SIZE (trlc::platform::getPageSize())

/**
 * @brief Align a type to cache line boundaries
 *
 * This macro can be used to align variables, struct members, or entire
 * types to cache line boundaries for optimal performance.
 *
 * @example
 * @code
 * struct TRLC_ALIGN_TO_CACHE_LINE MyStruct {
 *     // This struct will be cache-line aligned
 * };
 *
 * TRLC_ALIGN_TO_CACHE_LINE int my_var;  // Cache-line aligned variable
 * @endcode
 */
#define TRLC_ALIGN_TO_CACHE_LINE alignas(trlc::platform::getCacheLineSize())

/**
 * @brief Align a type to page boundaries
 *
 * This macro can be used to align variables, struct members, or entire
 * types to page boundaries for memory management optimizations.
 */
#define TRLC_ALIGN_TO_PAGE alignas(trlc::platform::getPageSize())

/**
 * @brief Create a cache-line aligned variable
 *
 * This macro declares a variable that is aligned to cache line boundaries.
 *
 * @param type The type of the variable
 * @param name The name of the variable
 */
// Note: TRLC_CACHE_ALIGNED is defined in macros.hpp

/**
 * @brief Create a page-aligned variable
 *
 * This macro declares a variable that is aligned to page boundaries.
 *
 * @param type The type of the variable
 * @param name The name of the variable
 */
#define TRLC_PAGE_ALIGNED(type, name) TRLC_ALIGN_TO_PAGE type name

//==============================================================================
// Static Assertions for Compile-Time Validation
//==============================================================================

// Verify that our alignment helpers work correctly
static_assert(alignof(trlc::platform::CacheLineAligned) >= 64,
              "CacheLineAligned should be at least 64-byte aligned");
static_assert(alignof(trlc::platform::PageAligned) >= 4096,
              "PageAligned should be at least 4096-byte aligned");

// Verify that our size functions work
static_assert(trlc::platform::getTypeSize<char>() == 1, "char should be 1 byte");
static_assert(trlc::platform::getTypeAlignment<char>() == 1, "char should be 1-byte aligned");

// Verify that our verification functions work
static_assert(trlc::platform::verifyTypeSize<char, 1>(), "Type size verification should work");
static_assert(trlc::platform::verifyTypeAlignment<char, 1>(),
              "Type alignment verification should work");

// Verify TypeInfo works correctly
static_assert(trlc::platform::TypeInfo<int>::size == sizeof(int),
              "TypeInfo size should match sizeof");
static_assert(trlc::platform::TypeInfo<int>::alignment == alignof(int),
              "TypeInfo alignment should match alignof");
static_assert(trlc::platform::TypeInfo<int>::is_fundamental, "int should be fundamental");
static_assert(trlc::platform::TypeInfo<int>::is_integral, "int should be integral");
static_assert(!trlc::platform::TypeInfo<int>::is_floating_point,
              "int should not be floating point");

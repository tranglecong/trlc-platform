#pragma once

/**
 * @file endianness.hpp
 * @brief Byte order detection and utilities for cross-platform development
 *
 * This header provides comprehensive endianness detection and byte manipulation
 * utilities. It includes compile-time byte order detection, efficient byte swapping
 * functions with compiler intrinsics, and network byte order conversion utilities.
 *
 * Features:
 * - Compile-time endianness detection with runtime fallbacks
 * - Efficient byte swapping using compiler intrinsics
 * - Network/host byte order conversion functions
 * - Template-based generic byte manipulation utilities
 * - Comprehensive macro interface for easy usage
 *
 * @author TRLC Platform Team
 * @version 1.0.0
 */

#include <cstdint>
#include <type_traits>

namespace trlc {
namespace platform {

/**
 * @brief Byte order enumeration
 *
 * Represents different byte ordering schemes used by CPU architectures.
 */
enum class ByteOrder : int {
    unknown = 0,    ///< Unknown byte order
    little_endian,  ///< Little-endian (LSB first)
    big_endian,     ///< Big-endian (MSB first)
    mixed_endian    ///< Mixed endianness (rare architectures)
};

/**
 * @brief Comprehensive endianness information structure
 *
 * Provides complete information about the system's byte order including
 * convenient boolean flags and utility methods for byte order operations.
 */
struct EndiannessInfo {
    ByteOrder byte_order;   ///< The detected byte order
    bool is_little_endian;  ///< True if system is little-endian
    bool is_big_endian;     ///< True if system is big-endian

    /**
     * @brief Check if given byte order matches native order
     * @param order Byte order to check
     * @return true if order matches native byte order
     */
    constexpr bool isNativeOrder(ByteOrder order) const noexcept { return byte_order == order; }

    /**
     * @brief Check if byte swapping is needed for target order
     * @param target_order Target byte order
     * @return true if byte swapping is required
     */
    constexpr bool needsByteSwap(ByteOrder target_order) const noexcept {
        if (byte_order == ByteOrder::unknown || target_order == ByteOrder::unknown) {
            return false;  // Conservative: assume no swap needed for unknown
        }
        return byte_order != target_order;
    }
};

namespace detail {

/**
 * @brief Compile-time endianness detection using preprocessor macros
 *
 * This function attempts to determine byte order at compile time using
 * standard preprocessor macros. Falls back to runtime detection if needed.
 */
constexpr ByteOrder detectByteOrderCompileTime() noexcept {
// Try standard endianness macros first
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return ByteOrder::little_endian;
    #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return ByteOrder::big_endian;
    #else
    return ByteOrder::mixed_endian;
    #endif

// Try BSD-style macros
#elif defined(BYTE_ORDER) && defined(LITTLE_ENDIAN) && defined(BIG_ENDIAN)
    #if BYTE_ORDER == LITTLE_ENDIAN
    return ByteOrder::little_endian;
    #elif BYTE_ORDER == BIG_ENDIAN
    return ByteOrder::big_endian;
    #else
    return ByteOrder::mixed_endian;
    #endif

// Try Windows-specific detection
#elif defined(_WIN32) || defined(_WIN64)
    // Windows is always little-endian on supported architectures
    return ByteOrder::little_endian;

// Try architecture-specific detection
#elif defined(__LITTLE_ENDIAN__) || defined(__ARMEL__)
    return ByteOrder::little_endian;
#elif defined(__BIG_ENDIAN__) || defined(__ARMEB__)
    return ByteOrder::big_endian;

// Architecture-specific knowledge
#elif defined(__i386__) || defined(__x86_64__) || defined(__amd64__) || defined(_M_IX86) || \
    defined(_M_X64) || defined(_M_AMD64) || defined(__aarch64__) || defined(_M_ARM64)
    // x86, x86_64, and ARM64 are little-endian
    return ByteOrder::little_endian;

#elif defined(__sparc__) || defined(__sparc) || defined(__MIPSEB__) || defined(_POWER) || \
    defined(__powerpc__) && defined(__BIG_ENDIAN__)
    // SPARC, big-endian MIPS, PowerPC in big-endian mode
    return ByteOrder::big_endian;

#else
    // Unable to determine at compile time
    return ByteOrder::unknown;
#endif
}

/**
 * @brief Runtime endianness detection using union method
 *
 * This function performs runtime byte order detection when compile-time
 * detection is not possible or returns unknown.
 */
inline ByteOrder detectByteOrderRuntime() noexcept {
    union {
        uint32_t integer;
        uint8_t bytes[4];
    } test = {0x01020304};

    if (test.bytes[0] == 0x01) {
        return ByteOrder::big_endian;
    } else if (test.bytes[0] == 0x04) {
        return ByteOrder::little_endian;
    } else {
        return ByteOrder::mixed_endian;
    }
}

}  // namespace detail

/**
 * @brief Get the system's byte order
 *
 * Returns the byte order of the current system. Uses compile-time detection
 * when possible, falls back to runtime detection for unknown cases.
 *
 * @return ByteOrder enum value representing the system's byte order
 */
constexpr ByteOrder getByteOrder() noexcept {
    constexpr ByteOrder compile_time_order = detail::detectByteOrderCompileTime();

    if constexpr (compile_time_order != ByteOrder::unknown) {
        return compile_time_order;
    } else {
        // Note: This branch won't be constexpr but allows fallback
        return detail::detectByteOrderRuntime();
    }
}

/**
 * @brief Get comprehensive endianness information
 *
 * Returns a complete EndiannessInfo structure with byte order and
 * convenient boolean flags.
 *
 * @return EndiannessInfo structure with complete endianness data
 */
constexpr EndiannessInfo getEndiannessInfo() noexcept {
    constexpr ByteOrder order = getByteOrder();
    return EndiannessInfo{order, order == ByteOrder::little_endian, order == ByteOrder::big_endian};
}

/**
 * @brief Check if system is little-endian
 *
 * @return true if system uses little-endian byte order
 */
constexpr bool isLittleEndian() noexcept {
    return getByteOrder() == ByteOrder::little_endian;
}

/**
 * @brief Check if system is big-endian
 *
 * @return true if system uses big-endian byte order
 */
constexpr bool isBigEndian() noexcept {
    return getByteOrder() == ByteOrder::big_endian;
}

// =============================================================================
// Byte Swapping Utilities
// =============================================================================

namespace detail {

/**
 * @brief 16-bit byte swap implementation
 *
 * Uses compiler intrinsics when available, falls back to manual implementation.
 */
constexpr uint16_t byteSwap16Impl(uint16_t value) noexcept {
#if defined(_MSC_VER)
    // MSVC has _byteswap_ushort but it's not constexpr
    // Use manual implementation for constexpr compatibility
    return static_cast<uint16_t>((value << 8) | (value >> 8));
#elif defined(__GNUC__) || defined(__clang__)
    // GCC/Clang builtin (available as constexpr in recent versions)
    #if __has_builtin(__builtin_bswap16)
    return __builtin_bswap16(value);
    #else
    return static_cast<uint16_t>((value << 8) | (value >> 8));
    #endif
#else
    // Manual implementation for unknown compilers
    return static_cast<uint16_t>((value << 8) | (value >> 8));
#endif
}

/**
 * @brief 32-bit byte swap implementation
 */
constexpr uint32_t byteSwap32Impl(uint32_t value) noexcept {
#if defined(_MSC_VER)
    // Manual implementation for constexpr compatibility
    return ((value & 0xFF000000) >> 24) | ((value & 0x00FF0000) >> 8) |
           ((value & 0x0000FF00) << 8) | ((value & 0x000000FF) << 24);
#elif defined(__GNUC__) || defined(__clang__)
    #if __has_builtin(__builtin_bswap32)
    return __builtin_bswap32(value);
    #else
    return ((value & 0xFF000000) >> 24) | ((value & 0x00FF0000) >> 8) |
           ((value & 0x0000FF00) << 8) | ((value & 0x000000FF) << 24);
    #endif
#else
    return ((value & 0xFF000000) >> 24) | ((value & 0x00FF0000) >> 8) |
           ((value & 0x0000FF00) << 8) | ((value & 0x000000FF) << 24);
#endif
}

/**
 * @brief 64-bit byte swap implementation
 */
constexpr uint64_t byteSwap64Impl(uint64_t value) noexcept {
#if defined(_MSC_VER)
    // Manual implementation for constexpr compatibility
    return ((value & 0xFF00000000000000ULL) >> 56) | ((value & 0x00FF000000000000ULL) >> 40) |
           ((value & 0x0000FF0000000000ULL) >> 24) | ((value & 0x000000FF00000000ULL) >> 8) |
           ((value & 0x00000000FF000000ULL) << 8) | ((value & 0x0000000000FF0000ULL) << 24) |
           ((value & 0x000000000000FF00ULL) << 40) | ((value & 0x00000000000000FFULL) << 56);
#elif defined(__GNUC__) || defined(__clang__)
    #if __has_builtin(__builtin_bswap64)
    return __builtin_bswap64(value);
    #else
    return ((value & 0xFF00000000000000ULL) >> 56) | ((value & 0x00FF000000000000ULL) >> 40) |
           ((value & 0x0000FF0000000000ULL) >> 24) | ((value & 0x000000FF00000000ULL) >> 8) |
           ((value & 0x00000000FF000000ULL) << 8) | ((value & 0x0000000000FF0000ULL) << 24) |
           ((value & 0x000000000000FF00ULL) << 40) | ((value & 0x00000000000000FFULL) << 56);
    #endif
#else
    return ((value & 0xFF00000000000000ULL) >> 56) | ((value & 0x00FF000000000000ULL) >> 40) |
           ((value & 0x0000FF0000000000ULL) >> 24) | ((value & 0x000000FF00000000ULL) >> 8) |
           ((value & 0x00000000FF000000ULL) << 8) | ((value & 0x0000000000FF0000ULL) << 24) |
           ((value & 0x000000000000FF00ULL) << 40) | ((value & 0x00000000000000FFULL) << 56);
#endif
}

}  // namespace detail

/**
 * @brief Swap bytes in a 16-bit value
 *
 * @param value 16-bit value to byte swap
 * @return Byte-swapped value
 */
constexpr uint16_t byteSwap16(uint16_t value) noexcept {
    return detail::byteSwap16Impl(value);
}

/**
 * @brief Swap bytes in a 32-bit value
 *
 * @param value 32-bit value to byte swap
 * @return Byte-swapped value
 */
constexpr uint32_t byteSwap32(uint32_t value) noexcept {
    return detail::byteSwap32Impl(value);
}

/**
 * @brief Swap bytes in a 64-bit value
 *
 * @param value 64-bit value to byte swap
 * @return Byte-swapped value
 */
constexpr uint64_t byteSwap64(uint64_t value) noexcept {
    return detail::byteSwap64Impl(value);
}

/**
 * @brief Generic byte swap function for integral types
 *
 * Automatically selects the appropriate byte swap implementation based
 * on the size of the type. Supports signed and unsigned integral types.
 *
 * @tparam Type Integral type to byte swap
 * @param value Value to byte swap
 * @return Byte-swapped value
 */
template <typename Type>
constexpr Type byteSwap(Type value) noexcept {
    static_assert(std::is_integral_v<Type>, "byteSwap only supports integral types");

    if constexpr (sizeof(Type) == 1) {
        // Single byte - no swapping needed
        return value;
    } else if constexpr (sizeof(Type) == 2) {
        // 16-bit types
        using UnsignedType = std::make_unsigned_t<Type>;
        auto unsigned_value = static_cast<UnsignedType>(value);
        auto swapped = byteSwap16(static_cast<uint16_t>(unsigned_value));
        return static_cast<Type>(swapped);
    } else if constexpr (sizeof(Type) == 4) {
        // 32-bit types
        using UnsignedType = std::make_unsigned_t<Type>;
        auto unsigned_value = static_cast<UnsignedType>(value);
        auto swapped = byteSwap32(static_cast<uint32_t>(unsigned_value));
        return static_cast<Type>(swapped);
    } else if constexpr (sizeof(Type) == 8) {
        // 64-bit types
        using UnsignedType = std::make_unsigned_t<Type>;
        auto unsigned_value = static_cast<UnsignedType>(value);
        auto swapped = byteSwap64(static_cast<uint64_t>(unsigned_value));
        return static_cast<Type>(swapped);
    } else {
        static_assert(sizeof(Type) <= 8, "byteSwap supports types up to 64 bits");
        return value;  // Should never reach here
    }
}

// =============================================================================
// Network Byte Order Conversion
// =============================================================================

/**
 * @brief Convert from host byte order to network byte order (big-endian)
 *
 * Network byte order is defined as big-endian. This function converts
 * values from the host's native byte order to network byte order.
 *
 * @tparam Type Integral type to convert
 * @param value Value in host byte order
 * @return Value in network byte order (big-endian)
 */
template <typename Type>
constexpr Type hostToNetwork(Type value) noexcept {
    static_assert(std::is_integral_v<Type>, "hostToNetwork only supports integral types");

    if constexpr (isLittleEndian()) {
        // Host is little-endian, need to swap to big-endian
        return byteSwap(value);
    } else {
        // Host is big-endian or unknown, no conversion needed
        return value;
    }
}

/**
 * @brief Convert from network byte order (big-endian) to host byte order
 *
 * Network byte order is defined as big-endian. This function converts
 * values from network byte order to the host's native byte order.
 *
 * @tparam Type Integral type to convert
 * @param value Value in network byte order (big-endian)
 * @return Value in host byte order
 */
template <typename Type>
constexpr Type networkToHost(Type value) noexcept {
    static_assert(std::is_integral_v<Type>, "networkToHost only supports integral types");

    if constexpr (isLittleEndian()) {
        // Host is little-endian, need to swap from big-endian
        return byteSwap(value);
    } else {
        // Host is big-endian or unknown, no conversion needed
        return value;
    }
}

// =============================================================================
// Specialized Network Conversion Functions
// =============================================================================

/**
 * @brief Convert 16-bit value from host to network byte order
 *
 * Equivalent to the POSIX htons() function.
 *
 * @param hostshort 16-bit value in host byte order
 * @return 16-bit value in network byte order
 */
constexpr uint16_t hostToNetworkShort(uint16_t hostshort) noexcept {
    return hostToNetwork(hostshort);
}

/**
 * @brief Convert 32-bit value from host to network byte order
 *
 * Equivalent to the POSIX htonl() function.
 *
 * @param hostlong 32-bit value in host byte order
 * @return 32-bit value in network byte order
 */
constexpr uint32_t hostToNetworkLong(uint32_t hostlong) noexcept {
    return hostToNetwork(hostlong);
}

/**
 * @brief Convert 16-bit value from network to host byte order
 *
 * Equivalent to the POSIX ntohs() function.
 *
 * @param netshort 16-bit value in network byte order
 * @return 16-bit value in host byte order
 */
constexpr uint16_t networkToHostShort(uint16_t netshort) noexcept {
    return networkToHost(netshort);
}

/**
 * @brief Convert 32-bit value from network to host byte order
 *
 * Equivalent to the POSIX ntohl() function.
 *
 * @param netlong 32-bit value in network byte order
 * @return 32-bit value in host byte order
 */
constexpr uint32_t networkToHostLong(uint32_t netlong) noexcept {
    return networkToHost(netlong);
}

// =============================================================================
// Utility Functions
// =============================================================================

/**
 * @brief Check if two byte orders are compatible (no swapping needed)
 *
 * @param order1 First byte order
 * @param order2 Second byte order
 * @return true if byte orders are compatible
 */
constexpr bool areByteOrdersCompatible(ByteOrder order1, ByteOrder order2) noexcept {
    // Unknown orders are considered compatible (conservative approach)
    if (order1 == ByteOrder::unknown || order2 == ByteOrder::unknown) {
        return true;
    }
    return order1 == order2;
}

/**
 * @brief Get the opposite byte order
 *
 * @param order Input byte order
 * @return Opposite byte order, or unknown if input is unknown/mixed
 */
constexpr ByteOrder getOppositeByteOrder(ByteOrder order) noexcept {
    switch (order) {
        case ByteOrder::little_endian:
            return ByteOrder::big_endian;
        case ByteOrder::big_endian:
            return ByteOrder::little_endian;
        default:
            return ByteOrder::unknown;
    }
}

/**
 * @brief Convert value between specified byte orders
 *
 * @tparam Type Integral type to convert
 * @param value Value to convert
 * @param from_order Source byte order
 * @param to_order Target byte order
 * @return Converted value
 */
template <typename Type>
constexpr Type convertByteOrder(Type value, ByteOrder from_order, ByteOrder to_order) noexcept {
    static_assert(std::is_integral_v<Type>, "convertByteOrder only supports integral types");

    if (areByteOrdersCompatible(from_order, to_order)) {
        return value;
    } else {
        return byteSwap(value);
    }
}

}  // namespace platform
}  // namespace trlc

// =============================================================================
// Convenience Macros
// =============================================================================

/**
 * @brief Check if system is little-endian at compile time
 *
 * Evaluates to true if the system uses little-endian byte order.
 */
#define TRLC_ENDIAN_LITTLE (trlc::platform::isLittleEndian())

/**
 * @brief Check if system is big-endian at compile time
 *
 * Evaluates to true if the system uses big-endian byte order.
 */
#define TRLC_ENDIAN_BIG (trlc::platform::isBigEndian())

/**
 * @brief Swap bytes in a value
 *
 * Generic macro for byte swapping that works with any integral type.
 *
 * @param x Value to byte swap
 * @return Byte-swapped value
 */
#define TRLC_BYTE_SWAP(x) (trlc::platform::byteSwap(x))

/**
 * @brief Convert value from host to network byte order
 *
 * @param x Value in host byte order
 * @return Value in network byte order
 */
#define TRLC_HTON(x) (trlc::platform::hostToNetwork(x))

/**
 * @brief Convert value from network to host byte order
 *
 * @param x Value in network byte order
 * @return Value in host byte order
 */
#define TRLC_NTOH(x) (trlc::platform::networkToHost(x))

/**
 * @brief Host to network byte order for 16-bit values
 *
 * Equivalent to POSIX htons().
 *
 * @param x 16-bit value in host byte order
 * @return 16-bit value in network byte order
 */
#define TRLC_HTONS(x) (trlc::platform::hostToNetworkShort(x))

/**
 * @brief Host to network byte order for 32-bit values
 *
 * Equivalent to POSIX htonl().
 *
 * @param x 32-bit value in host byte order
 * @return 32-bit value in network byte order
 */
#define TRLC_HTONL(x) (trlc::platform::hostToNetworkLong(x))

/**
 * @brief Network to host byte order for 16-bit values
 *
 * Equivalent to POSIX ntohs().
 *
 * @param x 16-bit value in network byte order
 * @return 16-bit value in host byte order
 */
#define TRLC_NTOHS(x) (trlc::platform::networkToHostShort(x))

/**
 * @brief Network to host byte order for 32-bit values
 *
 * Equivalent to POSIX ntohl().
 *
 * @param x 32-bit value in network byte order
 * @return 32-bit value in host byte order
 */
#define TRLC_NTOHL(x) (trlc::platform::networkToHostLong(x))

// Mark this header as successfully included
#define TRLC_ENDIANNESS_INCLUDED

// =============================================================================
// End of endianness.hpp
// =============================================================================

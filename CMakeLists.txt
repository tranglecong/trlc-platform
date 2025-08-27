cmake_minimum_required(VERSION 3.16)

# Project definition
project(trlc-platform 
    VERSION 1.0.0
    DESCRIPTION "Platform detection and abstraction layer"
    LANGUAGES CXX)

# Set CMake policies for modern behavior
if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)  # option() honors normal variables
endif()
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)  # MSVC runtime library flags are selected by an abstraction
endif()

# Options
option(TRLC_PLATFORM_ENABLE_ASSERTS "Enable assertion macros" OFF)
option(TRLC_PLATFORM_ENABLE_EXPERIMENTAL "Enable experimental features" OFF)
option(TRLC_PLATFORM_FORCE_PORTABLE "Force portable implementations" OFF)
option(TRLC_PLATFORM_BUILD_TESTS "Build unit tests" ON)

# C++ standard requirements
# Default to C++20 if available, fallback to C++17
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    
    # Test if C++20 works, otherwise fallback to C++17
    include(CheckCXXSourceCompiles)
    check_cxx_source_compiles("
        #if __cplusplus < 202002L
        #error C++20 not available
        #endif
        int main() { return 0; }
    " COMPILER_SUPPORTS_CPP20)
    
    if(NOT COMPILER_SUPPORTS_CPP20)
        message(STATUS "C++20 not supported, falling back to C++17")
        set(CMAKE_CXX_STANDARD 17)
    endif()
else()
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# Validate minimum C++17 requirement
if(CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "trlc-platform requires C++17 or later, got C++${CMAKE_CXX_STANDARD}")
endif()

# Include custom CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CheckCompiler)
include(CheckPlatform)
include(CheckFeatures)

# Run platform detection
trlc_detect_compiler()
trlc_detect_platform()
trlc_check_platform_capabilities()
trlc_detect_features()
trlc_check_compiler_capabilities()

# Main library target (header-only)
add_library(trlc-platform INTERFACE)
add_library(trlc::platform ALIAS trlc-platform)

# Target properties
set_target_properties(trlc-platform PROPERTIES
    INTERFACE_COMPILE_FEATURES cxx_std_17
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME platform
)

# Include directories
target_include_directories(trlc-platform
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Compiler requirements
target_compile_features(trlc-platform INTERFACE cxx_std_17)

# Configuration header generation
# Ensure we get the detected values for C++ standard
if(NOT DEFINED TRLC_CPP_STANDARD)
    if(CMAKE_CXX_STANDARD EQUAL 23 OR CMAKE_CXX_STANDARD GREATER 23)
        set(TRLC_CPP_STANDARD "cpp_23")
        set(TRLC_CPP_VERSION 202302L)
    elseif(CMAKE_CXX_STANDARD EQUAL 20)
        set(TRLC_CPP_STANDARD "cpp_20")
        set(TRLC_CPP_VERSION 202002L)
    elseif(CMAKE_CXX_STANDARD EQUAL 17)
        set(TRLC_CPP_STANDARD "cpp_17")
        set(TRLC_CPP_VERSION 201703L)
    else()
        set(TRLC_CPP_STANDARD "cpp_unknown")
        set(TRLC_CPP_VERSION 0L)
    endif()
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/trlc/platform/config.hpp"
    @ONLY
)

# Add generated config header to include path
target_include_directories(trlc-platform
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

# Compile definitions based on options
if(TRLC_PLATFORM_ENABLE_ASSERTS)
    target_compile_definitions(trlc-platform INTERFACE TRLC_PLATFORM_ENABLE_ASSERTS=1)
endif()

if(TRLC_PLATFORM_ENABLE_EXPERIMENTAL)
    target_compile_definitions(trlc-platform INTERFACE TRLC_PLATFORM_ENABLE_EXPERIMENTAL=1)
endif()

if(TRLC_PLATFORM_FORCE_PORTABLE)
    target_compile_definitions(trlc-platform INTERFACE TRLC_PLATFORM_FORCE_PORTABLE=1)
endif()

# Auto-enable debug utilities in Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR NOT DEFINED CMAKE_BUILD_TYPE)
    target_compile_definitions(trlc-platform INTERFACE TRLC_PLATFORM_ENABLE_DEBUG_UTILS=1)
    message(STATUS "  Debug Utilities:         ENABLED (Debug build)")
else()
    message(STATUS "  Debug Utilities:         DISABLED (Release build)")
endif()

# Platform-specific compile definitions
target_compile_definitions(trlc-platform INTERFACE
    TRLC_COMPILER_TYPE="${TRLC_COMPILER_TYPE}"
    TRLC_PLATFORM_TYPE="${TRLC_PLATFORM_TYPE}"
    TRLC_ARCHITECTURE_TYPE="${TRLC_ARCHITECTURE_TYPE}"
)

# Tests
if(TRLC_PLATFORM_BUILD_TESTS AND CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()
    add_subdirectory(tests)
    add_subdirectory(examples)
endif()

# Installation configuration
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install targets
install(TARGETS trlc-platform
    EXPORT trlc-platform-targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp"
)

# Install generated config header
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/trlc/platform/config.hpp"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/trlc/platform
)

# Export targets for build tree
export(EXPORT trlc-platform-targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/trlc-platform-targets.cmake"
    NAMESPACE trlc::
)

# Create package config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/trlc-platform-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/trlc-platform-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/trlc-platform
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# Create package version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/trlc-platform-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install package config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/trlc-platform-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/trlc-platform-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/trlc-platform
)

# Install export targets
install(EXPORT trlc-platform-targets
    FILE trlc-platform-targets.cmake
    NAMESPACE trlc::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/trlc-platform
)

# Register package for use from build tree
export(PACKAGE trlc-platform)

# Display configuration summary
message(STATUS "")
message(STATUS "trlc-platform Configuration Summary:")
message(STATUS "  Version:                 ${PROJECT_VERSION}")
message(STATUS "  C++ Standard:            C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler:                ${TRLC_COMPILER_NAME} ${TRLC_COMPILER_VERSION}")
message(STATUS "  Platform:                ${TRLC_PLATFORM_NAME}")
message(STATUS "  Architecture:            ${TRLC_ARCHITECTURE_TYPE}")
message(STATUS "  Environment:             ${TRLC_ENVIRONMENT_TYPE}")
message(STATUS "  Build Tests:             ${TRLC_PLATFORM_BUILD_TESTS}")
message(STATUS "  Enable Asserts:          ${TRLC_PLATFORM_ENABLE_ASSERTS}")
message(STATUS "  Enable Experimental:     ${TRLC_PLATFORM_ENABLE_EXPERIMENTAL}")
message(STATUS "  Force Portable:          ${TRLC_PLATFORM_FORCE_PORTABLE}")
message(STATUS "  Install Prefix:          ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "Feature Detection Results:")
message(STATUS "  Exceptions:              ${TRLC_HAS_EXCEPTIONS}")
message(STATUS "  RTTI:                    ${TRLC_HAS_RTTI}")
message(STATUS "  Threads:                 ${TRLC_HAS_THREADS}")
message(STATUS "  Atomic:                  ${TRLC_HAS_ATOMIC}")
message(STATUS "  Structured Bindings:     ${TRLC_HAS_STRUCTURED_BINDINGS}")
message(STATUS "  if constexpr:            ${TRLC_HAS_IF_CONSTEXPR}")
if(CMAKE_CXX_STANDARD GREATER_EQUAL 20)
message(STATUS "  Concepts:                ${TRLC_HAS_CONCEPTS}")
message(STATUS "  Ranges:                  ${TRLC_HAS_RANGES}")
endif()
message(STATUS "  Compiler Builtins:       ${TRLC_HAS_BUILTIN_FUNCTIONS}")
message(STATUS "  SIMD (SSE):              ${TRLC_HAS_SSE_SUPPORT}")
message(STATUS "  SIMD (AVX):              ${TRLC_HAS_AVX_SUPPORT}")
if(TRLC_ARCHITECTURE_TYPE MATCHES "arm")
message(STATUS "  SIMD (NEON):             ${TRLC_HAS_NEON_SUPPORT}")
endif()
message(STATUS "")

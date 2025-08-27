# CheckCompiler.cmake - Compiler detection module for trlc-platform

include_guard(GLOBAL)

# Function to detect compiler and set cache variables
function(trlc_detect_compiler)
    message(STATUS "Detecting compiler information...")
    
    # Initialize variables
    set(TRLC_COMPILER_TYPE "unknown")
    set(TRLC_COMPILER_VERSION "0.0.0")
    set(TRLC_COMPILER_MAJOR 0)
    set(TRLC_COMPILER_MINOR 0)
    set(TRLC_COMPILER_PATCH 0)
    set(TRLC_COMPILER_NAME "Unknown Compiler")

    # Detect compiler type and version using CMAKE_CXX_COMPILER_ID
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(TRLC_COMPILER_TYPE "gcc")
        set(TRLC_COMPILER_NAME "GNU GCC")
        set(TRLC_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
        
        # Parse GCC version with fallback for incomplete versions
        if(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)")
            set(TRLC_COMPILER_MAJOR "${CMAKE_MATCH_1}")
            set(TRLC_COMPILER_MINOR "${CMAKE_MATCH_2}")
            set(TRLC_COMPILER_PATCH "${CMAKE_MATCH_3}")
        elseif(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)\\.([0-9]+)")
            set(TRLC_COMPILER_MAJOR "${CMAKE_MATCH_1}")
            set(TRLC_COMPILER_MINOR "${CMAKE_MATCH_2}")
            set(TRLC_COMPILER_PATCH "0")
        else()
            set(TRLC_COMPILER_MAJOR "0")
            set(TRLC_COMPILER_MINOR "0")
            set(TRLC_COMPILER_PATCH "0")
        endif()
        
        # Check minimum GCC version for C++17 support
        if(TRLC_COMPILER_MAJOR LESS 7)
            message(WARNING "GCC ${TRLC_COMPILER_VERSION} may have limited C++17 support. GCC 7+ recommended.")
        endif()
        
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(TRLC_COMPILER_TYPE "clang")
        set(TRLC_COMPILER_NAME "LLVM Clang")
        set(TRLC_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
        
        # Parse Clang version
        if(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)")
            set(TRLC_COMPILER_MAJOR "${CMAKE_MATCH_1}")
            set(TRLC_COMPILER_MINOR "${CMAKE_MATCH_2}")
            set(TRLC_COMPILER_PATCH "${CMAKE_MATCH_3}")
        elseif(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)\\.([0-9]+)")
            set(TRLC_COMPILER_MAJOR "${CMAKE_MATCH_1}")
            set(TRLC_COMPILER_MINOR "${CMAKE_MATCH_2}")
            set(TRLC_COMPILER_PATCH "0")
        else()
            set(TRLC_COMPILER_MAJOR "0")
            set(TRLC_COMPILER_MINOR "0")
            set(TRLC_COMPILER_PATCH "0")
        endif()
        
        # Check minimum Clang version for C++17 support
        if(TRLC_COMPILER_MAJOR LESS 6)
            message(WARNING "Clang ${TRLC_COMPILER_VERSION} may have limited C++17 support. Clang 6+ recommended.")
        endif()
        
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        set(TRLC_COMPILER_TYPE "clang")
        set(TRLC_COMPILER_NAME "Apple Clang")
        set(TRLC_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
        
        # Parse Apple Clang version
        if(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)")
            set(TRLC_COMPILER_MAJOR "${CMAKE_MATCH_1}")
            set(TRLC_COMPILER_MINOR "${CMAKE_MATCH_2}")
            set(TRLC_COMPILER_PATCH "${CMAKE_MATCH_3}")
        elseif(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)\\.([0-9]+)")
            set(TRLC_COMPILER_MAJOR "${CMAKE_MATCH_1}")
            set(TRLC_COMPILER_MINOR "${CMAKE_MATCH_2}")
            set(TRLC_COMPILER_PATCH "0")
        else()
            set(TRLC_COMPILER_MAJOR "0")
            set(TRLC_COMPILER_MINOR "0")
            set(TRLC_COMPILER_PATCH "0")
        endif()
        
        # Check minimum Apple Clang version
        if(TRLC_COMPILER_MAJOR LESS 10)
            message(WARNING "Apple Clang ${TRLC_COMPILER_VERSION} may have limited C++17 support. Apple Clang 10+ recommended.")
        endif()
        
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(TRLC_COMPILER_TYPE "msvc")
        set(TRLC_COMPILER_NAME "Microsoft Visual C++")
        set(TRLC_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
        
        # Parse MSVC version
        if(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)")
            set(TRLC_COMPILER_MAJOR "${CMAKE_MATCH_1}")
            set(TRLC_COMPILER_MINOR "${CMAKE_MATCH_2}")
            set(TRLC_COMPILER_PATCH "${CMAKE_MATCH_3}")
        elseif(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)\\.([0-9]+)")
            set(TRLC_COMPILER_MAJOR "${CMAKE_MATCH_1}")
            set(TRLC_COMPILER_MINOR "${CMAKE_MATCH_2}")
            set(TRLC_COMPILER_PATCH "0")
        else()
            set(TRLC_COMPILER_MAJOR "0")
            set(TRLC_COMPILER_MINOR "0")
            set(TRLC_COMPILER_PATCH "0")
        endif()
        
        # Check minimum MSVC version for C++17 support
        if(TRLC_COMPILER_MAJOR LESS 19 OR (TRLC_COMPILER_MAJOR EQUAL 19 AND TRLC_COMPILER_MINOR LESS 10))
            message(WARNING "MSVC ${TRLC_COMPILER_VERSION} may have limited C++17 support. MSVC 2017 (19.10+) recommended.")
        endif()
        
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        # Check if it's the new Intel LLVM-based compiler
        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "2021")
            set(TRLC_COMPILER_TYPE "intel_llvm")
            set(TRLC_COMPILER_NAME "Intel C++ Compiler (LLVM)")
        else()
            set(TRLC_COMPILER_TYPE "intel_classic")
            set(TRLC_COMPILER_NAME "Intel C++ Compiler (Classic)")
        endif()
        set(TRLC_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
        
        # Parse Intel version
        if(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)")
            set(TRLC_COMPILER_MAJOR "${CMAKE_MATCH_1}")
            set(TRLC_COMPILER_MINOR "${CMAKE_MATCH_2}")
            set(TRLC_COMPILER_PATCH "${CMAKE_MATCH_3}")
        else()
            # Intel versions can be just year-based
            set(TRLC_COMPILER_MAJOR "${CMAKE_CXX_COMPILER_VERSION}")
            set(TRLC_COMPILER_MINOR "0")
            set(TRLC_COMPILER_PATCH "0")
        endif()
        
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
        set(TRLC_COMPILER_TYPE "intel_llvm")
        set(TRLC_COMPILER_NAME "Intel C++ Compiler (LLVM)")
        set(TRLC_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
        
        # Parse Intel LLVM version
        if(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)")
            set(TRLC_COMPILER_MAJOR "${CMAKE_MATCH_1}")
            set(TRLC_COMPILER_MINOR "${CMAKE_MATCH_2}")
            set(TRLC_COMPILER_PATCH "${CMAKE_MATCH_3}")
        else()
            set(TRLC_COMPILER_MAJOR "${CMAKE_CXX_COMPILER_VERSION}")
            set(TRLC_COMPILER_MINOR "0")
            set(TRLC_COMPILER_PATCH "0")
        endif()
        
    elseif(MINGW OR CMAKE_CXX_COMPILER_ID STREQUAL "MinGW")
        set(TRLC_COMPILER_TYPE "mingw")
        set(TRLC_COMPILER_NAME "MinGW GCC")
        set(TRLC_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
        
        # Parse MinGW version (usually based on GCC)
        if(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)")
            set(TRLC_COMPILER_MAJOR "${CMAKE_MATCH_1}")
            set(TRLC_COMPILER_MINOR "${CMAKE_MATCH_2}")
            set(TRLC_COMPILER_PATCH "${CMAKE_MATCH_3}")
        elseif(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)\\.([0-9]+)")
            set(TRLC_COMPILER_MAJOR "${CMAKE_MATCH_1}")
            set(TRLC_COMPILER_MINOR "${CMAKE_MATCH_2}")
            set(TRLC_COMPILER_PATCH "0")
        else()
            set(TRLC_COMPILER_MAJOR "0")
            set(TRLC_COMPILER_MINOR "0")
            set(TRLC_COMPILER_PATCH "0")
        endif()
        
    else()
        # Handle unknown compilers
        set(TRLC_COMPILER_TYPE "unknown")
        set(TRLC_COMPILER_NAME "Unknown Compiler (${CMAKE_CXX_COMPILER_ID})")
        set(TRLC_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
        set(TRLC_COMPILER_MAJOR "0")
        set(TRLC_COMPILER_MINOR "0")
        set(TRLC_COMPILER_PATCH "0")
        message(WARNING "Unknown compiler detected: ${CMAKE_CXX_COMPILER_ID}. Proceeding with fallback configuration.")
    endif()

    # Set cache variables
    set(TRLC_COMPILER_TYPE "${TRLC_COMPILER_TYPE}" CACHE STRING "Detected compiler type")
    set(TRLC_COMPILER_VERSION "${TRLC_COMPILER_VERSION}" CACHE STRING "Detected compiler version")
    set(TRLC_COMPILER_NAME "${TRLC_COMPILER_NAME}" CACHE STRING "Detected compiler name")
    set(TRLC_COMPILER_MAJOR "${TRLC_COMPILER_MAJOR}" CACHE STRING "Compiler major version")
    set(TRLC_COMPILER_MINOR "${TRLC_COMPILER_MINOR}" CACHE STRING "Compiler minor version")
    set(TRLC_COMPILER_PATCH "${TRLC_COMPILER_PATCH}" CACHE STRING "Compiler patch version")

    # Set parent scope variables
    set(TRLC_COMPILER_TYPE "${TRLC_COMPILER_TYPE}" PARENT_SCOPE)
    set(TRLC_COMPILER_VERSION "${TRLC_COMPILER_VERSION}" PARENT_SCOPE)
    set(TRLC_COMPILER_NAME "${TRLC_COMPILER_NAME}" PARENT_SCOPE)
    set(TRLC_COMPILER_MAJOR "${TRLC_COMPILER_MAJOR}" PARENT_SCOPE)
    set(TRLC_COMPILER_MINOR "${TRLC_COMPILER_MINOR}" PARENT_SCOPE)
    set(TRLC_COMPILER_PATCH "${TRLC_COMPILER_PATCH}" PARENT_SCOPE)

    # Informative status message
    message(STATUS "Detected compiler: ${TRLC_COMPILER_NAME} ${TRLC_COMPILER_VERSION}")
    message(STATUS "Compiler executable: ${CMAKE_CXX_COMPILER}")
endfunction()

# Function to check compiler capabilities
function(trlc_check_compiler_capabilities)
    # Check for specific compiler features
    include(CheckCXXCompilerFlag)
    
    # Check for color diagnostics support
    check_cxx_compiler_flag("-fdiagnostics-color=always" TRLC_COMPILER_HAS_COLOR_DIAGNOSTICS_GCC)
    check_cxx_compiler_flag("-fcolor-diagnostics" TRLC_COMPILER_HAS_COLOR_DIAGNOSTICS_CLANG)
    
    if(TRLC_COMPILER_HAS_COLOR_DIAGNOSTICS_GCC OR TRLC_COMPILER_HAS_COLOR_DIAGNOSTICS_CLANG)
        set(TRLC_COMPILER_HAS_COLOR_DIAGNOSTICS TRUE CACHE BOOL "Compiler supports color diagnostics")
    else()
        set(TRLC_COMPILER_HAS_COLOR_DIAGNOSTICS FALSE CACHE BOOL "Compiler supports color diagnostics")
    endif()
    
    # Check for builtin attributes support
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        set(TRLC_COMPILER_HAS_BUILTIN_ATTRIBUTES TRUE CACHE BOOL "Compiler supports __builtin attributes")
    else()
        set(TRLC_COMPILER_HAS_BUILTIN_ATTRIBUTES FALSE CACHE BOOL "Compiler supports __builtin attributes")
    endif()
    
    # Check for inline assembly support
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        set(TRLC_COMPILER_HAS_INLINE_ASM TRUE CACHE BOOL "Compiler supports inline assembly")
    else()
        set(TRLC_COMPILER_HAS_INLINE_ASM FALSE CACHE BOOL "Compiler supports inline assembly")
    endif()
    
    message(DEBUG "Compiler capabilities checked")
endfunction()

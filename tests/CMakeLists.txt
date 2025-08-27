# Tests CMakeLists.txt for trlc-platform

# Enable testing
enable_testing()

# Function to create a test executable
function(add_platform_test test_name source_file)
    add_executable(${test_name} ${source_file})

    target_link_libraries(${test_name} trlc-platform)

    # Add to test suite
    add_test(NAME ${test_name} COMMAND ${test_name})
    
    # Set test properties
    set_tests_properties(${test_name} PROPERTIES
        TIMEOUT 30
        FAIL_REGULAR_EXPRESSION "❌|FAILED|ERROR"
    )
endfunction()

# Create test executables for existing tests
add_platform_test(test_compiler_detection test_compiler_detection.cpp)
add_platform_test(test_platform_detection test_platform_detection.cpp)
add_platform_test(test_architecture_detection test_architecture_detection.cpp)
add_platform_test(test_cpp_standard_detection test_cpp_standard_detection.cpp)
add_platform_test(test_features_detection test_features_detection.cpp)
add_platform_test(test_macros test_macros.cpp)
add_platform_test(test_endianness test_endianness.cpp)
add_platform_test(test_typeinfo test_typeinfo.cpp)
add_platform_test(test_debug_utils test_debug_utils.cpp)
add_platform_test(test_integration test_integration.cpp)
add_platform_test(test_template_specializations test_template_specializations.cpp)


# Create a target to run all tests
add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose --output-on-failure
    COMMENT "Running all TRLC platform tests"
)

# Print configuration summary
message(STATUS "TRLC Platform Test Configuration:")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Test Timeout: 30 seconds")
message(STATUS "  Run tests with: make test OR ctest --verbose")
message(STATUS "  Run all tests: make run_all_tests")

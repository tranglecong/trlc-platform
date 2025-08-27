# Example CMakeLists.txt for trlc-platform

# Function to create a test executable
function(add_platform_example test_name source_file)
    add_executable(${test_name} ${source_file})

    target_link_libraries(${test_name} trlc-platform)
    
endfunction()

add_platform_example(portable_library_example portable_library_example.cpp)
add_platform_example(readme_example readme_example.cpp)
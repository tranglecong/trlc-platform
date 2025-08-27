# CheckFeatures.cmake - Feature detection module for trlc-platform

include_guard(GLOBAL)

# Function to detect language and runtime features
function(trlc_detect_features)
    message(STATUS "Detecting C++ language and compiler features...")
    
    # Include necessary CMake modules
    include(CheckCXXSourceCompiles)
    include(CheckIncludeFileCXX)
    
    # Detect C++ standard features
    trlc_check_cpp_standard_features()
    
    # Detect language features
    trlc_check_language_features()
    
    # Detect compiler capabilities
    trlc_check_compiler_features()
    
    # Detect runtime features (basic detection)
    trlc_check_runtime_features()
    
    message(STATUS "Feature detection completed")
endfunction()

# Function to check C++ standard features and determine preferred standard
function(trlc_check_cpp_standard_features)
    message(STATUS "Checking C++ standard support...")
    
    # Determine available and target C++ standard
    # Default to C++20 if available, fallback to C++17
    if(NOT DEFINED CMAKE_CXX_STANDARD)
        # Try C++20 first
        check_cxx_source_compiles("
            #if __cplusplus < 202002L
            #error C++20 not available
            #endif
            int main() { return 0; }
        " TRLC_COMPILER_SUPPORTS_CPP20)
        
        if(TRLC_COMPILER_SUPPORTS_CPP20)
            set(CMAKE_CXX_STANDARD 20 PARENT_SCOPE)
            set(TRLC_PREFERRED_CPP_STANDARD "cpp_20")
            message(STATUS "C++20 support detected - using C++20 as default")
        else()
            # Check C++17 support (minimum requirement)
            check_cxx_source_compiles("
                #if __cplusplus < 201703L
                #error C++17 not available
                #endif
                int main() { return 0; }
            " TRLC_COMPILER_SUPPORTS_CPP17)
            
            if(TRLC_COMPILER_SUPPORTS_CPP17)
                set(CMAKE_CXX_STANDARD 17 PARENT_SCOPE)
                set(TRLC_PREFERRED_CPP_STANDARD "cpp_17")
                message(STATUS "C++17 support detected - using C++17 as fallback")
            else()
                message(FATAL_ERROR "C++17 support is required but not available")
            endif()
        endif()
    endif()
    
    # Determine current C++ standard based on CMAKE_CXX_STANDARD
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
        message(FATAL_ERROR "trlc-platform requires C++17 or later, got C++${CMAKE_CXX_STANDARD}")
    endif()

    # Check for specific C++17 features
    message(STATUS "Testing C++17 features...")
    
    check_cxx_source_compiles("
        #include <tuple>
        int main() {
            auto tuple = std::make_tuple(1, 2.0, 'c');
            auto [a, b, c] = tuple;
            return a + static_cast<int>(b) + c;
        }
    " TRLC_HAS_STRUCTURED_BINDINGS)

    check_cxx_source_compiles("
        template<bool B>
        constexpr int test() {
            if constexpr (B) {
                return 42;
            } else {
                return 0;
            }
        }
        int main() { 
            static_assert(test<true>() == 42);
            return test<false>(); 
        }
    " TRLC_HAS_IF_CONSTEXPR)

    check_cxx_source_compiles("
        #include <optional>
        #include <variant>
        #include <string_view>
        int main() {
            std::optional<int> opt = 42;
            std::variant<int, double> var = 3.14;
            std::string_view sv = \"hello\";
            return opt.value() + static_cast<int>(std::get<double>(var)) + sv.length();
        }
    " TRLC_HAS_CPP17_LIBRARY)

    # Check for C++20 features if C++20 is enabled
    if(CMAKE_CXX_STANDARD GREATER_EQUAL 20)
        message(STATUS "Testing C++20 features...")
        
        check_cxx_source_compiles("
            #include <concepts>
            template<typename T>
            concept Integral = std::is_integral_v<T>;
            
            template<Integral T>
            constexpr T add(T a, T b) { return a + b; }
            
            int main() { 
                static_assert(Integral<int>);
                return add(1, 2); 
            }
        " TRLC_HAS_CONCEPTS)

        check_cxx_source_compiles("
            #include <ranges>
            #include <vector>
            #include <algorithm>
            int main() {
                std::vector<int> v{1, 2, 3, 4, 5};
                auto filtered = v | std::views::filter([](int x) { return x > 2; });
                return std::ranges::distance(filtered);
            }
        " TRLC_HAS_RANGES)

        check_cxx_source_compiles("
            #include <coroutine>
            struct SimpleCoroutine {
                struct promise_type {
                    SimpleCoroutine get_return_object() { return {}; }
                    std::suspend_never initial_suspend() { return {}; }
                    std::suspend_never final_suspend() noexcept { return {}; }
                    void return_void() {}
                    void unhandled_exception() {}
                };
            };
            SimpleCoroutine test() { co_return; }
            int main() { test(); return 0; }
        " TRLC_HAS_COROUTINES)

        check_cxx_source_compiles("
            #include <span>
            #include <array>
            int main() {
                std::array<int, 5> arr{1, 2, 3, 4, 5};
                std::span<int> sp{arr};
                return sp.size();
            }
        " TRLC_HAS_SPAN)
    else()
        set(TRLC_HAS_CONCEPTS FALSE)
        set(TRLC_HAS_RANGES FALSE)
        set(TRLC_HAS_COROUTINES FALSE)
        set(TRLC_HAS_SPAN FALSE)
    endif()

    # Check for C++23 features if C++23 is enabled
    if(CMAKE_CXX_STANDARD GREATER_EQUAL 23)
        message(STATUS "Testing C++23 features...")
        
        check_cxx_source_compiles("
            #include <print>
            int main() {
                std::print(\"Hello, C++23!\\n\");
                return 0;
            }
        " TRLC_HAS_PRINT)
    else()
        set(TRLC_HAS_PRINT FALSE)
    endif()

    # Set cache variables
    set(TRLC_CPP_STANDARD "${TRLC_CPP_STANDARD}" CACHE STRING "Detected C++ standard")
    set(TRLC_CPP_VERSION "${TRLC_CPP_VERSION}" CACHE STRING "C++ version macro value")
    
    # Set parent scope variables
    set(TRLC_CPP_STANDARD "${TRLC_CPP_STANDARD}" PARENT_SCOPE)
    set(TRLC_CPP_VERSION "${TRLC_CPP_VERSION}" PARENT_SCOPE)
    
    message(STATUS "C++ Standard: ${TRLC_CPP_STANDARD}")
endfunction()

# Function to check core language features using try_compile
function(trlc_check_language_features)
    message(STATUS "Testing core language features...")
    
    # Check for exceptions support
    check_cxx_source_compiles("
        #include <stdexcept>
        int main() {
            try {
                throw std::runtime_error(\"test\");
            } catch (const std::exception& e) {
                return 0;
            }
            return 1;
        }
    " TRLC_HAS_EXCEPTIONS_TEST)
    
    # Exception support might be disabled via compiler flags
    if(CMAKE_CXX_FLAGS MATCHES "-fno-exceptions" OR CMAKE_CXX_FLAGS MATCHES "/EHs-c-")
        set(TRLC_HAS_EXCEPTIONS FALSE CACHE BOOL "Has exception support")
        message(STATUS "Exceptions: disabled by compiler flags")
    else()
        set(TRLC_HAS_EXCEPTIONS ${TRLC_HAS_EXCEPTIONS_TEST} CACHE BOOL "Has exception support")
        if(TRLC_HAS_EXCEPTIONS)
            message(STATUS "Exceptions: supported")
        else()
            message(STATUS "Exceptions: not supported")
        endif()
    endif()

    # Check for RTTI support
    check_cxx_source_compiles("
        #include <typeinfo>
        class Base { public: virtual ~Base() = default; };
        class Derived : public Base {};
        int main() {
            Base* b = new Derived();
            bool result = (typeid(*b) == typeid(Derived));
            delete b;
            return result ? 0 : 1;
        }
    " TRLC_HAS_RTTI_TEST)
    
    # RTTI support might be disabled
    if(CMAKE_CXX_FLAGS MATCHES "-fno-rtti" OR CMAKE_CXX_FLAGS MATCHES "/GR-")
        set(TRLC_HAS_RTTI FALSE CACHE BOOL "Has RTTI support")
        message(STATUS "RTTI: disabled by compiler flags")
    else()
        set(TRLC_HAS_RTTI ${TRLC_HAS_RTTI_TEST} CACHE BOOL "Has RTTI support")
        if(TRLC_HAS_RTTI)
            message(STATUS "RTTI: supported")
        else()
            message(STATUS "RTTI: not supported")
        endif()
    endif()

    # Check for thread support
    find_package(Threads QUIET)
    if(Threads_FOUND)
        check_cxx_source_compiles("
            #include <thread>
            #include <atomic>
            #include <mutex>
            
            std::atomic<int> counter{0};
            std::mutex mtx;
            
            void worker() {
                std::lock_guard<std::mutex> lock(mtx);
                counter.fetch_add(1);
            }
            
            int main() {
                std::thread t1(worker);
                std::thread t2(worker);
                t1.join();
                t2.join();
                return counter.load() == 2 ? 0 : 1;
            }
        " TRLC_HAS_THREADS)
        
        if(TRLC_HAS_THREADS)
            message(STATUS "Threads: supported")
        else()
            message(STATUS "Threads: library found but test failed")
        endif()
    else()
        set(TRLC_HAS_THREADS FALSE CACHE BOOL "Has thread support")
        message(STATUS "Threads: not found")
    endif()

    # Check for atomic operations
    check_cxx_source_compiles("
        #include <atomic>
        int main() {
            std::atomic<int> a{0};
            std::atomic<long long> b{0};
            std::atomic<void*> c{nullptr};
            
            a.store(42);
            b.fetch_add(1);
            c.compare_exchange_weak(c, nullptr);
            
            return a.load() + b.load() + (c.load() == nullptr ? 0 : 1);
        }
    " TRLC_HAS_ATOMIC)
    
    if(TRLC_HAS_ATOMIC)
        message(STATUS "Atomic operations: supported")
    else()
        message(STATUS "Atomic operations: not supported")
    endif()

    # Set parent scope variables
    set(TRLC_HAS_EXCEPTIONS "${TRLC_HAS_EXCEPTIONS}" PARENT_SCOPE)
    set(TRLC_HAS_RTTI "${TRLC_HAS_RTTI}" PARENT_SCOPE)
    set(TRLC_HAS_THREADS "${TRLC_HAS_THREADS}" PARENT_SCOPE)
    set(TRLC_HAS_ATOMIC "${TRLC_HAS_ATOMIC}" PARENT_SCOPE)
endfunction()

# Function to check compiler-specific features and intrinsics
function(trlc_check_compiler_features)
    message(STATUS "Testing compiler capabilities...")
    
    # Check for builtin functions (GCC/Clang)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        check_cxx_source_compiles("
            int main() {
                unsigned int x = 0xFF;
                int result = __builtin_popcount(x);
                if (__builtin_expect(result == 8, 1)) {
                    return 0;
                }
                __builtin_unreachable();
            }
        " TRLC_HAS_BUILTIN_FUNCTIONS)
        
        set(TRLC_HAS_BUILTIN_EXPECT TRUE CACHE BOOL "Has __builtin_expect")
        set(TRLC_HAS_BUILTIN_UNREACHABLE TRUE CACHE BOOL "Has __builtin_unreachable")
        
        if(TRLC_HAS_BUILTIN_FUNCTIONS)
            message(STATUS "Compiler builtins: supported")
        else()
            message(STATUS "Compiler builtins: partially supported")
        endif()
    else()
        set(TRLC_HAS_BUILTIN_FUNCTIONS FALSE CACHE BOOL "Has builtin functions")
        set(TRLC_HAS_BUILTIN_EXPECT FALSE CACHE BOOL "Has __builtin_expect")
        set(TRLC_HAS_BUILTIN_UNREACHABLE FALSE CACHE BOOL "Has __builtin_unreachable")
        message(STATUS "Compiler builtins: not available (non-GCC/Clang compiler)")
    endif()

    # Check for __has_attribute support
    check_cxx_source_compiles("
        #ifdef __has_attribute
        #  if __has_attribute(nodiscard)
        [[nodiscard]] int test() { return 42; }
        #  else
        int test() { return 42; }
        #  endif
        #else
        int test() { return 42; }
        #endif
        int main() { return test(); }
    " TRLC_HAS_ATTRIBUTE_DETECTION)

    # Check for __has_cpp_attribute support
    check_cxx_source_compiles("
        #ifdef __has_cpp_attribute
        #  if __has_cpp_attribute(nodiscard)
        [[nodiscard]] int test() { return 42; }
        #  else
        int test() { return 42; }
        #  endif
        #else
        int test() { return 42; }
        #endif
        int main() { return test(); }
    " TRLC_HAS_CPP_ATTRIBUTE_DETECTION)

    # Check for inline assembly support
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        check_cxx_source_compiles("
            int main() {
                int result;
                #if defined(__x86_64__) || defined(__i386__)
                __asm__ volatile (\"movl $42, %0\" : \"=r\" (result));
                #elif defined(__aarch64__) || defined(__arm__)
                __asm__ volatile (\"mov %0, #42\" : \"=r\" (result));
                #else
                result = 42;  // Fallback for other architectures
                #endif
                return result == 42 ? 0 : 1;
            }
        " TRLC_HAS_INLINE_ASM)
        
        if(TRLC_HAS_INLINE_ASM)
            message(STATUS "Inline assembly: supported")
        else()
            message(STATUS "Inline assembly: test failed")
        endif()
    else()
        set(TRLC_HAS_INLINE_ASM FALSE CACHE BOOL "Has inline assembly support")
        message(STATUS "Inline assembly: not available (compiler limitation)")
    endif()

    # Set parent scope variables
    set(TRLC_HAS_BUILTIN_FUNCTIONS "${TRLC_HAS_BUILTIN_FUNCTIONS}" PARENT_SCOPE)
    set(TRLC_HAS_BUILTIN_EXPECT "${TRLC_HAS_BUILTIN_EXPECT}" PARENT_SCOPE)
    set(TRLC_HAS_BUILTIN_UNREACHABLE "${TRLC_HAS_BUILTIN_UNREACHABLE}" PARENT_SCOPE)
    set(TRLC_HAS_ATTRIBUTE_DETECTION "${TRLC_HAS_ATTRIBUTE_DETECTION}" PARENT_SCOPE)
    set(TRLC_HAS_CPP_ATTRIBUTE_DETECTION "${TRLC_HAS_CPP_ATTRIBUTE_DETECTION}" PARENT_SCOPE)
    set(TRLC_HAS_INLINE_ASM "${TRLC_HAS_INLINE_ASM}" PARENT_SCOPE)
endfunction()

# Function to check runtime features and SIMD support
function(trlc_check_runtime_features)
    message(STATUS "Testing SIMD and runtime features...")
    
    # Check for x86 SIMD support (compile-time detection only)
    if(TRLC_ARCHITECTURE_TYPE STREQUAL "x86_64" OR TRLC_ARCHITECTURE_TYPE STREQUAL "x86")
        # Check for SSE support
        check_cxx_source_compiles("
            #include <xmmintrin.h>
            int main() {
                __m128 a = _mm_setzero_ps();
                __m128 b = _mm_set1_ps(1.0f);
                __m128 c = _mm_add_ps(a, b);
                float result[4];
                _mm_storeu_ps(result, c);
                return result[0] == 1.0f ? 0 : 1;
            }
        " TRLC_HAS_SSE_COMPILE)

        # Check for SSE2 support
        check_cxx_source_compiles("
            #include <emmintrin.h>
            int main() {
                __m128d a = _mm_setzero_pd();
                __m128d b = _mm_set1_pd(1.0);
                __m128d c = _mm_add_pd(a, b);
                double result[2];
                _mm_storeu_pd(result, c);
                return result[0] == 1.0 ? 0 : 1;
            }
        " TRLC_HAS_SSE2_COMPILE)

        # Check for AVX support
        check_cxx_source_compiles("
            #include <immintrin.h>
            int main() {
                __m256 a = _mm256_setzero_ps();
                __m256 b = _mm256_set1_ps(1.0f);
                __m256 c = _mm256_add_ps(a, b);
                float result[8];
                _mm256_storeu_ps(result, c);
                return result[0] == 1.0f ? 0 : 1;
            }
        " TRLC_HAS_AVX_COMPILE)

        # Check for AVX2 support
        check_cxx_source_compiles("
            #include <immintrin.h>
            int main() {
                __m256i a = _mm256_setzero_si256();
                __m256i b = _mm256_set1_epi32(1);
                __m256i c = _mm256_add_epi32(a, b);
                return _mm256_extract_epi32(c, 0) == 1 ? 0 : 1;
            }
        " TRLC_HAS_AVX2_COMPILE)

        set(TRLC_HAS_SSE_SUPPORT ${TRLC_HAS_SSE_COMPILE} CACHE BOOL "Has SSE compile support")
        set(TRLC_HAS_SSE2_SUPPORT ${TRLC_HAS_SSE2_COMPILE} CACHE BOOL "Has SSE2 compile support")
        set(TRLC_HAS_AVX_SUPPORT ${TRLC_HAS_AVX_COMPILE} CACHE BOOL "Has AVX compile support")
        set(TRLC_HAS_AVX2_SUPPORT ${TRLC_HAS_AVX2_COMPILE} CACHE BOOL "Has AVX2 compile support")
        
        if(TRLC_HAS_AVX2_SUPPORT)
            message(STATUS "SIMD: AVX2 supported")
        elseif(TRLC_HAS_AVX_SUPPORT)
            message(STATUS "SIMD: AVX supported")
        elseif(TRLC_HAS_SSE2_SUPPORT)
            message(STATUS "SIMD: SSE2 supported")
        elseif(TRLC_HAS_SSE_SUPPORT)
            message(STATUS "SIMD: SSE supported")
        else()
            message(STATUS "SIMD: x86 SIMD not available")
        endif()
    else()
        set(TRLC_HAS_SSE_SUPPORT FALSE CACHE BOOL "Has SSE compile support")
        set(TRLC_HAS_SSE2_SUPPORT FALSE CACHE BOOL "Has SSE2 compile support")
        set(TRLC_HAS_AVX_SUPPORT FALSE CACHE BOOL "Has AVX compile support")
        set(TRLC_HAS_AVX2_SUPPORT FALSE CACHE BOOL "Has AVX2 compile support")
    endif()

    # Check for ARM NEON support
    if(TRLC_ARCHITECTURE_TYPE STREQUAL "arm64" OR TRLC_ARCHITECTURE_TYPE STREQUAL "arm")
        check_cxx_source_compiles("
            #include <arm_neon.h>
            int main() {
                float32x4_t a = vdupq_n_f32(0.0f);
                float32x4_t b = vdupq_n_f32(1.0f);
                float32x4_t c = vaddq_f32(a, b);
                float result = vgetq_lane_f32(c, 0);
                return result == 1.0f ? 0 : 1;
            }
        " TRLC_HAS_NEON_COMPILE)
        
        set(TRLC_HAS_NEON_SUPPORT ${TRLC_HAS_NEON_COMPILE} CACHE BOOL "Has NEON compile support")
        
        if(TRLC_HAS_NEON_SUPPORT)
            message(STATUS "SIMD: ARM NEON supported")
        else()
            message(STATUS "SIMD: ARM NEON not available")
        endif()
    else()
        set(TRLC_HAS_NEON_SUPPORT FALSE CACHE BOOL "Has NEON compile support")
    endif()

    # Set parent scope variables
    set(TRLC_HAS_SSE_SUPPORT "${TRLC_HAS_SSE_SUPPORT}" PARENT_SCOPE)
    set(TRLC_HAS_SSE2_SUPPORT "${TRLC_HAS_SSE2_SUPPORT}" PARENT_SCOPE)
    set(TRLC_HAS_AVX_SUPPORT "${TRLC_HAS_AVX_SUPPORT}" PARENT_SCOPE)
    set(TRLC_HAS_AVX2_SUPPORT "${TRLC_HAS_AVX2_SUPPORT}" PARENT_SCOPE)
    set(TRLC_HAS_NEON_SUPPORT "${TRLC_HAS_NEON_SUPPORT}" PARENT_SCOPE)
endfunction()

# Convenience function to check all features
function(trlc_check_features)
    trlc_detect_features()
endfunction()

# Function to check C++ standard features
function(trlc_check_cpp_standard_features)
    # Determine C++ standard
    if(CMAKE_CXX_STANDARD EQUAL 23 OR CMAKE_CXX_STANDARD GREATER 23)
        set(TRLC_CPP_STANDARD "cpp_23" CACHE STRING "Detected C++ standard")
        set(TRLC_CPP_VERSION 202302L CACHE STRING "C++ version macro value")
    elseif(CMAKE_CXX_STANDARD EQUAL 20)
        set(TRLC_CPP_STANDARD "cpp_20" CACHE STRING "Detected C++ standard")
        set(TRLC_CPP_VERSION 202002L CACHE STRING "C++ version macro value")
    elseif(CMAKE_CXX_STANDARD EQUAL 17)
        set(TRLC_CPP_STANDARD "cpp_17" CACHE STRING "Detected C++ standard")
        set(TRLC_CPP_VERSION 201703L CACHE STRING "C++ version macro value")
    else()
        set(TRLC_CPP_STANDARD "cpp_pre_17" CACHE STRING "Detected C++ standard")
        set(TRLC_CPP_VERSION 201402L CACHE STRING "C++ version macro value")
    endif()

    # Check for specific C++17 features
    check_cxx_source_compiles("
        #include <tuple>
        int main() {
            auto [a, b] = std::make_tuple(1, 2);
            return 0;
        }
    " TRLC_HAS_STRUCTURED_BINDINGS)

    check_cxx_source_compiles("
        template<bool B>
        int test() {
            if constexpr (B) {
                return 1;
            } else {
                return 0;
            }
        }
        int main() { return test<true>(); }
    " TRLC_HAS_IF_CONSTEXPR)

    # Check for C++20 features
    if(CMAKE_CXX_STANDARD GREATER_EQUAL 20)
        check_cxx_source_compiles("
            #include <concepts>
            template<typename T>
            concept Integral = std::is_integral_v<T>;
            template<Integral T>
            T add(T a, T b) { return a + b; }
            int main() { return add(1, 2); }
        " TRLC_HAS_CONCEPTS)

        check_cxx_source_compiles("
            #include <ranges>
            #include <vector>
            int main() {
                std::vector<int> v{1, 2, 3};
                auto filtered = v | std::views::filter([](int x) { return x > 1; });
                return 0;
            }
        " TRLC_HAS_RANGES)
    else()
        set(TRLC_HAS_CONCEPTS FALSE CACHE BOOL "Has C++20 concepts")
        set(TRLC_HAS_RANGES FALSE CACHE BOOL "Has C++20 ranges")
    endif()

    # Set parent scope variables
    set(TRLC_CPP_STANDARD "${TRLC_CPP_STANDARD}" PARENT_SCOPE)
    set(TRLC_CPP_VERSION "${TRLC_CPP_VERSION}" PARENT_SCOPE)
endfunction()

# Function to check language features
function(trlc_check_language_features)
    # Check for exceptions support
    check_cxx_source_compiles("
        int main() {
            try {
                throw 42;
            } catch (int e) {
                return e == 42 ? 0 : 1;
            }
        }
    " TRLC_HAS_EXCEPTIONS_TEST)
    
    # Exception support might be disabled even if syntax works
    if(CMAKE_CXX_FLAGS MATCHES "-fno-exceptions" OR CMAKE_CXX_FLAGS MATCHES "/EHs-c-")
        set(TRLC_HAS_EXCEPTIONS FALSE CACHE BOOL "Has exception support")
    else()
        set(TRLC_HAS_EXCEPTIONS ${TRLC_HAS_EXCEPTIONS_TEST} CACHE BOOL "Has exception support")
    endif()

    # Check for RTTI support
    check_cxx_source_compiles("
        #include <typeinfo>
        class Base { virtual ~Base() {} };
        class Derived : public Base {};
        int main() {
            Base* b = new Derived();
            return typeid(*b) == typeid(Derived) ? 0 : 1;
        }
    " TRLC_HAS_RTTI_TEST)
    
    # RTTI support might be disabled
    if(CMAKE_CXX_FLAGS MATCHES "-fno-rtti" OR CMAKE_CXX_FLAGS MATCHES "/GR-")
        set(TRLC_HAS_RTTI FALSE CACHE BOOL "Has RTTI support")
    else()
        set(TRLC_HAS_RTTI ${TRLC_HAS_RTTI_TEST} CACHE BOOL "Has RTTI support")
    endif()

    # Check for thread support
    find_package(Threads QUIET)
    if(Threads_FOUND)
        check_cxx_source_compiles("
            #include <thread>
            #include <atomic>
            int main() {
                std::atomic<int> counter{0};
                std::thread t([&counter]() { counter++; });
                t.join();
                return counter.load();
            }
        " TRLC_HAS_THREADS)
    else()
        set(TRLC_HAS_THREADS FALSE CACHE BOOL "Has thread support")
    endif()

    # Check for atomic operations
    check_cxx_source_compiles("
        #include <atomic>
        int main() {
            std::atomic<int> a{0};
            a.store(42);
            return a.load() == 42 ? 0 : 1;
        }
    " TRLC_HAS_ATOMIC)

    message(DEBUG "Language features checked")
endfunction()

# Function to check compiler-specific features
function(trlc_check_compiler_features)
    # Check for builtin functions (GCC/Clang)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        check_cxx_source_compiles("
            int main() {
                return __builtin_popcount(0xFF) == 8 ? 0 : 1;
            }
        " TRLC_HAS_BUILTIN_FUNCTIONS)
        
        set(TRLC_HAS_BUILTIN_EXPECT TRUE CACHE BOOL "Has __builtin_expect")
        set(TRLC_HAS_BUILTIN_UNREACHABLE TRUE CACHE BOOL "Has __builtin_unreachable")
    else()
        set(TRLC_HAS_BUILTIN_FUNCTIONS FALSE CACHE BOOL "Has builtin functions")
        set(TRLC_HAS_BUILTIN_EXPECT FALSE CACHE BOOL "Has __builtin_expect")
        set(TRLC_HAS_BUILTIN_UNREACHABLE FALSE CACHE BOOL "Has __builtin_unreachable")
    endif()

    # Check for __has_attribute support
    check_cxx_source_compiles("
        #ifdef __has_attribute
        #  if __has_attribute(nodiscard)
        [[nodiscard]] int test() { return 42; }
        #  else
        int test() { return 42; }
        #  endif
        #else
        int test() { return 42; }
        #endif
        int main() { return test(); }
    " TRLC_HAS_ATTRIBUTE_DETECTION)

    # Check for __has_cpp_attribute support
    check_cxx_source_compiles("
        #ifdef __has_cpp_attribute
        #  if __has_cpp_attribute(nodiscard)
        [[nodiscard]] int test() { return 42; }
        #  else
        int test() { return 42; }
        #  endif
        #else
        int test() { return 42; }
        #endif
        int main() { return test(); }
    " TRLC_HAS_CPP_ATTRIBUTE_DETECTION)

    message(DEBUG "Compiler features checked")
endfunction()

# Function to check runtime features (basic compile-time detection)
function(trlc_check_runtime_features)
    # Check for x86 SIMD support (compile-time detection only)
    if(TRLC_ARCHITECTURE_TYPE STREQUAL "x86_64" OR TRLC_ARCHITECTURE_TYPE STREQUAL "x86")
        # Check for SSE support
        check_cxx_source_compiles("
            #include <xmmintrin.h>
            int main() {
                __m128 a = _mm_setzero_ps();
                return 0;
            }
        " TRLC_HAS_SSE_COMPILE)

        # Check for AVX support
        check_cxx_source_compiles("
            #include <immintrin.h>
            int main() {
                __m256 a = _mm256_setzero_ps();
                return 0;
            }
        " TRLC_HAS_AVX_COMPILE)

        set(TRLC_HAS_SSE_SUPPORT ${TRLC_HAS_SSE_COMPILE} CACHE BOOL "Has SSE compile support")
        set(TRLC_HAS_AVX_SUPPORT ${TRLC_HAS_AVX_COMPILE} CACHE BOOL "Has AVX compile support")
    else()
        set(TRLC_HAS_SSE_SUPPORT FALSE CACHE BOOL "Has SSE compile support")
        set(TRLC_HAS_AVX_SUPPORT FALSE CACHE BOOL "Has AVX compile support")
    endif()

    # Check for ARM NEON support
    if(TRLC_ARCHITECTURE_TYPE STREQUAL "arm64" OR TRLC_ARCHITECTURE_TYPE STREQUAL "arm")
        check_cxx_source_compiles("
            #include <arm_neon.h>
            int main() {
                float32x4_t a = vdupq_n_f32(0.0f);
                return 0;
            }
        " TRLC_HAS_NEON_COMPILE)
        
        set(TRLC_HAS_NEON_SUPPORT ${TRLC_HAS_NEON_COMPILE} CACHE BOOL "Has NEON compile support")
    else()
        set(TRLC_HAS_NEON_SUPPORT FALSE CACHE BOOL "Has NEON compile support")
    endif()

    message(DEBUG "Runtime features checked")
endfunction()

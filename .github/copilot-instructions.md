You are implementing trlc-platform, a header-only C++ library for compile-time platform detection and abstraction. This library provides:
- Compiler detection (GCC, Clang, MSVC, Intel)
- Platform detection (Windows, Linux, macOS, BSD)  
- Architecture detection (x86, x64, ARM, ARM64)
- C++ standard detection (C++17/20/23)
- Feature detection (exceptions, RTTI, threads, SIMD)
- Portable utility macros
- Debug and assertion utilities

Key requirements:
- Header-only implementation with constexpr functions
- C++17 minimum, C++20 feature detection
- Zero runtime overhead
- Cross-platform compatibility
- Follow the naming conventions provided

Naming Conventions:
- Classes/Structs/Enums: PascalCase (MyClass, MyStruct, MyEnum)
- Functions/Methods: camelCase (doSomething, calculateValue)
- Variables/Parameters/Members: snake_case (some_variable, member_data)
- Private/Protected members: underscore prefix (_internal_state)
- Constants: UPPER_CASE (MAX_BUFFER_SIZE)
- Enum constants: snake_case (file_opened, connection_closed)
- Namespaces: snake_case (trlc::platform)
- Macros: UPPER_CASE (TRLC_HAS_FEATURE)
- Template parameters: PascalCase (template<typename PlatformType>)

All code should be in namespace trlc::platform.
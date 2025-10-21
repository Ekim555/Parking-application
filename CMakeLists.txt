cmake_minimum_required(VERSION 3.10)

# -------------------------------
# Project Information
# -------------------------------
project(ParkingApp VERSION 1.0 LANGUAGES CXX)

# -------------------------------
# Compiler Standard Detection
# -------------------------------
# Try enabling C++17 if supported; fallback to C++14 for older GCC/MinGW
include(CheckCXXCompilerFlag)

check_cxx_compiler_flag("-std=c++17" COMPILER_SUPPORTS_CXX17)
check_cxx_compiler_flag("-std=c++14" COMPILER_SUPPORTS_CXX14)

if(COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_STANDARD 17)
    message(STATUS "✅ Using C++17 standard")
elseif(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_STANDARD 14)
    message(STATUS "⚙️  Falling back to C++14 standard")
else()
    message(FATAL_ERROR "❌ Your compiler does not support C++14 or newer.")
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------------
# Warning Level Settings
# -------------------------------
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# -------------------------------
# Target Executable
# -------------------------------
add_executable(ParkingApp
    main.cpp
)

# -------------------------------
# Include Directories
# -------------------------------
target_include_directories(ParkingApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# -------------------------------
# Output Directory
# -------------------------------
set_target_properties(ParkingApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# -------------------------------
# Custom Run Target (Optional)
# -------------------------------
# Enables "Run" from VSCode Command Palette or terminal:
#   cmake --build build --target run
add_custom_target(run
    COMMAND ParkingApp
    DEPENDS ParkingApp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

cmake_minimum_required(VERSION 3.20)
project(OrderBook VERSION 1.0.0 LANGUAGES CXX)

# Export compile commands for IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(BUILD_TESTS "Build test suite" ON)
option(BUILD_EXAMPLES "Build example programs" ON)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    add_compile_options(-O3 -march=native)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.cc")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Main library
add_library(orderbook_lib STATIC ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(orderbook_lib)

# Executable (if you have a main)
file(GLOB MAIN_SOURCES "src/main.cpp")
if(MAIN_SOURCES)
    add_executable(orderbook ${MAIN_SOURCES})
    target_link_libraries(orderbook orderbook_lib)
endif()

# Testing
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation
install(TARGETS orderbook_lib
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

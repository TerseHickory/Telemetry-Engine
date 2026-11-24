cmake_minimum_required(VERSION 3.20)
project(Telemetry-Engine LANGUAGES CXX)

# Enforce C++20 without non-standard GNU extensions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json automatically for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(Telemetry-Engine STATIC
    ./collector/generator.cpp
    ./collector/ring_buffer.cpp
    ./collector/writer_thread.cpp
)

add_executable(Telemetry-App ./main.cpp)
target_link_libraries(Telemetry-App PRIVATE Telemetry-Engine)

# High-signal C++ Core Guidelines warning flags for GCC & Clang
target_compile_options(Telemetry-Engine PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
    -Wimplicit-fallthrough
)

# Optional: Enable AddressSanitizer & UndefinedBehaviorSanitizer in Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(Telemetry-App PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(Telemetry-App PRIVATE -fsanitize=address,undefined)
endif()

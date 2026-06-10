cmake_minimum_required(VERSION 3.16)

project(PlayArchive VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization for Release builds
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Oi /Ot /GL")

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find Dependencies
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)
find_package(LibArchive REQUIRED)
find_package(ZSTD REQUIRED)
# xxHash usually needs to be added via FetchContent or find_path if not a standard package
# For now, let's assume it's available or we'll fetch it later.

include_directories(include)

add_subdirectory(src/core)
add_subdirectory(src/gui)
add_subdirectory(src/cli)

# Enable Testing
enable_testing()
add_subdirectory(tests)

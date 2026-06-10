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

include(FetchContent)

# Find or Fetch ZSTD
find_package(ZSTD QUIET)
if(NOT ZSTD_FOUND)
    FetchContent_Declare(
        zstd
        GIT_REPOSITORY https://github.com/facebook/zstd.git
        GIT_TAG v1.5.5
        SOURCE_SUBDIR build/cmake
    )
    set(ZSTD_BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
    set(ZSTD_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
    set(ZSTD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ZSTD_BUILD_STATIC ON CACHE BOOL "" FORCE)
    set(ZSTD_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(zstd)
    if(TARGET libzstd_static)
        add_library(ZSTD::ZSTD ALIAS libzstd_static)
    endif()
endif()

# Find or Fetch xxHash
find_package(xxHash QUIET)
if(NOT xxHash_FOUND)
    FetchContent_Declare(
        xxhash
        GIT_REPOSITORY https://github.com/Cyan4973/xxHash.git
        GIT_TAG v0.8.2
    )
    FetchContent_GetProperties(xxhash)
    if(NOT xxhash_POPULATED)
        FetchContent_Populate(xxhash)
        add_library(xxhash STATIC ${xxhash_SOURCE_DIR}/xxhash.c)
        target_include_directories(xxhash PUBLIC ${xxhash_SOURCE_DIR})
        add_library(xxHash::xxhash ALIAS xxhash)
    endif()
endif()

# Find Dependencies that are harder to fetch
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Concurrent)
find_package(LibArchive REQUIRED)

include_directories(include)

add_subdirectory(src/core)
add_subdirectory(src/gui)
add_subdirectory(src/cli)

# Enable Testing
enable_testing()

# Find or Fetch GTest
find_package(GTest QUIET)
if(NOT GTest_FOUND)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/03597a01eeeb94c40a374e578d123f885133645a.zip
    )
    # For Windows: do not override the build's MSVC runtime settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(tests)

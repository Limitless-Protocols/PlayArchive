set(CORE_SOURCES
    FormatManager.cpp
    GCFEngine.cpp
    LibArchiveHandler.cpp
    PlatformUtils.cpp
)

add_library(playarchive_core STATIC ${CORE_SOURCES})

if(WIN32)
    target_link_libraries(playarchive_core PRIVATE Shlwapi)
endif()

target_link_libraries(playarchive_core
    PUBLIC
    Qt6::Core
    LibArchive::LibArchive
    ZSTD::ZSTD
    xxHash::xxhash
)

target_include_directories(playarchive_core PUBLIC ${CMAKE_SOURCE_DIR}/include)

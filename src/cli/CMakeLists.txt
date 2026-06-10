add_executable(playarchive_cli main.cpp)

target_link_libraries(playarchive_cli
    PRIVATE
    playarchive_core
)

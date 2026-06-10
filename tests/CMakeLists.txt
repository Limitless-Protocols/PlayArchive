add_executable(playarchive_tests GCFTest.cpp)
target_link_libraries(playarchive_tests
    PRIVATE
    GTest::gtest_main
    playarchive_core
)

add_test(NAME GCFTests COMMAND playarchive_tests)

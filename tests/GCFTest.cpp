#include <gtest/gtest.h>
#include "playarchive/GCFEngine.h"
#include <filesystem>
#include <fstream>

using namespace PlayArchive;
namespace fs = std::filesystem;

TEST(GCFEngineTest, BasicCreateAndExtract) {
    GCFEngine engine;
    std::string testDir = "test_data";
    std::string testFile = testDir + "/hello.txt";
    std::string archivePath = "test.gcf";
    std::string extractPath = "extracted";

    fs::create_directories(testDir);
    std::ofstream(testFile) << "Hello PlayArchive!";

    std::vector<std::string> sources = { testDir };
    ASSERT_TRUE(engine.create(archivePath, sources));
    ASSERT_TRUE(fs::exists(archivePath));

    auto entries = engine.list(archivePath);
    ASSERT_EQ(entries.size(), 1);
    EXPECT_EQ(entries[0].path, "hello.txt");

    ASSERT_TRUE(engine.extract(archivePath, extractPath));
    ASSERT_TRUE(fs::exists(extractPath + "/hello.txt"));

    // Cleanup
    fs::remove_all(testDir);
    fs::remove_all(extractPath);
    fs::remove(archivePath);
}

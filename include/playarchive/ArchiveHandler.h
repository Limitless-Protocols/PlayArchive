#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace PlayArchive {

struct ArchiveEntry {
    std::string path;
    uint64_t size;
    bool isDirectory;
};

class IArchiveHandler {
public:
    virtual ~IArchiveHandler() = default;

    // Progress callback: (current_bytes, total_bytes, current_file)
    using ProgressCallback = std::function<void(uint64_t, uint64_t, const std::string&)>;

    virtual bool create(const std::string& archivePath, 
                        const std::vector<std::string>& sourcePaths,
                        ProgressCallback progress = nullptr) = 0;

    virtual bool extract(const std::string& archivePath, 
                         const std::string& destinationPath,
                         ProgressCallback progress = nullptr) = 0;

    virtual std::vector<ArchiveEntry> list(const std::string& archivePath) = 0;
    
    virtual std::string formatName() const = 0;
};

} // namespace PlayArchive

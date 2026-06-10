#pragma once

#include "ArchiveHandler.h"
#include <vector>
#include <string>
#include <map>

namespace PlayArchive {

class GCFEngine : public IArchiveHandler {
public:
    bool create(const std::string& archivePath, 
                const std::vector<std::string>& sourcePaths,
                ProgressCallback progress = nullptr) override;

    bool extract(const std::string& archivePath, 
                 const std::string& destinationPath,
                 ProgressCallback progress = nullptr) override;

    std::vector<ArchiveEntry> list(const std::string& archivePath) override;
    
    std::string formatName() const override { return "GCF (Game Compression Format)"; }

private:
    struct GCFHeader {
        char magic[4] = {'G', 'C', 'F', '1'};
        uint64_t entryTableOffset;
        uint32_t entryCount;
    };

    struct GCFEntry {
        std::string path;
        uint64_t originalSize;
        uint64_t compressedSize;
        uint64_t offset;
        bool isCompressed;
        uint32_t crc32;
    };

    bool isAlreadyCompressed(const std::string& extension);
};

} // namespace PlayArchive

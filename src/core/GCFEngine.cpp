#include "playarchive/GCFEngine.h"
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <set>

// Placeholder for Zstd and xxHash
// In a real build, we'd include <zstd.h> and <xxhash.h>
#define ZSTD_CLEVEL_DEFAULT 3

namespace fs = std::filesystem;

namespace PlayArchive {

bool GCFEngine::isAlreadyCompressed(const std::string& ext) {
    static const std::set<std::string> compressedExts = {
        ".png", ".jpg", ".jpeg", ".mp3", ".ogg", ".mp4", ".zip", ".7z", ".rar", ".gz"
    };
    std::string lowerExt = ext;
    std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), ::tolower);
    return compressedExts.count(lowerExt) > 0;
}

bool GCFEngine::create(const std::string& archivePath, 
                       const std::vector<std::string>& sourcePaths,
                       ProgressCallback progress) {
    std::ofstream outFile(archivePath, std::ios::binary);
    if (!outFile) return false;

    GCFHeader header;
    outFile.write(reinterpret_cast<char*>(&header), sizeof(header));

    std::vector<GCFEntry> entries;
    uint64_t currentOffset = sizeof(header);

    for (const auto& path : sourcePaths) {
        if (!fs::exists(path)) continue;

        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            if (entry.is_regular_file()) {
                GCFEntry gcfEntry;
                gcfEntry.path = fs::relative(entry.path(), path).string();
                gcfEntry.originalSize = fs::file_size(entry.path());
                gcfEntry.offset = currentOffset;
                
                std::ifstream inFile(entry.path(), std::ios::binary);
                std::vector<char> buffer(gcfEntry.originalSize);
                inFile.read(buffer.data(), gcfEntry.originalSize);

                if (isAlreadyCompressed(entry.path().extension().string())) {
                    gcfEntry.isCompressed = false;
                    gcfEntry.compressedSize = gcfEntry.originalSize;
                    outFile.write(buffer.data(), gcfEntry.originalSize);
                } else {
                    // Placeholder for Zstd compression
                    // size_t cSize = ZSTD_compress(cBuffer, cCapacity, buffer.data(), buffer.size(), ZSTD_CLEVEL_DEFAULT);
                    gcfEntry.isCompressed = true;
                    gcfEntry.compressedSize = gcfEntry.originalSize; // Placeholder
                    outFile.write(buffer.data(), gcfEntry.originalSize); // Placeholder: writing raw for now
                }

                currentOffset += gcfEntry.compressedSize;
                entries.push_back(gcfEntry);

                if (progress) {
                    progress(currentOffset, 0, gcfEntry.path); // Total size 0 as we don't know it yet
                }
            }
        }
    }

    header.entryTableOffset = currentOffset;
    header.entryCount = static_cast<uint32_t>(entries.size());

    // Write entry table
    for (const auto& entry : entries) {
        uint32_t pathLen = static_cast<uint32_t>(entry.path.length());
        outFile.write(reinterpret_cast<const char*>(&pathLen), sizeof(pathLen));
        outFile.write(entry.path.c_str(), pathLen);
        outFile.write(reinterpret_cast<const char*>(&entry.originalSize), sizeof(entry.originalSize));
        outFile.write(reinterpret_cast<const char*>(&entry.compressedSize), sizeof(entry.compressedSize));
        outFile.write(reinterpret_cast<const char*>(&entry.offset), sizeof(entry.offset));
        outFile.write(reinterpret_cast<const char*>(&entry.isCompressed), sizeof(entry.isCompressed));
    }

    // Update header at the beginning
    outFile.seekp(0);
    outFile.write(reinterpret_cast<char*>(&header), sizeof(header));

    return true;
}

bool GCFEngine::extract(const std::string& archivePath, 
                        const std::string& destinationPath,
                        ProgressCallback progress) {
    std::ifstream inFile(archivePath, std::ios::binary);
    if (!inFile) return false;

    GCFHeader header;
    inFile.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (std::string(header.magic, 4) != "GCF1") return false;

    inFile.seekg(header.entryTableOffset);
    std::vector<GCFEntry> entries;
    for (uint32_t i = 0; i < header.entryCount; ++i) {
        GCFEntry entry;
        uint32_t pathLen;
        inFile.read(reinterpret_cast<char*>(&pathLen), sizeof(pathLen));
        std::vector<char> pathBuf(pathLen);
        inFile.read(pathBuf.data(), pathLen);
        entry.path = std::string(pathBuf.data(), pathLen);
        inFile.read(reinterpret_cast<char*>(&entry.originalSize), sizeof(entry.originalSize));
        inFile.read(reinterpret_cast<char*>(&entry.compressedSize), sizeof(entry.compressedSize));
        inFile.read(reinterpret_cast<char*>(&entry.offset), sizeof(entry.offset));
        inFile.read(reinterpret_cast<char*>(&entry.isCompressed), sizeof(entry.isCompressed));
        entries.push_back(entry);
    }

    for (const auto& entry : entries) {
        fs::path outPath = fs::path(destinationPath) / entry.path;
        fs::create_directories(outPath.parent_path());

        std::ofstream outFile(outPath, std::ios::binary);
        inFile.seekg(entry.offset);
        
        std::vector<char> buffer(entry.compressedSize);
        inFile.read(buffer.data(), entry.compressedSize);

        if (entry.isCompressed) {
            // Placeholder for Zstd decompression
            // ZSTD_decompress(dBuffer, dCapacity, buffer.data(), buffer.size());
            outFile.write(buffer.data(), entry.compressedSize); // Placeholder
        } else {
            outFile.write(buffer.data(), entry.compressedSize);
        }

        if (progress) {
            progress(0, 0, entry.path);
        }
    }

    return true;
}

std::vector<ArchiveEntry> GCFEngine::list(const std::string& archivePath) {
    std::ifstream inFile(archivePath, std::ios::binary);
    std::vector<ArchiveEntry> result;
    if (!inFile) return result;

    GCFHeader header;
    inFile.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (std::string(header.magic, 4) != "GCF1") return result;

    inFile.seekg(header.entryTableOffset);
    for (uint32_t i = 0; i < header.entryCount; ++i) {
        uint32_t pathLen;
        inFile.read(reinterpret_cast<char*>(&pathLen), sizeof(pathLen));
        std::vector<char> pathBuf(pathLen);
        inFile.read(pathBuf.data(), pathLen);
        
        ArchiveEntry ae;
        ae.path = std::string(pathBuf.data(), pathLen);
        inFile.read(reinterpret_cast<char*>(&ae.size), sizeof(ae.size));
        ae.isDirectory = false;
        
        // Skip rest of entry fields
        inFile.seekg(sizeof(uint64_t) + sizeof(uint64_t) + sizeof(bool), std::ios::cur);
        
        result.push_back(ae);
    }
    return result;
}

} // namespace PlayArchive

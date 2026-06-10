#pragma once

#include "ArchiveHandler.h"
#include <archive.h>
#include <archive_entry.h>

namespace PlayArchive {

class LibArchiveHandler : public IArchiveHandler {
public:
    LibArchiveHandler(const std::string& format);

    bool create(const std::string& archivePath, 
                const std::vector<std::string>& sourcePaths,
                ProgressCallback progress = nullptr) override;

    bool extract(const std::string& archivePath, 
                 const std::string& destinationPath,
                 ProgressCallback progress = nullptr) override;

    std::vector<ArchiveEntry> list(const std::string& archivePath) override;
    
    std::string formatName() const override { return "LibArchive: " + m_format; }

private:
    std::string m_format;
};

} // namespace PlayArchive

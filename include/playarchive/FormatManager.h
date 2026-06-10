#pragma once

#include "ArchiveHandler.h"
#include <map>
#include <string>
#include <memory>

namespace PlayArchive {

class FormatManager {
public:
    static FormatManager& instance();

    void registerHandler(const std::string& extension, std::shared_ptr<IArchiveHandler> handler);
    std::shared_ptr<IArchiveHandler> getHandler(const std::string& archivePath);

private:
    FormatManager() = default;
    std::map<std::string, std::shared_ptr<IArchiveHandler>> handlers;
};

} // namespace PlayArchive

#include "playarchive/FormatManager.h"
#include <filesystem>
#include <algorithm>

namespace PlayArchive {

FormatManager& FormatManager::instance() {
    static FormatManager inst;
    return inst;
}

void FormatManager::registerHandler(const std::string& extension, std::shared_ptr<IArchiveHandler> handler) {
    handlers[extension] = handler;
}

std::shared_ptr<IArchiveHandler> FormatManager::getHandler(const std::string& archivePath) {
    std::string ext = std::filesystem::path(archivePath).extension().string();
    if (!ext.empty() && ext[0] == '.') {
        ext = ext.substr(1);
    }
    
    // Normalize to lowercase
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    auto it = handlers.find(ext);
    if (it != handlers.end()) {
        return it->second;
    }
    return nullptr;
}

} // namespace PlayArchive

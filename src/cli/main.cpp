#include <iostream>
#include <string>
#include <vector>
#include "playarchive/FormatManager.h"
#include "playarchive/GCFEngine.h"
#include "playarchive/LibArchiveHandler.h"

using namespace PlayArchive;

void printUsage() {
    std::cout << "Usage: playarchive-cli <command> <archive_path> [source_paths/dest_path]\n";
    std::cout << "Commands:\n";
    std::cout << "  create <archive> <source1> [source2...] : Create an archive\n";
    std::cout << "  extract <archive> <destination>         : Extract an archive\n";
    std::cout << "  list <archive>                          : List archive contents\n";
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printUsage();
        return 1;
    }

    // Register handlers
    auto& fm = FormatManager::instance();
    fm.registerHandler("gcf", std::make_shared<GCFEngine>());
    fm.registerHandler("zip", std::make_shared<LibArchiveHandler>("zip"));
    fm.registerHandler("7z", std::make_shared<LibArchiveHandler>("7z"));
    fm.registerHandler("tar", std::make_shared<LibArchiveHandler>("tar"));

    std::string cmd = argv[1];
    std::string archivePath = argv[2];

    auto handler = fm.getHandler(archivePath);
    if (!handler) {
        std::cerr << "Error: Unsupported format for " << archivePath << "\n";
        return 1;
    }

    auto progress = [](uint64_t cur, uint64_t total, const std::string& file) {
        std::cout << "Processing: " << file << "\r" << std::flush;
    };

    if (cmd == "create") {
        if (argc < 4) {
            printUsage();
            return 1;
        }
        std::vector<std::string> sources;
        for (int i = 3; i < argc; ++i) sources.push_back(argv[i]);
        if (handler->create(archivePath, sources, progress)) {
            std::cout << "\nArchive created successfully.\n";
        } else {
            std::cerr << "\nFailed to create archive.\n";
            return 1;
        }
    } else if (cmd == "extract") {
        if (argc < 4) {
            printUsage();
            return 1;
        }
        std::string dest = argv[3];
        if (handler->extract(archivePath, dest, progress)) {
            std::cout << "\nExtraction complete.\n";
        } else {
            std::cerr << "\nFailed to extract archive.\n";
            return 1;
        }
    } else if (cmd == "list") {
        auto entries = handler->list(archivePath);
        for (const auto& entry : entries) {
            std::cout << (entry.isDirectory ? "[D] " : "[F] ") << entry.path << " (" << entry.size << " bytes)\n";
        }
    } else {
        printUsage();
        return 1;
    }

    return 0;
}

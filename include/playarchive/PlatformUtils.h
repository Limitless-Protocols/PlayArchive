#pragma once
#include <string>
#include <vector>

namespace PlayArchive {

class PlatformUtils {
public:
    static bool checkContextMenuInstalled();
    static void installContextMenu();
    static std::string getExecutablePath();
    static std::string getCLIExecutablePath();
};

} // namespace PlayArchive

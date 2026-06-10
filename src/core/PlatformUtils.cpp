#include "playarchive/PlatformUtils.h"
#include <windows.h>
#include <shlwapi.h>
#include <iostream>
#include <filesystem>

#pragma comment(lib, "Shlwapi.lib")

namespace fs = std::filesystem;

namespace PlayArchive {

std::string PlatformUtils::getExecutablePath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::string(buffer);
}

std::string PlatformUtils::getCLIExecutablePath() {
    fs::path guiPath = getExecutablePath();
    fs::path cliPath = guiPath.parent_path() / "playarchive.exe";
    return cliPath.string();
}

bool PlatformUtils::checkContextMenuInstalled() {
    HKEY hKey;
    std::string subkey = "Software\\Classes\\AllFilesystemObjects\\shell\\PlayArchive";
    if (RegOpenKeyExA(HKEY_CURRENT_USER, subkey.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return true;
    }
    return false;
}

void setRegistryValue(const std::string& subkey, const std::string& valueName, const std::string& data) {
    HKEY hKey;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, subkey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueExA(hKey, valueName.empty() ? NULL : valueName.c_str(), 0, REG_SZ, (const BYTE*)data.c_str(), (DWORD)data.length() + 1);
        RegCloseKey(hKey);
    }
}

void PlatformUtils::installContextMenu() {
    std::string guiPath = getExecutablePath();
    std::string cliPath = getCLIExecutablePath();
    
    // Main Cascading Menu
    std::string rootKey = "Software\\Classes\\AllFilesystemObjects\\shell\\PlayArchive";
    setRegistryValue(rootKey, "MUIVerb", "PlayArchive");
    setRegistryValue(rootKey, "SubCommands", "");
    setRegistryValue(rootKey, "Icon", guiPath);

    // Sub-menu: Compress to GCF
    std::string compressKey = rootKey + "\\shell\\CompressGCF";
    setRegistryValue(compressKey, "MUIVerb", "Compress to GCF");
    setRegistryValue(compressKey + "\\command", "", "\"" + cliPath + "\" create \"%1.gcf\" \"%1\"");

    // Sub-menu: Add to archive... (GUI)
    std::string guiAddKey = rootKey + "\\shell\\AddGUI";
    setRegistryValue(guiAddKey, "MUIVerb", "Add to archive...");
    setRegistryValue(guiAddKey + "\\command", "", "\"" + guiPath + "\" --add \"%1\"");

    // File Association for .gcf
    setRegistryValue("Software\\Classes\\.gcf", "", "PlayArchive.GCF");
    std::string assocKey = "Software\\Classes\\PlayArchive.GCF\\shell\\PlayArchive";
    setRegistryValue(assocKey, "MUIVerb", "PlayArchive");
    setRegistryValue(assocKey, "SubCommands", "");
    setRegistryValue(assocKey, "Icon", guiPath);

    // Sub-menu for .gcf: Extract Here
    std::string extractHereKey = assocKey + "\\shell\\ExtractHere";
    setRegistryValue(extractHereKey, "MUIVerb", "Extract Here");
    setRegistryValue(extractHereKey + "\\command", "", "\"" + cliPath + "\" extract \"%1\" \".\"");

    // Sub-menu for .gcf: Extract to folder
    std::string extractFolderKey = assocKey + "\\shell\\ExtractFolder";
    setRegistryValue(extractFolderKey, "MUIVerb", "Extract to folder");
    setRegistryValue(extractFolderKey + "\\command", "", "\"" + cliPath + "\" extract \"%1\" \"%1_extracted\"");
}

} // namespace PlayArchive

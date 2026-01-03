#include "path_helper.hpp"
#include <windows.h>
#include <string>

std::string getExecutableDir() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string fullPath(buffer);
    
    // Remove executable name to get directory
    size_t pos = fullPath.find_last_of("\\/");
    std::string exeDir = fullPath.substr(0, pos);
    
    // Go up one level from build/ to project root
    pos = exeDir.find_last_of("\\/");
    std::string projectRoot = exeDir.substr(0, pos);
    
    return projectRoot;  // This is now Git-Lite directory
}
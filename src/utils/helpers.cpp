#include <sys/stat.h>
#include <direct.h>
#include "helpers.h"

void makeDir(const std::string& path) {
#ifdef _WIN32
    _mkdir(path.c_str());
#else
    mkdir(path.c_str(), 0777);
#endif
}
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <direct.h>
#include "init.h"
#include "../utils/helpers.h"

using namespace std;


// void makeDir(const string& path) {
// #ifdef _WIN32
//     _mkdir(path.c_str());
// #else
//     mkdir(path.c_str(), 0777);
// #endif
// }

void vcs_init(const string& repoName) {
    string repoPath = repoName;
    string gitlitePath = repoPath + "/.gitlite";

    ifstream check((gitlitePath + "/HEAD").c_str());
    if (check.good()) {
        cout << "Reinitialized existing Git-Lite repository in " << gitlitePath << "\n";
        return;
    }

    makeDir(repoPath);
    makeDir(gitlitePath);
    makeDir(gitlitePath + "/objects");
    makeDir(gitlitePath + "/commits");
    makeDir(gitlitePath + "/branches");

    ofstream head(gitlitePath + "/HEAD");
    head << "ref: refs/heads/main\n";
    head.close();

    ofstream mainBranch(gitlitePath + "/branches/main");
    mainBranch << "null";
    mainBranch.close();

    cout << "Initialized empty Git-Lite repository in " << gitlitePath << "\n";
}

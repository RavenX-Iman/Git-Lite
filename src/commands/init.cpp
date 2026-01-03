#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <direct.h>
#include <cstdlib>
#include "init.h"
#include "../utils/helpers.h"
#include "../utils/path_helper.hpp"  // ADD THIS LINE

using namespace std;

// REMOVE the getExecutableDir() function completely

void vcs_init(const string& repoName) {
    string repoPath = repoName;
    string gitlitePath = repoPath + "/.gitlite";

    ifstream check((gitlitePath + "/HEAD").c_str());
    if (check.good()) {
        cout << "Reinitialized existing Git-Lite repository in " << gitlitePath << "\n";
        return;
    }

    // Create directory structure
    makeDir(repoPath);
    makeDir(gitlitePath);
    makeDir(gitlitePath + "/objects");
    makeDir(gitlitePath + "/commits");
    makeDir(gitlitePath + "/branches");
    makeDir(gitlitePath + "/keys");

    // Create HEAD file
    ofstream head(gitlitePath + "/HEAD");
    head << "ref: refs/heads/main\n";
    head.close();

    // Create main branch
    ofstream mainBranch(gitlitePath + "/branches/main");
    mainBranch << "null";
    mainBranch.close();

// Generate HMAC key
string projectRoot = getExecutableDir();
string scriptPath = projectRoot + "\\sha\\generate_hmac_key.py";  // Direct path
string absoluteKeyPath = gitlitePath + "/keys/hmac.key";
string keyGenCommand = "python \"" + scriptPath + "\" \"" + absoluteKeyPath + "\"";
    
    int keyGenResult = system(keyGenCommand.c_str());
    
    if (keyGenResult != 0) {
        cout << "Warning: HMAC key generation failed. Commit signing disabled.\n";
    }

    cout << "Initialized empty Git-Lite repository in " << gitlitePath << "\n";
}
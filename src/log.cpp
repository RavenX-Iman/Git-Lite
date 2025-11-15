#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <dirent.h>     // <-- important
#include "log.h"
using namespace std;

void vcs_log() {
    string commitFolder = ".gitlite/commits";

    DIR* dir = opendir(commitFolder.c_str());
    if (!dir) {
        cout << "No commits found.\n";
        return;
    }

    vector<string> files;
    dirent* entry;

    while ((entry = readdir(dir)) != NULL) {
        string name = entry->d_name;

        // Only read *.txt files
        if (name.size() > 4 && name.substr(name.size() - 4) == ".txt") {
            files.push_back(commitFolder + "/" + name);
        }
    }
    closedir(dir);

    if (files.empty()) {
        cout << "No commits yet.\n";
        return;
    }

    // Sort newest first (alphabetical works because hash varies)
    sort(files.rbegin(), files.rend());

    cout << "======= Commit History =======\n\n";

    for (string file : files) {
        ifstream in(file);
        if (!in) continue;

        cout << "-----------------------------\n";
        string line;

        while (getline(in, line)) {
            cout << line << endl;
        }

        cout << "-----------------------------\n\n";
        in.close();
    }
}

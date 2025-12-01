#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dirent.h>
    #include <sys/stat.h>
#endif

#include "status.h"
#include "../utils/helpers.h"

using namespace std;

// Read entire file content
string getFileContent(const string &path)
{
    ifstream file(path, ios::binary);
    if (!file) return "";
    return string((istreambuf_iterator<char>(file)),
                   istreambuf_iterator<char>());
}

// NEW: Read metadata of last commit
unordered_map<string, string> readCommitMeta(const string& commitHash)
{
    unordered_map<string, string> meta;
    string path = ".gitlite/commits/" + commitHash + "/meta";

    ifstream file(path);
    if (!file) return meta;

    string line;
    while (getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != string::npos) {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            if (!value.empty() && value[0] == ' ')
                value.erase(0, 1); // remove leading space
            meta[key] = value;
        }
    }
    return meta;
}

void vcs_status()
{
    string indexPath = ".gitlite/index";
    unordered_map<string, string> stagedFiles; // filename -> hash

    cout << "=== Status ===\n\n";

    // NEW: Show last commit information
    string lastCommitHash;
    {
        ifstream head(".gitlite/HEAD");
        if (head) getline(head, lastCommitHash);
    }

    if (!lastCommitHash.empty()) {
        auto meta = readCommitMeta(lastCommitHash);

        cout << "Last Commit:\n";
        cout << "  Hash:      " << lastCommitHash << "\n";
        if (meta.count("author"))
            cout << "  Author:    " << meta["author"] << "\n";
        if (meta.count("ip"))
            cout << "  IP:        " << meta["ip"] << "\n";
        if (meta.count("timestamp"))
            cout << "  Time:      " << meta["timestamp"] << "\n";
        cout << "\n";
    }

    // Read staged files from index
    ifstream index(indexPath);
    if (!index) {
        cout << "Nothing staged (run 'vcs add <file>' to stage files)\n";
        return;
    }

    string line;
    while (getline(index, line)) {
        if (line.empty()) continue;
        
        size_t spacePos = line.find(' ');
        if (spacePos != string::npos) {
            string hash = line.substr(0, spacePos);
            string filename = line.substr(spacePos + 1);
            stagedFiles[filename] = hash;
        }
    }
    index.close();

    if (stagedFiles.empty()) {
        cout << "Nothing staged for commit\n";
    } else {
        cout << "Changes to be committed:\n";
        for (const auto& entry : stagedFiles) {
            cout << "  new file:   " << entry.first << "\n";
        }
    }

    cout << "\n";

    // Check for untracked files
    unordered_set<string> untrackedFiles;

#ifdef _WIN32
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA("*", &findData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            string name = findData.cFileName;
            if (name != "." && name != ".." && name != ".gitlite") {
                if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    if (stagedFiles.find(name) == stagedFiles.end()) {
                        untrackedFiles.insert(name);
                    }
                }
            }
        } while (FindNextFileA(hFind, &findData) != 0);
        FindClose(hFind);
    }
#else
    DIR* dir = opendir(".");
    if (dir) {
        dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            string name = entry->d_name;
            if (name != "." && name != ".." && name != ".gitlite") {
                struct stat st;
                if (stat(name.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
                    if (stagedFiles.find(name) == stagedFiles.end()) {
                        untrackedFiles.insert(name);
                    }
                }
            }
        }
        closedir(dir);
    }
#endif

    if (!untrackedFiles.empty()) {
        cout << "Untracked files:\n";
        cout << "  (use \"vcs add <file>\" to track)\n\n";
        for (const string& file : untrackedFiles) {
            cout << "  " << file << "\n";
        }
    }

    cout << "\n";
}

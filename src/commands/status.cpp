#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <vector>

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

// Read last commit from current branch
string getLastCommitHash()
{
    // Read HEAD to get current branch
    ifstream headFile(".gitlite/HEAD");
    if (!headFile) return "";
    
    string refPath;
    getline(headFile, refPath);
    headFile.close();

    // Extract branch name from "ref: refs/heads/main"
    string branchName = "main";
    size_t pos = refPath.find("refs/heads/");
    if (pos != string::npos)
        branchName = refPath.substr(pos + 11);

    // Read branch file to get commit hash
    string branchPath = ".gitlite/branches/" + branchName;
    ifstream branchFile(branchPath);
    if (!branchFile) return "";
    
    string commitHash;
    getline(branchFile, commitHash);
    branchFile.close();

    if (commitHash == "null") return "";
    return commitHash;
}

// Read commit metadata from .txt file
unordered_map<string, string> readCommitMeta(const string& commitHash)
{
    unordered_map<string, string> meta;
    if (commitHash.empty()) return meta;
    
    string path = ".gitlite/commits/" + commitHash + ".txt";

    ifstream file(path);
    if (!file) return meta;

    string line;
    while (getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != string::npos) {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            
            // Trim leading space
            if (!value.empty() && value[0] == ' ')
                value.erase(0, 1);
            
            meta[key] = value;
        }
    }
    return meta;
}

// Get files from a specific commit
unordered_set<string> getFilesFromCommit(const string& commitHash)
{
    unordered_set<string> files;
    if (commitHash.empty() || commitHash == "null") return files;
    
    string path = ".gitlite/commits/" + commitHash + ".txt";
    ifstream file(path);
    if (!file) return files;

    string line;
    bool inFilesSection = false;
    
    while (getline(file, line)) {
        if (line == "files:") {
            inFilesSection = true;
            continue;
        }
        
        if (inFilesSection && !line.empty()) {
            // Line format: "  hash filename"
            // Remove leading spaces
            size_t start = line.find_first_not_of(" \t");
            if (start != string::npos) {
                line = line.substr(start);
                
                // Extract filename (after hash)
                size_t spacePos = line.find(' ');
                if (spacePos != string::npos) {
                    string filename = line.substr(spacePos + 1);
                    files.insert(filename);
                }
            }
        }
    }
    
    return files;
}

// Get ALL committed files by traversing entire commit history
unordered_set<string> getAllCommittedFiles(const string& startCommitHash)
{
    unordered_set<string> allFiles;
    if (startCommitHash.empty() || startCommitHash == "null") return allFiles;
    
    string currentHash = startCommitHash;
    
    // Traverse through commit chain from latest to first
    while (!currentHash.empty() && currentHash != "null") {
        // Get files from current commit
        auto filesInCommit = getFilesFromCommit(currentHash);
        allFiles.insert(filesInCommit.begin(), filesInCommit.end());
        
        // Get parent commit
        auto meta = readCommitMeta(currentHash);
        currentHash = meta["parent"];
    }
    
    return allFiles;
}

void vcs_status()
{
    string indexPath = ".gitlite/index";
    unordered_map<string, string> stagedFiles; // filename -> hash

    cout << "=== Status ===\n\n";

    // Get and show last commit information
    string lastCommitHash = getLastCommitHash();
    
    if (!lastCommitHash.empty()) {
        auto meta = readCommitMeta(lastCommitHash);

        cout << "Last Commit:\n";
        cout << "  Hash:      " << lastCommitHash << "\n";
        if (meta.count("author"))
            cout << "  Author:    " << meta["author"] << "\n";
        if (meta.count("author_ip"))
            cout << "  IP:        " << meta["author_ip"] << "\n";
        if (meta.count("date"))
            cout << "  Date:      " << meta["date"] << "\n";
        if (meta.count("message"))
            cout << "  Message:   " << meta["message"] << "\n";
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

    // Show staged files
    if (stagedFiles.empty()) {
        cout << "Nothing staged for commit\n";
    } else {
        cout << "Changes to be committed:\n";
        for (const auto& entry : stagedFiles) {
            cout << "  new file:   " << entry.first << "\n";
        }
    }

    cout << "\n";

    // Get ALL committed files from entire history
    unordered_set<string> committedFiles = getAllCommittedFiles(lastCommitHash);

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
                    // Only show as untracked if NOT staged AND NOT committed
                    if (stagedFiles.find(name) == stagedFiles.end() &&
                        committedFiles.find(name) == committedFiles.end()) {
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
                    // Only show as untracked if NOT staged AND NOT committed
                    if (stagedFiles.find(name) == stagedFiles.end() &&
                        committedFiles.find(name) == committedFiles.end()) {
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
    } else if (stagedFiles.empty()) {
        cout << "Working directory clean\n";
    }

    cout << "\n";
}
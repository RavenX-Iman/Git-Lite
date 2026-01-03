#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <windows.h>
#include <dirent.h>
#include "../utils/path_helper.hpp"  // ADD THIS LINE
#include "log.h"
using namespace std;



// -----------------------------
// VERIFY HMAC SIGNATURE
// -----------------------------
string verifyHMACSignature(const string& commitData, const string& storedSignature, const string& keyPath) {
    // Get project root
    string projectRoot = getExecutableDir();
    string scriptPath = projectRoot + "\\sha\\hmac_verify.py";
    
    // Write commit data to temp file
    ofstream tempData("tmp_verify_data.txt");
    tempData << commitData;
    tempData.close();
    
    // Build command - signature as argument, data via stdin
    string command = "python \"" + scriptPath + "\" \"" + keyPath + "\" \"" + storedSignature + "\" < tmp_verify_data.txt > tmp_verify_result.txt 2>&1";
    
    system(command.c_str());
    
    // Read verification result
    string verificationResult;
    ifstream infile("tmp_verify_result.txt");
    if (infile.is_open()) {
        getline(infile, verificationResult);
        infile.close();
    }
    
    // Clean up
    remove("tmp_verify_result.txt");
    remove("tmp_verify_data.txt");
    
    return verificationResult;
}

// -----------------------------
// PARSE COMMIT FILE
// -----------------------------
struct CommitInfo {
    string commitHash;
    string parent;
    string message;
    string author;
    string authorIP;
    string date;
    string signature;
    string files;
};

CommitInfo parseCommitFile(const string& filePath) {
    CommitInfo info;
    ifstream in(filePath);
    string line;
    bool inFiles = false;
    
    while (getline(in, line)) {
        if (line.find("commit: ") == 0) {
            info.commitHash = line.substr(8);
        }
        else if (line.find("parent: ") == 0) {
            info.parent = line.substr(8);
        }
        else if (line.find("message: ") == 0) {
            info.message = line.substr(9);
        }
        else if (line.find("author: ") == 0) {
            info.author = line.substr(8);
        }
        else if (line.find("author_ip: ") == 0) {
            info.authorIP = line.substr(11);
        }
        else if (line.find("date: ") == 0) {
            info.date = line.substr(6);
        }
        else if (line.find("signature: ") == 0) {
            info.signature = line.substr(11);
        }
        else if (line.find("files:") == 0) {
            inFiles = true;
        }
        else if (inFiles) {
            info.files += line + "\n";
        }
    }
    
    in.close();
    return info;
}

// -----------------------------
// MAIN LOG FUNCTION
// -----------------------------
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

        if (name.size() > 4 && name.substr(name.size() - 4) == ".txt") {
            files.push_back(commitFolder + "/" + name);
        }
    }
    closedir(dir);

    if (files.empty()) {
        cout << "No commits yet.\n";
        return;
    }

    // Sort newest first
    sort(files.rbegin(), files.rend());

    cout << "======= Commit History =======\n\n";
    
    // Check if HMAC key exists
    string keyPath = ".gitlite/keys/hmac.key";
    ifstream keyCheck(keyPath);
    bool keyExists = keyCheck.good();
    keyCheck.close();

    for (string file : files) {
        CommitInfo commit = parseCommitFile(file);
        
        cout << "-----------------------------\n";
        cout << "commit: " << commit.commitHash << "\n";
        cout << "parent: " << commit.parent << "\n";
        cout << "message: " << commit.message << "\n";
        cout << "author: " << commit.author << "\n";
        cout << "author_ip: " << commit.authorIP << "\n";
        cout << "date: " << commit.date << "\n";
        
        // ========================================
        // VERIFY SIGNATURE
        // ========================================
        if (commit.signature == "unsigned") {
            cout << "signature: UNSIGNED\n";
        }
        else if (commit.signature == "signing_failed") {
            cout << "signature: SIGNING FAILED\n";
        }
        else if (!keyExists) {
            cout << "signature: CANNOT VERIFY (key missing)\n";
        }
        else {
            // Reconstruct canonical data
            stringstream canonicalData;
            canonicalData << "commit:" << commit.commitHash << "|"
                         << "parent:" << commit.parent << "|"
                         << "author:" << commit.author << "|"
                         << "author_ip:" << commit.authorIP << "|"
                         << "date:" << commit.date << "|"
                         << "message:" << commit.message << "|"
                         << "files:" << commit.files;
            
            string commitDataForHMAC = canonicalData.str();
            
            // Verify signature
            string verifyResult = verifyHMACSignature(commitDataForHMAC, commit.signature, keyPath);
            
            if (verifyResult == "VALID") {
                cout << "signature: VALID (verified)\n";
            } else if (verifyResult.find("ERROR") != string::npos) {
                cout << "signature: VERIFICATION ERROR\n";
            } else {
                cout << "signature: INVALID (TAMPERED)\n";
                cout << "WARNING: This commit has been modified!\n";
            }
        }
        // ========================================
        
        cout << "files:\n" << commit.files;
        cout << "-----------------------------\n\n";
    }
}
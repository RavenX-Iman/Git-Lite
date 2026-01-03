#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "../utils/path_helper.hpp"  // ADD THIS LINE
#pragma comment(lib, "ws2_32.lib")

using namespace std;

// -----------------------------
// HELPER FUNCTIONS
// -----------------------------
string getCurrentTime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

string getAuthorName()
{
    char username[256];
    DWORD size = sizeof(username);

    if (GetUserNameA(username, &size))
        return string(username);

    return "Unknown User";
}

string getIPAddress()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    char host[256];
    gethostname(host, sizeof(host));

    struct hostent *host_entry = gethostbyname(host);
    char *ip = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));

    WSACleanup();
    return string(ip);
}



// -----------------------------
// GET HMAC SIGNATURE
// -----------------------------
string getHMACSignature(const string& commitData, const string& keyPath) {
    // Get project root (Git-Lite directory)
    string projectRoot = getExecutableDir();
    string scriptPath = projectRoot + "\\sha\\hmac_sign.py";
    
    // Write commit data to temp file
    ofstream tempData("tmp_commit_data.txt");
    tempData << commitData;
    tempData.close();
    
    // Build command - pass data via stdin (< redirection)
    string command = "python \"" + scriptPath + "\" \"" + keyPath + "\" < tmp_commit_data.txt > tmp_hmac.txt 2>&1";
    
    system(command.c_str());
    
    // Read signature from output file
    string signature;
    ifstream infile("tmp_hmac.txt");
    if (infile.is_open()) {
        string line;
        while (getline(infile, line)) {
            // Only take lines that look like valid signatures (64 hex chars)
            if (line.length() == 64 && line.find("ERROR") == string::npos && line.find("Usage") == string::npos) {
                signature = line;
                break;
            }
        }
        infile.close();
    }
    
    // Clean up
    remove("tmp_hmac.txt");
    remove("tmp_commit_data.txt");
    
    return signature;
}

// -----------------------------
// MAIN COMMIT FUNCTION
// -----------------------------
void vcs_commit(const string &message)
{
    string indexPath = ".gitlite/index";
    ifstream index(indexPath);

    if (!index)
    {
        cout << "Nothing to commit (index file missing or empty).\n";
        return;
    }

    string line, commitContent;
    bool hasFiles = false;

    // Get commit metadata
    string authorName = getAuthorName(); 
    string ip = getIPAddress();
    string timestamp = getCurrentTime();

    // Build file list
    string filesList;
    while (getline(index, line))
    {
        if (!line.empty())
        {
            filesList += "  " + line + "\n";
            hasFiles = true;
        }
    }
    index.close();

    if (!hasFiles)
    {
        cout << "Nothing to commit.\n";
        return;
    }

    // Get parent commit
    ifstream headFile(".gitlite/HEAD");
    string refPath;
    getline(headFile, refPath);
    headFile.close();

    string branchName = "main";
    size_t pos = refPath.find("refs/heads/");
    if (pos != string::npos)
        branchName = refPath.substr(pos + 11);

    string branchPath = ".gitlite/branches/" + branchName;
    ifstream branch(branchPath);
    string parent;
    getline(branch, parent);
    branch.close();

    if (parent.empty() || parent == "null")
        parent = "null";

    // Generate commit hash
    string preHashContent = message + authorName + ip + timestamp + filesList;
    size_t commitHashValue = hash<string>{}(preHashContent + to_string(time(0)));
    string commitHash = to_string(commitHashValue);

    // ========================================
    // CREATE CANONICAL COMMIT STRING FOR HMAC
    // ========================================
    stringstream canonicalData;
    canonicalData << "commit:" << commitHash << "|"
                  << "parent:" << parent << "|"
                  << "author:" << authorName << "|"
                  << "author_ip:" << ip << "|"
                  << "date:" << timestamp << "|"
                  << "message:" << message << "|"
                  << "files:" << filesList;
    
    string commitDataForHMAC = canonicalData.str();
    
    // Generate HMAC signature
    string signature = "unsigned";
    string keyPath = ".gitlite/keys/hmac.key";
    
    ifstream keyCheck(keyPath);
    if (keyCheck.good()) {
        keyCheck.close();
        signature = getHMACSignature(commitDataForHMAC, keyPath);
        
        if (signature.empty() || signature.find("ERROR") != string::npos) {
            signature = "signing_failed";
        }
    }
    // ========================================

    // Build final commit content
    commitContent = "commit: " + commitHash + "\n";
    commitContent += "parent: " + parent + "\n";
    commitContent += "message: " + message + "\n";
    commitContent += "author: " + authorName + "\n";
    commitContent += "author_ip: " + ip + "\n";
    commitContent += "date: " + timestamp + "\n";
    commitContent += "signature: " + signature + "\n";
    commitContent += "files:\n" + filesList;

    // Save commit file
    string commitFile = ".gitlite/commits/" + commitHash + ".txt";
    ofstream commitOut(commitFile);
    commitOut << commitContent;
    commitOut.close();

    // Update branch pointer
    ofstream branchOut(branchPath);
    branchOut << commitHash;
    branchOut.close();

    // Clear index
    ofstream clearIndex(indexPath, ios::trunc);
    clearIndex.close();

    // Output success message
    cout << "Committed as " << commitHash << ": " << message << "\n";
    
    if (signature != "unsigned" && signature != "signing_failed") {
        cout << "Signature: " << signature.substr(0, 16) << "...\n";
    } else if (signature == "unsigned") {
        cout << "Warning: Commit unsigned (no HMAC key found)\n";
    } else {
        cout << "Warning: Signature generation failed\n";
    }
}
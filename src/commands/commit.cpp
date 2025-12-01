#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

// -----------------------------
// GET CURRENT DATE-TIME
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

// -----------------------------
// GET LOCAL MACHINE IP ADDRESS
// -----------------------------
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

    // Author + IP
    string authorName = getAuthorName(); 
    string ip = getIPAddress();

    commitContent += "message: " + message + "\n";
    commitContent += "author: " + authorName + "\n";
    commitContent += "author_ip: " + ip + "\n";   // <-- NEW
    commitContent += "date: " + getCurrentTime() + "\n";
    commitContent += "files:\n";

    // Read index (files staged for commit)
    while (getline(index, line))
    {
        if (!line.empty())
        {
            commitContent += "  " + line + "\n";
            hasFiles = true;
        }
    }
    index.close();

    if (!hasFiles)
    {
        cout << "Nothing to commit.\n";
        return;
    }

    // Generate commit hash
    size_t commitHashValue = hash<string>{}(commitContent + to_string(time(0)));
    string commitHash = to_string(commitHashValue);

    // Read HEAD to know branch
    ifstream headFile(".gitlite/HEAD");
    string refPath;
    getline(headFile, refPath);
    headFile.close();

    string branchName = "main";
    size_t pos = refPath.find("refs/heads/");
    if (pos != string::npos)
        branchName = refPath.substr(pos + 11);

    string branchPath = ".gitlite/branches/" + branchName;

    // Get parent commit
    ifstream branch(branchPath);
    string parent;
    getline(branch, parent);
    branch.close();

    if (parent.empty() || parent == "null")
        parent = "null";

    // Final commit content
    commitContent = "commit: " + commitHash + "\n" +
                    "parent: " + parent + "\n" +
                    commitContent;

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
}

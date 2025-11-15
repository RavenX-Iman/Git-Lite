#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "commit.h"
using namespace std;

string getCurrentTime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

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

    commitContent += "message: " + message + "\n";
    commitContent += "author: Huma Ijaz\n";
    commitContent += "date: " + getCurrentTime() + "\n";
    commitContent += "files:\n";

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

    size_t commitHashValue = hash<string>{}(commitContent + to_string(time(0)));
    string commitHash = to_string(commitHashValue);

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

    commitContent = "commit: " + commitHash + "\n" +
                    "parent: " + parent + "\n" +
                    commitContent;

    string commitFile = ".gitlite/commits/" + commitHash + ".txt";
    ofstream commitOut(commitFile);
    commitOut << commitContent;
    commitOut.close();

    ofstream branchOut(branchPath);
    branchOut << commitHash;
    branchOut.close();

    ofstream clearIndex(indexPath, ios::trunc);
    clearIndex.close();

    cout << "Committed as " << commitHash << ": " << message << "\n";
}

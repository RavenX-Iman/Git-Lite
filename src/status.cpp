#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <unordered_set>

using namespace std;
namespace fs = filesystem;

string getFileContent(const string &path)
{
    ifstream file(path);
    return string((istreambuf_iterator<char>(file)),
                   istreambuf_iterator<char>());
}

void vcs_status()
{
    string indexPath = ".gitle/index";
    unordered_set<string> trackedFiles;

    // Read tracked files
    ifstream index(indexPath);
    string line;
    while (getline(index, line))
        trackedFiles.insert(line);

    cout << "=== Status ===\n";

    // Check working directory files
    for (const auto &file : fs::directory_iterator("src"))
    {
        if (!file.is_regular_file()) continue;

        string path = file.path().string();

        if (trackedFiles.count(path) == 0)
        {
            cout << "Untracked: " << path << "\n";
            continue;
        }

        // Check modification
        string current = getFileContent(path);
        string storedPath = ".gitle/" + path; // your system stores files here
        ifstream stored(storedPath);

        if (!stored)
        {
            cout << "Staged (new): " << path << "\n";
            continue;
        }

        string oldContent((istreambuf_iterator<char>(stored)),
                           istreambuf_iterator<char>());

        if (oldContent != current)
            cout << "Modified: " << path << "\n";
    }

    // Detect deleted files
    for (const string &tracked : trackedFiles)
    {
        if (!fs::exists(tracked))
            cout << "Deleted: " << tracked << "\n";
    }
}

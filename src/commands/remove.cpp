#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "remove.h"

using namespace std;

void vcs_remove(const string& filename) {
    string indexPath = ".gitlite/index";
    
    // Check if index file exists
    ifstream indexIn(indexPath);
    if (!indexIn) {
        cout << "Error: No files staged (index is empty).\n";
        return;
    }

    // Read all lines from index
    vector<pair<string, string>> indexEntries; // <hash, filename>
    string line;
    bool fileFound = false;

    while (getline(indexIn, line)) {
        if (line.empty()) continue;

        // Parse "hash filename" format
        size_t spacePos = line.find(' ');
        if (spacePos != string::npos) {
            string hash = line.substr(0, spacePos);
            string fname = line.substr(spacePos + 1);

            if (fname == filename) {
                fileFound = true;
                cout << "Removed " << filename << " from staging area.\n";
            } else {
                // Keep other files
                indexEntries.push_back({hash, fname});
            }
        }
    }
    indexIn.close();

    if (!fileFound) {
        cout << "Error: File '" << filename << "' is not staged.\n";
        return;
    }

    // Rewrite index without the removed file
    ofstream indexOut(indexPath, ios::trunc);
    for (const auto& entry : indexEntries) {
        indexOut << entry.first << " " << entry.second << "\n";
    }
    indexOut.close();

    // Optional: Delete the file from working directory
    // Uncomment the following lines if you want to delete the actual file
    /*
    if (remove(filename.c_str()) == 0) {
        cout << "Also deleted " << filename << " from working directory.\n";
    }
    */
}
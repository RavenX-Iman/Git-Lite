#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "commands/init.h"
#include "commands/add.h"
#include "commands/commit.h"
#include "commands/log.h"
#include "commands/status.h"
#include "commands/remove.h"
#include "utils/path_helper.hpp"

using namespace std;

// Updated getSHA256 with correct path
string getSHA256(const string& input) {
    // Get project root directory
    string projectRoot = getExecutableDir();
    string scriptPath = projectRoot + "\\sha\\sha_module.py";
    
    string command = "python \"" + scriptPath + "\" \"" + input + "\"";
    
    // Append redirect to a temp file
    command += " > tmp_hash.txt";

    system(command.c_str());

    // Read the hash from the temp file
    string hash;
    ifstream infile("tmp_hash.txt");
    if (infile.is_open()) {
        getline(infile, hash);
        infile.close();
    }

    // Delete temp file
    remove("tmp_hash.txt");

    return hash;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: vcs <command> [args]\n";
        return 1;
    }

    string command = argv[1];

    if (command == "init") {
        if (argc < 3) {
            cout << "Usage: vcs init <project-name>\n";
            return 1;
        }
        vcs_init(argv[2]);
    }

    else if (command == "add") {
        if (argc < 3) {
            cout << "Usage: vcs add <filename>\n";
            return 1;
        }
        vcs_add(argv[2]);
    }

    // Code to apply SHA-256 on the commit part
    else if (command == "commit") {
        if (argc < 4 || string(argv[2]) != "-m") {
            cout << "Usage: vcs commit -m \"message\"\n";
            return 1;
        }

        string message = argv[3];

        // Get SHA-256 of the commit message
        string commitHash = getSHA256(message);

        cout << "Commit SHA-256: " << commitHash << "\n";

        // Call your existing commit function
        vcs_commit(message);
    }

    else if (command == "log") {
        vcs_log();
    }

    else if (command == "status") {
        vcs_status();
    }

    else if (command == "rm") {
        if (argc < 3) {
            cout << "Usage: vcs rm <filename>\n";
            return 1;
        }
        vcs_remove(argv[2]);
    }

    else {
        cout << "Unknown command: " << command << "\n";
    }

    return 0;
}
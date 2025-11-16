#include <iostream>
#include <string>
#include "commands/init.h"
#include "commands/add.h"
#include "commands/commit.h"
#include "commands/log.h"
#include "commands/status.h"
using namespace std;

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

    else if (command == "commit") {
        if (argc < 4 || string(argv[2]) != "-m") {
            cout << "Usage: vcs commit -m \"message\"\n";
            return 1;
        }
        vcs_commit(argv[3]);
    }

    else if (command == "log") {
        vcs_log();
    }

    else if (command == "status") {
    vcs_status();
    }

    else {
        cout << "Unknown command: " << command << "\n";
    }

    return 0;
}

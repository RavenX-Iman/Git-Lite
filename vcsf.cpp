#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>   // for linux/mac _mkdir
#include <direct.h>     // for Windows _mkdir
#include <cstring>
#include <ctime>

using namespace std;

// ---------- Helper ----------
void makeDir(const string& path) {
#ifdef _WIN32
    _mkdir(path.c_str());   // Windows
#else
    mkdir(path.c_str(), 0777); // Linux/Mac
#endif
}

// ---------- INIT ----------
void vcs_init(const string& repoName) {
    string repoPath = repoName;
    string gitlitePath = repoPath + "/.gitlite";

    //Checks if HEAD file exists ,means repo already initialized.
    ifstream check((gitlitePath + "/HEAD").c_str());
    if (check.good()) {
        cout << "Reinitialized existing Git-Lite repository in " << gitlitePath << "\n";
        return;
    }

    // Create folders
    makeDir(repoPath);
    makeDir(gitlitePath);
    makeDir(gitlitePath + "/objects");
    makeDir(gitlitePath + "/commits");
    makeDir(gitlitePath + "/branches");

    // HEAD
    ofstream head(gitlitePath + "/HEAD");
    head << "ref: refs/heads/main\n";
    head.close();

    // main branch
    ofstream mainBranch(gitlitePath + "/branches/main");
    mainBranch << "null";
    mainBranch.close();

    cout << "Initialized empty Git-Lite repository in "
         << gitlitePath << "\n";
}

// ---------- ADD ----------
void vcs_add(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Error: File not found.\n";
        return;
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // simple hash fallback
    size_t hashValue = hash<string>{}(content);
    string hashStr = to_string(hashValue);

    // store file snapshot
    string objectPath = ".gitlite/objects/" + hashStr;
    makeDir(".gitlite");
    makeDir(".gitlite/objects");
    ofstream obj(objectPath, ios::binary);
    obj << content;
    obj.close();

    // update index
    ofstream index(".gitlite/index", ios::app);
    index << hashStr << " " << filename << "\n";
    index.close();

    cout << "Added " << filename << " to staging area.\n";
}

// ---------- COMMIT ----------
string getCurrentTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

 void vcs_commit(const string& message) {
    string indexPath = ".gitlite/index";
    ifstream index(indexPath);
    if (!index) {
        cout << "Nothing to commit (index file missing or empty).\n";
        return;
    }

    // Read staged files from index
    string line;
    string commitContent;
    bool hasFiles = false;

    commitContent += "message: " + message + "\n";
    commitContent += "author: Huma Ijaz\n";
    commitContent += "date: " + getCurrentTime() + "\n";
    commitContent += "files:\n";

    while (getline(index, line)) {
        if (!line.empty()) {
            commitContent += "  " + line + "\n";
            hasFiles = true;
        }
    }
    index.close();

    if (!hasFiles) {
        cout << "Nothing to commit.\n";
        return;
    }

    // Create commit hash (simple fallback)
    size_t commitHashValue = hash<string>{}(commitContent + to_string(time(0)));
    string commitHash = to_string(commitHashValue);

    // Get branch from HEAD
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

    commitContent = "commit: " + commitHash + "\n" +
                    "parent: " + parent + "\n" +
                    commitContent;

    // Write commit file
    string commitFile = ".gitlite/commits/" + commitHash + ".txt";
    ofstream commitOut(commitFile);
    commitOut << commitContent;
    commitOut.close();

    // Update branch reference
    ofstream branchOut(branchPath);
    branchOut << commitHash;
    branchOut.close();

    // Clear index
    ofstream clearIndex(indexPath, ios::trunc);
    clearIndex.close();

    cout << "Committed as " << commitHash << ": " << message << "\n";
}

// ---------- MAIN ----------
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
        string message = argv[3];
        vcs_commit(message);
    }

    else {
        cout << "Unknown command: " << command << "\n";
    }

    return 0;
}

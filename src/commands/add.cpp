#include <iostream>
#include <fstream>
#include <string>
#include "add.h"
#include "../utils/helpers.h"

using namespace std;

// extern void makeDir(const string&);

void vcs_add(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Error: File not found.\n";
        return;
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    size_t hashValue = hash<string>{}(content);
    string hashStr = to_string(hashValue);

    makeDir(".gitlite");
    makeDir(".gitlite/objects");

    string objectPath = ".gitlite/objects/" + hashStr;
    ofstream obj(objectPath, ios::binary);
    obj << content;
    obj.close();

    ofstream index(".gitlite/index", ios::app);
    index << hashStr << " " << filename << "\n";
    index.close();

    cout << "Added " << filename << " to staging area.\n";
}

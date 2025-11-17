# Git-Lite Architecture

## 📖 System Overview

Git-Lite is a simplified version control system that demonstrates how fundamental data structures power real-world version control operations. Built for educational purposes, it implements core Git functionality using hash tables, linked lists, and file I/O.

---

## 🎯 Design Philosophy

> *"The best way to understand version control isn't to use Git—it's to build one."*

Git-Lite bridges the gap between theoretical DSA concepts and practical software engineering by:
- Using **hash-based content addressing** for efficient storage
- Implementing **linked list commit chains** for history tracking
- Organizing data with **tree structures** for branch management
- Applying **dynamic programming** for file comparison (planned)

---

## 🏗️ Core Components

### 1. Storage Layer (Hash Tables)

**Purpose:** Content-addressable storage system

**How it works:**
```cpp
// Content → Hash → Storage
size_t hashValue = hash<string>{}(fileContent);
string objectId = to_string(hashValue);
string objectPath = ".gitlite/objects/" + objectId;
```

**Key Features:**
- **Content-addressable**: Files stored by content hash
- **Automatic deduplication**: Identical content stored once
- **Fast lookups**: O(1) average time complexity

**Example:**
```
File: "Hello World"
Hash: 1234567890
Path: .gitlite/objects/1234567890
```

---

### 2. History Layer (Linked Lists)

**Purpose:** Maintain commit history and relationships

**Structure:**
```cpp
struct Commit {
    string hash;           // Unique identifier
    string parent;         // Previous commit (linked list)
    string message;        // User description
    string author;         // Commit author
    string timestamp;      // Creation time
    vector<string> files;  // File snapshots
};
```

**Commit Chain:**
```
HEAD → Commit C → Commit B → Commit A → null
       (latest)              (initial)
```

**Key Features:**
- **Parent pointers**: Each commit links to previous
- **Chain traversal**: Walk history from HEAD to initial
- **Immutable history**: Commits never change

---

### 3. Staging Layer (Index)

**Purpose:** Temporary storage for changes before commit

**Implementation:**
```
Format: <hash> <filename>
Example: 1234567890 main.cpp
         9876543210 utils.h
```

**Workflow:**
1. User runs `vcs add file.txt`
2. File content hashed and stored in objects/
3. Mapping added to index: `hash filename`
4. On commit, index is read and cleared

---

### 4. Branch Management (Tree Structure)

**Purpose:** Support multiple development lines

**Structure:**
```
.gitlite/branches/
├── main       → points to commit hash
├── feature    → points to different commit
└── hotfix     → points to another commit
```

**HEAD pointer:**
```
.gitlite/HEAD
Content: "ref: refs/heads/main"
```

---

## 🔄 Data Flow

### Complete Workflow
```
┌─────────────────┐
│ Working Directory│
│  (Your files)   │
└────────┬────────┘
         │ vcs add
         ▼
┌─────────────────┐
│  Staging Area   │
│    (Index)      │
└────────┬────────┘
         │ vcs commit
         ▼
┌─────────────────┐
│   Repository    │
│ (Commit History)│
└─────────────────┘
```

### Detailed Operation Flow

**1. `vcs init MyProject`**
```
Create:
  MyProject/.gitlite/
  ├── HEAD (ref: refs/heads/main)
  ├── index (empty)
  ├── objects/ (empty)
  ├── commits/ (empty)
  └── branches/
      └── main (null)
```

**2. `vcs add file.txt`**
```
1. Read file.txt content
2. Generate hash: hash("content") → 1234567890
3. Store: .gitlite/objects/1234567890 ← file content
4. Update: .gitlite/index += "1234567890 file.txt"
```

**3. `vcs commit -m "message"`**
```
1. Read staged files from index
2. Get parent commit from current branch
3. Generate commit hash
4. Create commit object:
   commit: abc123
   parent: xyz789
   message: "Initial commit"
   author: Eman Tahir
   date: 2025-11-16 15:30:00
   files:
     1234567890 file.txt
5. Store: .gitlite/commits/abc123.txt
6. Update branch: main → abc123
7. Clear index
```

**4. `vcs log`**
```
1. Read HEAD → get current branch
2. Read branch → get latest commit hash
3. Read commit → display info
4. Follow parent pointer
5. Repeat until parent = null
```

**5. `vcs status`**
```
1. List all files in working directory
2. Read index to see staged files
3. Compare: working files vs staged files
4. Display:
   - Staged files (in index)
   - Untracked files (not in index)
```

---

## 📁 Repository Structure

### Internal Structure
```
MyProject/
└── .gitlite/
    ├── HEAD                      # Current branch pointer
    │                             # Content: "ref: refs/heads/main"
    │
    ├── index                     # Staging area
    │                             # Format: <hash> <filename>
    │
    ├── objects/                  # Content storage (hash-indexed)
    │   ├── 1234567890           # File snapshots
    │   ├── 9876543210           # Stored by content hash
    │   └── 5555555555
    │
    ├── commits/                  # Commit metadata
    │   ├── abc123.txt           # Commit objects
    │   ├── def456.txt           # Each contains: hash, parent,
    │   └── ghi789.txt           # message, author, date, files
    │
    └── branches/                 # Branch references
        ├── main                  # Points to latest commit hash
        └── feature              # (future implementation)
```

### File Formats

**Commit File Example:**
```
commit: 1234567890
parent: null
message: Initial commit
author: Eman Tahir
date: 2025-11-16 15:30:00
files:
  9876543210 file1.txt
  5555555555 file2.cpp
```

**Index File Example:**
```
9876543210 file1.txt
5555555555 file2.cpp
1111111111 main.py
```

---

## ⚡ Algorithm Complexity

| Operation | Time | Space | Notes |
|-----------|------|-------|-------|
| `init` | O(1) | O(1) | Directory creation |
| `add` | O(n) | O(n) | n = file size (hashing + I/O) |
| `commit` | O(m) | O(m) | m = staged files count |
| `log` | O(c) | O(1) | c = commit count (traversal) |
| `status` | O(f) | O(f) | f = files in directory |
| Hash lookup | O(1) avg | O(k) | k = object size |

### Storage Efficiency

**Deduplication Example:**
```
# Two identical files
file1.txt → hash: 123456
file2.txt → hash: 123456

# Only stored once:
.gitlite/objects/123456 (one copy)
```

**Space Usage:**
- Unique file content: Stored once
- Duplicate content: References only
- Commit metadata: ~500 bytes per commit
- Total: Sum of unique content + commits

---

## 🔧 Implementation Details

### Cross-Platform Support
```cpp
#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    // Windows-specific implementations
#else
    #include <dirent.h>
    #include <sys/stat.h>
    // POSIX-specific implementations
#endif
```

### Error Handling
```cpp
// File operations
if (!file.is_open()) {
    cerr << "Error: Cannot open file\n";
    return false;
}

// User input validation
if (message.empty()) {
    cerr << "Error: Commit message required\n";
    return false;
}
```

---

## 🚀 Future Enhancements

### Planned Features

1. **Branch Management**
   - Create/switch branches
   - Merge branches
   - Conflict resolution

2. **File Comparison (Diff)**
   - LCS algorithm implementation
   - Line-by-line comparison
   - Unified diff format

3. **Checkout**
   - Restore files from commits
   - Switch between commits
   - Detached HEAD state

4. **Optimization**
   - Compression (zlib)
   - SHA-256 hashing
   - Packfiles for efficiency

---

## 📚 Key Learnings

This architecture demonstrates:
- ✅ **Practical data structures** in action
- ✅ **Hash-based systems** for content addressing
- ✅ **Linked data** for versioning
- ✅ **File I/O** for persistence
- ✅ **Algorithm design** for real problems

---

## 🎓 Academic Context

**Course:** Data Structures  
**Institution:** COMSATS University Islamabad  
**Purpose:** Demonstrate practical DSA applications

---

## 📖 References

- [Pro Git Book](https://git-scm.com/book/en/v2)
- [Git Internals](https://git-scm.com/book/en/v2/Git-Internals-Plumbing-and-Porcelain)
- Data Structures and Algorithms in C++ - Adam Drozdek

---

**Last Updated:** November 2025  
**Maintained by:** Git-Lite Team
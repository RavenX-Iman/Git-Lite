<div align="center">

# 🔰 Git-Lite

### A Lightweight Version Control System Built with Core Data Structures

[![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white)](https://github.com/RavenX-Iman/Git-Lite)

*Demonstrating practical applications of Data Structures in real-world software development*

[Features](#-features) • [Quick Start](#-quick-start) • [Usage](#-usage) • [Architecture](#-architecture) • [Team](#-authors)

</div>

---

## 📖 Overview

**Git-Lite** is a simplified version control system that implements essential Git-like functionality from scratch. Built as a semester project for the Data Structures course at COMSATS University Islamabad, this system demonstrates how fundamental data structures power real-world version control operations.

Unlike typical academic projects, Git-Lite goes beyond theory, it's a **functional tool** that can track changes, maintain history, and manage file versions using optimized algorithms and carefully chosen data structures.

### 🎯 Project Philosophy

> *"The best way to understand version control isn't to use Git—it's to build one."*

This project bridges the gap between theoretical DSA concepts and practical software engineering by implementing:
- **Hash-based content addressing** for efficient storage
- **Linked list commit chains** for history tracking  
- **Tree structures** for branch management
- **Dynamic programming** for file comparison

---

## ✨ Features

### Currently Implemented

<table>
<tr>
<td width="50%">

#### 🚀 Repository Initialization
```bash
vcs init MyProject
```
- Creates `.gitlite` directory structure
- Initializes HEAD and main branch
- Sets up object and commit storage
- Detects and handles re-initialization

</td>
<td width="50%">

#### 📦 File Staging
```bash
vcs add filename.txt
```
- Hash-based content storage
- Efficient duplicate detection
- Automatic object management
- Index tracking for staged files

</td>
</tr>
<tr>
<td>

#### 💾 Commit Creation
```bash
vcs commit -m "Your message"
```
- Immutable snapshots with metadata
- Linked commit history (parent-child)
- Automatic timestamping
- Branch pointer updates

</td>
<td>

#### 📜 History Viewing
```bash
vcs log
```
- View complete commit history
- Shows commit hash, message, author
- Displays parent relationships
- Lists all files in each commit

</td>
</tr>
<tr>
<td colspan="2">

#### 📊 Repository Status
```bash
vcs status
```
- Shows staged files ready for commit
- Lists untracked files in working directory
- Clean, organized output
- Helpful usage hints

</td>
</tr>
</table>

---
### 🔄 Future Enhancements™
- `vcs rm` - Unstage/remove files
- `vcs checkout` - Restore files from commits
- `vcs branch` - Create/manage branches



### Data Structures Implementation

```
┌─────────────────────────────────────────────────┐
│               Git-Lite System                   │
├─────────────────────────────────────────────────┤
│                                                 │
│  Hash Tables  →  Content-addressable storage    │
│  Linked Lists →  Commit chain & history         │
│  Trees        →  Directory structure & branches │
│  File System  →  Persistent data storage        │
│                                                 │
└─────────────────────────────────────────────────┘

```

 📚 **Deep Dive:** For comprehensive architecture documentation including detailed data flow, implementation specifics, and algorithm explanations, see [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)

---

## 🚀 Quick Start

### Prerequisites
```bash
# Windows
- MinGW GCC/G++ compiler (8.1.0 or higher)
- C++17 support
- Visual Studio Code (recommended)

# Linux/Mac  
- GCC/G++ (7.0 or higher)
- C++17 support
- Any text editor
```

### Installation
```bash
# Clone the repository
git clone https://github.com/RavenX-Iman/Git-Lite.git
cd Git-Lite

# Create build directory and compile
mkdir build

# Windows
g++ vcsf.cpp -o build/vcs.exe

# Linux/Mac
g++ vcsf.cpp -o build/vcs
chmod +x build/vcs
```

### First Steps
```bash
# 1. Initialize a new repository
build/vcs init TechVault
cd TechVault

# 2. Create and add files
echo "print('Hello World!')" > app.py
../build/vcs add app.py

# 3. Check status
../build/vcs status

# 4. Commit your changes
../build/vcs commit -m "Initial commit"

# 5. View history
../build/vcs log
```
---
## 🆚 Comparison With Real Git


| Feature                  | Git-Lite | Real Git |
|--------------------------|----------|----------|
| Hash Algorithm           | C++ hash | SHA-1 / SHA-256 |
| Commit Structure         | Single parent | Supports merges (multiple parents) |
| Object Types             | file snapshots | blob, tree, commit, tag |
| Branching                | Basic pointers | refs, HEAD detaching |
| Diffing                  | (Planned) | Efficient LCS + delta compression |
| Storage Optimization     | None | Packfiles, compression |
---

---

### Command Reference

| Command | Description | Example |
|---------|-------------|---------|
| `vcs init <name>` | Create new repository | `vcs init MyRepo` |
| `vcs add <file>` | Stage a file | `vcs add main.cpp` |
| `vcs commit -m "msg"` | Commit staged files | `vcs commit -m "Fixed bug"` |
| `vcs log` | View commit history | `vcs log` |
| `vcs status` | Check repository status | `vcs status` |
---

### Storage Efficiency

- **Deduplication**: Identical files stored once (content-addressed)
- **Incremental**: Only changed files consume additional space
- **Metadata**: Minimal overhead per commit (~500 bytes)

---


### Cross-Platform Compatibility"
*(Tested on Windows, designed for cross-platform)*"
```cpp
// Windows & Linux support
#ifdef _WIN32
    #include <windows.h>
    // Windows-specific code
#else
    #include <dirent.h>
    // POSIX-specific code
#endif
```

### Learning Objectives Met

✅ Practical application of hash tables and linked lists  
✅ File I/O and serialization techniques  
✅ Algorithm design and complexity analysis  
✅ Software architecture and modular design  
✅ Version control system internals  


## 👥 Authors

<table>
<tr>
<td align="center" width="33%">
<img src="https://github.com/RavenX-Iman.png" width="100px;" alt="Eman Tahir"/><br />
<sub><b>Eman Tahir</b></sub><br />
<a href="https://github.com/RavenX-Iman">SP24-BSE-020</a><br />
<sub>Commit System & Documentation</sub>
</td>
<td align="center" width="33%">
<img src="https://github.com/Huma-Ijaz.png" width="100px;" alt="Huma Ijaz"/><br />
<sub><b>Huma Ijaz</b></sub><br /><a href="https://github.com/Huma-Ijaz">SP24-BSE-034</a><br />
<sub>File Staging & Hash Implementation</sub>
</td>
<td align="center" width="33%">
<img src="https://github.com/areebactech.png" width="100px;" alt="Areeba Asif"/><br />
<sub><b>Areeba Asif</b></sub><br />
<a href="https://github.com/areebactech">SP24-BSE-042</a><br />
<sub>Repository Initialization & Architecture</sub>
</td>
</tr>
</table>



### Expected Behavior

- ✅ Reinitializing existing repo shows appropriate message
- ✅ Adding non-existent files displays error
- ✅ Empty staging area prevents commits
- ✅ Commits properly chain together
- ✅ Identical content uses same object hash

---

## 🤝 Contributing

We welcome contributions from students, developers, and anyone interested in version control!

### Quick Contributing Guide

**For Students & Learners:**
- 🐛 **Report Bugs** - Found an issue? Open a GitHub issue
- 💡 **Suggest Features** - Ideas for improvement? Let us know
- 📚 **Improve Docs** - Help make our documentation clearer
- ✨ **Add Examples** - Share interesting use cases

**For Developers:**
```bash
# Quick start
git clone https://github.com/RavenX-Iman/Git-Lite.git
cd Git-Lite
g++ vcsf.cpp -o build/vcs
```

**Contribution Steps:**
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes and test thoroughly
4. Commit with clear messages (`git commit -m 'Add: feature description'`)
5. Push and open a Pull Request

> 📚 **Full Guidelines:** See [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md) for detailed code style, testing procedures, commit message format, and complete contribution workflow.


---

## 📚 References & Resources

### Core Learning Materials
1. **Pro Git (2nd Edition)** - Scott Chacon & Ben Straub  
   📖 [Free Online Book](https://git-scm.com/book/en/v2)

2. **Git Internals Documentation**  
   🔧 [Official Plumbing Guide](https://git-scm.com/book/en/v2/Git-Internals-Plumbing-and-Porcelain)

3. **Data Structures and Algorithms in C++** - Adam Drozdek  
   📘 Standard DSA textbook

### Inspirations
- [Git Source Code](https://github.com/git/git) - The real deal
- [Gitlet](http://gitlet.maryrosecook.com/) - JavaScript implementation
- [ugit](https://www.leshenko.net/p/ugit/) - Python implementation

---

## 📝 License

This project is created for **educational purposes** as part of the Data Structures course curriculum at COMSATS University Islamabad.

**Academic Use:** Free to use, modify, and learn from  
**Attribution:** Please credit the original authors when using this code

---

## 🐛 Known Issues & Limitations

- ⚠️ **Single-user only**: No authentication or multi-user support
- ⚠️ **No compression**: Files stored as-is (not compressed)
- ⚠️ **Basic hashing**: Uses `std::hash` (not cryptographic)
- ⚠️ **No merge**: Branch merging not implemented yet
- ⚠️ **Windows paths**: May have issues with special characters

### Future Improvements
- Add SHA-256 for content hashing
- Implement compression (zlib)
- Add network protocol for remote repos
- Create GUI interface
- Support larger binary files

---

## 💬 FAQ

**Q: Can I use this for real projects?**  
A: Git-Lite is educational. For production, use Git. But it's great for learning!

**Q: Why not just use Git?**  
A: Building your own teaches you how version control *actually* works.

**Q: What's the difference from Git?**  
A: Simplified algorithms, no network features, basic data structures.

**Q: Can I contribute?**  
A: Absolutely! See the [Contributing](#-contributing) section.


---

<div align="center">

### ⭐ Star this repo if you found it helpful!

 COMSATS University Islamabad Sahiwal Campus  
 Department of Software Engineering

**Data Structures - Fall 2025**

[⬆ Back to Top](#-git-lite)

</div>

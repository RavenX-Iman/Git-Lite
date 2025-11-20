# Contributing to Git-Lite

Thank you for your interest in contributing to Git-Lite! We welcome contributions from students, developers, and anyone interested in version control systems.

## 🎯 Ways to Contribute

- 🐛 **Report bugs** - Found an issue? Let us know
- 💡 **Suggest features** - Have an idea? Share it
- 📚 **Improve documentation** - Help make it clearer
- 🔧 **Submit code** - Fix bugs or add features
- ✨ **Share examples** - Show interesting use cases

---

## 🚀 Getting Started

### 1. Fork & Clone
```bash
# Fork the repo on GitHub, then:
git clone https://github.com/YOUR-USERNAME/Git-Lite.git
cd Git-Lite
```

### 2. Set Up Development Environment
```bash
# Create build directory
mkdir build

# Compile the project
g++ vcsf.cpp -o build/vcs

# Test it works
build/vcs init TestRepo
```

### 3. Create a Feature Branch
```bash
git checkout -b feature/your-feature-name
# or
git checkout -b fix/bug-description
```

---

## 💻 Development Workflow

### Making Changes

1. **Write your code** in the appropriate files:
   - Commands → `src/commands/`
   - Utilities → `src/utils/`
   - Main logic → `src/main.cpp`

2. **Test your changes**:
```bash
   # Compile
   g++ vcsf.cpp -o build/vcs
   
   # Manual testing
   build/vcs init TestProject
   cd TestProject
   # ... test your changes
```

3. **Commit with clear messages**:
```bash
   git add .
   git commit -m "Add: Brief description of what you added"
   # or
   git commit -m "Fix: Brief description of what you fixed"
```

4. **Push to your fork**:
```bash
   git push origin feature/your-feature-name
```

5. **Open a Pull Request** on GitHub

---

## 📋 Code Style Guidelines

### General Principles

- **Clear naming**: Use descriptive variable and function names
- **Comments**: Explain complex logic and algorithms
- **Consistency**: Follow existing code patterns
- **Simplicity**: Keep functions focused and concise

### C++ Style
```cpp
// Good: Clear variable names
string commitHash = generateHash(content);
string parentCommit = getParentFromBranch(branchName);

// Bad: Unclear names
string h = gen(c);
string p = get(b);

// Good: Commented complex logic
// Generate unique hash using content and timestamp
// This prevents collisions for commits made at same time
size_t hashValue = hash<string>{}(content + to_string(time(0)));

// Good: Consistent error handling
if (!file.is_open()) {
    cerr << "Error: Cannot open file '" << filename << "'\n";
    return false;
}
```

### Commit Message Format
```
Type: Brief description (50 chars max)

Detailed explanation if needed (72 chars per line)
- Point 1
- Point 2

Fixes #issue-number (if applicable)
```

**Types:**
- `Add:` - New feature
- `Fix:` - Bug fix
- `Update:` - Update existing feature
- `Refactor:` - Code restructuring
- `Docs:` - Documentation changes
- `Test:` - Test additions/changes

---

## 🧪 Testing Your Changes

### Manual Testing Checklist

Before submitting a PR, test these scenarios:
```bash
# 1. Fresh initialization
build/vcs init TestRepo
cd TestRepo

# 2. Add files
echo "test" > file1.txt
../build/vcs add file1.txt
../build/vcs status  # Should show staged file

# 3. Commit
../build/vcs commit -m "Test commit"
../build/vcs log     # Should show commit

# 4. Multiple commits
echo "test2" > file2.txt
../build/vcs add file2.txt
../build/vcs commit -m "Second commit"
../build/vcs log     # Should show both commits

# 5. Error handling
../build/vcs add nonexistent.txt  # Should show error
../build/vcs commit -m "Empty"    # Should prevent commit if nothing staged
```

### What to Test

- ✅ Your new feature works as expected
- ✅ Existing features still work (no regression)
- ✅ Error cases are handled gracefully
- ✅ Edge cases are considered

---

## 🎓 For Students

This is a great opportunity to:
- Practice collaborative development
- Learn Git workflows
- Understand version control internals
- Build your portfolio

Don't worry about making mistakes - that's how we learn!

---

## 📞 Questions or Issues?

- **Found a bug?** Open an [issue](https://github.com/RavenX-Iman/Git-Lite/issues)
- **Need help?** Comment on an issue or PR
- **Want to discuss?** Start a [discussion](https://github.com/RavenX-Iman/Git-Lite/discussions)

---

## 👥 Core Team

- **Eman Tahir** ([@RavenX-Iman](https://github.com/RavenX-Iman)) - Commit System
- **Huma Ijaz** ([@Huma-Ijaz](https://github.com/Huma-Ijaz)) - File Staging
- **Areeba Asif** ([@areebactech](https://github.com/areebactech)) - Repository Init

---

## 📜 Code of Conduct

- Be respectful and constructive
- Help others learn and grow
- Give credit where it's due
- Focus on the code, not the person

---

**Thank you for contributing to Git-Lite!** 🚀
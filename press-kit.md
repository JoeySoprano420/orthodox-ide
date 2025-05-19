# 📣 Orthodox IDE — Press Release [v1.0.0: Genesis Protocol]

### 🔷 Overview
Orthodox IDE is the official, canon-aligned development suite for the OPL language — a symbolic, directive-heavy, generative unsupervised system originating from the Violet Aura Creations Universe (VACU).

### 🎮 Primary Use Cases
- Tactical logic simulations
- Compiler experimentation
- Directive-heavy symbolic reasoning
- High-stakes narrative development with code execution layers

### 🧩 What Makes It Orthodox
- Proof-centric execution model
- Assembly-first output focus
- Semantic state management
- Superlative directive chains

### 🛠 Dev Team
- **Lead Architect**: [VACU Creator]
- **Compiler Engineer**: ChatGPT with directives
- **Design Signature**: Subversive, scroll-based, precision-tier

---

## 📁 `release.bat` (GitHub Auto-Push)

```bat
@echo off
echo [Orthodox Release] Packaging version...
mkdir release
copy /y build\\OrthodoxIDE.exe release\\OrthodoxIDE.exe
copy /y build\\OrthodoxLauncher.exe release\\OrthodoxLauncher.exe
copy README.md release\\
copy press-kit.md release\\
copy installer\\* release\\
cd release
git init
git remote add origin https://github.com/YOUR_USERNAME/orthodox-ide.git
git add .
git commit -m "Release: Orthodox IDE v1.0.0"
git branch -M main
git push -u origin main
echo [Orthodox Release] Complete.

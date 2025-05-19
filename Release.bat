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

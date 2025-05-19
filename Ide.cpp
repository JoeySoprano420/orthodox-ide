Lightening Strategy

from pathlib import Path
import zipfile

# Define the directory structure and file contents for the OPLC project
project_structure = {
    "oplc/oplc.cpp": """
#include "opl_parser.h"
#include "opl_emitter.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: oplc <input.opl>" << std::endl;
        return 1;
    }

    std::string inputPath = argv[1];
    try {
        auto ast = OPLParser::Parse(inputPath);
        OPLEmitter emitter;
        emitter.EmitASM(ast, "build/output.asm");
        std::cout << "Assembly written to build/output.asm" << std::endl;
    } catch (std::exception& ex) {
        std::cerr << "Compilation error: " << ex.what() << std::endl;
        return 2;
    }

    return 0;
}
""",
    "oplc/opl_parser.h": """
#pragma once
#include <string>
#include <vector>
#include <memory>

struct ASTNode {
    std::string type;
    std::string value;
    std::vector<std::shared_ptr<ASTNode>> children;
};

class OPLParser {
public:
    static std::shared_ptr<ASTNode> Parse(const std::string& filepath);
};
""",
    "oplc/opl_parser.cpp": """
#include "opl_parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::shared_ptr<ASTNode> OPLParser::Parse(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) throw std::runtime_error("Cannot open file");

    auto root = std::make_shared<ASTNode>();
    root->type = "Program";

    std::string line;
    while (std::getline(file, line)) {
        auto node = std::make_shared<ASTNode>();
        node->type = "Line";
        node->value = line;
        root->children.push_back(node);
    }
    return root;
}
""",
    "oplc/opl_emitter.h": """
#pragma once
#include "opl_parser.h"
#include <string>

class OPLEmitter {
public:
    void EmitASM(std::shared_ptr<ASTNode> root, const std::string& outputPath);
};
""",
    "oplc/opl_emitter.cpp": """
#include "opl_emitter.h"
#include <fstream>

void OPLEmitter::EmitASM(std::shared_ptr<ASTNode> root, const std::string& outputPath) {
    std::ofstream out(outputPath);
    for (const auto& child : root->children) {
        out << "; " << child->value << "\\n";
        out << "nop\\n";
    }
}
""",
    "oplc/stdlib/base.opl": """
@truth(ONE) = 1;
@truth(ZERO) = 0;

!resolver double(|x|) = |x| * 2;
!resolver confirm(|x|) = (|x| >= 0x10);

::macro[payload_fire] {
    mov rax, 0x1337;
    int 0x80;
}
""",
    "oplc/examples/sample.opl": """
@truth(axiom) = 0x1F;
@state(current) := derive(@axiom);
@modifier(strict) ~ enforce(@state);

{
    !declare |α| = 12;
    !resolve |β| = double(|α|);
    !proof @result := confirm(|β| > 20);
    !commit @result;
}

⛓commit_path {
    if (@result == true) {
        ::emit_hex(0xC4);
        ::invoke[routine: payload_fire];
    }
}
""",
    "oplc/opl.tmLanguage.json": """
{
  "name": "Orthodox Programming Language",
  "fileTypes": ["opl"],
  "scopeName": "source.opl",
  "patterns": [
    { "match": "@[a-zA-Z_][a-zA-Z0-9_]*", "name": "keyword.control.opl" },
    { "match": "!\\\\w+", "name": "keyword.directive.opl" },
    { "match": "::\\\\w+", "name": "support.function.directive.opl" },
    { "match": "\\\\|[^\\\\|]+\\\\|", "name": "variable.parameter.opl" },
    { "match": "~", "name": "keyword.operator.bind.opl" },
    { "match": ":=", "name": "keyword.operator.assign.opl" },
    { "match": "==", "name": "keyword.operator.comparison.opl" }
  ]
}
"""
}

# Create a zip file with all the project files
zip_path = "/mnt/data/OPLC_Compiler_Release.zip"
with zipfile.ZipFile(zip_path, 'w') as zipf:
    for filepath, content in project_structure.items():
        file_path_obj = Path(filepath)
        zipf.writestr(str(file_path_obj), content)

zip_path

# Extend the ZIP package to include the .bat and .sh build scripts and a preconfigured .vscode folder
additional_files = {
    "oplc/build.bat": """
@echo off
echo [OPLC] Compiling...
g++ -std=c++17 -o build\\oplc.exe oplc.cpp opl_parser.cpp opl_emitter.cpp
echo [OPLC] Compilation complete.
""",
    "oplc/build.sh": """
#!/bin/bash
echo "[OPLC] Compiling..."
g++ -std=c++17 -o build/oplc oplc.cpp opl_parser.cpp opl_emitter.cpp
echo "[OPLC] Compilation complete."
""",
    "oplc/.vscode/launch.json": """
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Run OPLC",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/build/oplc",
      "args": ["examples/sample.opl"],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "setupCommands": [
        { "description": "Enable pretty-printing", "text": "-enable-pretty-printing", "ignoreFailures": true }
      ]
    }
  ]
}
""",
    "oplc/.vscode/tasks.json": """
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Build OPLC",
      "type": "shell",
      "command": "./build.sh",
      "group": {
        "kind": "build",
        "isDefault": true
      }
    }
  ]
}
"""
}

# Add the new files to the ZIP
with zipfile.ZipFile("/mnt/data/OPLC_Compiler_Release.zip", 'a') as zipf:
    for filepath, content in additional_files.items():
        zipf.writestr(filepath, content)

# Create NSIS and InnoSetup installer script templates for packaging the Orthodox IDE
installer_scripts = {
    "oplc/installer/oplc_installer.nsi": """
; NSIS Installer Script for OPLC Compiler
Name "Orthodox Programming Language Compiler"
OutFile "OPLC_Installer.exe"
InstallDir $PROGRAMFILES\\OPLC
RequestExecutionLevel admin

Page directory
Page instfiles

Section ""
  SetOutPath $INSTDIR
  File /r oplc\\*
  CreateShortcut "$DESKTOP\\OPLC Compiler.lnk" "$INSTDIR\\build\\oplc.exe"
SectionEnd
""",
    "oplc/installer/oplc_innosetup.iss": """
; Inno Setup Installer Script for OPLC Compiler
[Setup]
AppName=Orthodox Programming Language Compiler
AppVersion=1.0
DefaultDirName={pf}\\OPLC
DefaultGroupName=OPLC
OutputBaseFilename=OPLC_InnoSetupInstaller
Compression=lzma
SolidCompression=yes

[Files]
Source: "oplc\\*"; DestDir: "{app}"; Flags: recursesubdirs

[Icons]
Name: "{commondesktop}\\OPLC Compiler"; Filename: "{app}\\build\\oplc.exe"
"""
}

# Add the installer scripts to the ZIP
with zipfile.ZipFile("/mnt/data/OPLC_Compiler_Release.zip", 'a') as zipf:
    for filepath, content in installer_scripts.items():
        zipf.writestr(filepath, content)

# Define the Orthodox IDE Qt project structure and essential source files
qt_ide_files = {
    "OrthodoxIDE/main.cpp": """
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
""",
    "OrthodoxIDE/mainwindow.h": """
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTreeView>
#include <QSplitter>
#include <QPlainTextEdit>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private:
    QTreeView *projectTree;
    QTextEdit *editor;
    QPlainTextEdit *proofLog;
    QPlainTextEdit *terminal;
};

#endif // MAINWINDOW_H
""",
    "OrthodoxIDE/mainwindow.cpp": """
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QFileSystemModel>
#include <QDir>
#include <QTextEdit>
#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
    QSplitter *rightSplitter = new QSplitter(Qt::Vertical, mainSplitter);

    projectTree = new QTreeView(mainSplitter);
    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::currentPath());
    projectTree->setModel(model);
    projectTree->setRootIndex(model->index(QDir::currentPath()));

    editor = new QTextEdit(rightSplitter);
    editor->setPlainText("// Orthodox .opl file editor");

    proofLog = new QPlainTextEdit(rightSplitter);
    proofLog->setReadOnly(true);
    proofLog->setPlaceholderText("Proof Log Visualizer");

    terminal = new QPlainTextEdit(rightSplitter);
    terminal->setReadOnly(true);
    terminal->setPlaceholderText("Directive Terminal Echo");

    rightSplitter->addWidget(editor);
    rightSplitter->addWidget(proofLog);
    rightSplitter->addWidget(terminal);
    mainSplitter->addWidget(projectTree);
    mainSplitter->addWidget(rightSplitter);

    setCentralWidget(mainSplitter);
    setWindowTitle("Orthodox IDE");
    resize(1000, 700);
}
""",
    "OrthodoxIDE/OrthodoxIDE.pro": """
QT += core gui widgets

CONFIG += c++17
SOURCES += main.cpp \\
           mainwindow.cpp

HEADERS += mainwindow.h

FORMS +=
"""
}

# Create a zip file with the Orthodox IDE Qt project files
zip_path_qt = "/mnt/data/OrthodoxIDE_Qt_Starter.zip"
with zipfile.ZipFile(zip_path_qt, 'w') as zipf:
    for filepath, content in qt_ide_files.items():
        zipf.writestr(filepath, content)

zip_path_qt

# Add extended IDE functionality: syntax highlighting, runtime output hooks, and Build & Run menu integration.
extended_files = {
    "OrthodoxIDE/editorwidget.h": """
#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QTextEdit>
#include <QSyntaxHighlighter>

class OPLHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    OPLHighlighter(QTextDocument *parent = nullptr);
protected:
    void highlightBlock(const QString &text) override;
};

class EditorWidget : public QTextEdit {
    Q_OBJECT
public:
    EditorWidget(QWidget *parent = nullptr);
private:
    OPLHighlighter *highlighter;
};

#endif // EDITORWIDGET_H
""",
    "OrthodoxIDE/editorwidget.cpp": """
#include "editorwidget.h"
#include <QTextCharFormat>
#include <QRegularExpression>

OPLHighlighter::OPLHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {}

void OPLHighlighter::highlightBlock(const QString &text) {
    QTextCharFormat format;
    format.setForeground(Qt::darkMagenta);

    QRegularExpression keywords("@\\w+|!\\w+|::\\w+|:=|==|~|\\|.*?\\|");
    QRegularExpressionMatchIterator i = keywords.globalMatch(text);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        setFormat(match.capturedStart(), match.capturedLength(), format);
    }
}

EditorWidget::EditorWidget(QWidget *parent)
    : QTextEdit(parent) {
    highlighter = new OPLHighlighter(document());
    setPlainText("// Orthodox .opl code here");
}
""",
    "OrthodoxIDE/mainwindow.h": """
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTreeView>
#include <QSplitter>
#include <QPlainTextEdit>
#include <QProcess>
#include "editorwidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    void buildAndRun();

private:
    QTreeView *projectTree;
    EditorWidget *editor;
    QPlainTextEdit *proofLog;
    QPlainTextEdit *terminal;
    QProcess *compilerProcess;
    void setupMenu();
};

#endif // MAINWINDOW_H
""",
    "OrthodoxIDE/mainwindow.cpp": """
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QFileSystemModel>
#include <QDir>
#include <QMenuBar>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), compilerProcess(new QProcess(this))
{
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
    QSplitter *rightSplitter = new QSplitter(Qt::Vertical, mainSplitter);

    projectTree = new QTreeView(mainSplitter);
    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::currentPath());
    projectTree->setModel(model);
    projectTree->setRootIndex(model->index(QDir::currentPath()));

    editor = new EditorWidget(rightSplitter);
    proofLog = new QPlainTextEdit(rightSplitter);
    proofLog->setReadOnly(true);
    proofLog->setPlaceholderText("Proof Log Visualizer");

    terminal = new QPlainTextEdit(rightSplitter);
    terminal->setReadOnly(true);
    terminal->setPlaceholderText("Directive Terminal Echo");

    rightSplitter->addWidget(editor);
    rightSplitter->addWidget(proofLog);
    rightSplitter->addWidget(terminal);
    mainSplitter->addWidget(projectTree);
    mainSplitter->addWidget(rightSplitter);

    setCentralWidget(mainSplitter);
    setWindowTitle("Orthodox IDE");
    resize(1000, 700);
    setupMenu();

    connect(compilerProcess, &QProcess::readyReadStandardOutput, [=](){
        terminal->appendPlainText(compilerProcess->readAllStandardOutput());
    });
    connect(compilerProcess, &QProcess::readyReadStandardError, [=](){
        proofLog->appendPlainText(compilerProcess->readAllStandardError());
    });
}

void MainWindow::setupMenu() {
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QAction *runAction = new QAction("Build & Run", this);
    connect(runAction, &QAction::triggered, this, &MainWindow::buildAndRun);
    fileMenu->addAction(runAction);
}

void MainWindow::buildAndRun() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select .opl File", ".", "*.opl");
    if (filePath.isEmpty()) return;

    QString cmd = "cmd.exe /C build.bat";  // Or "bash build.sh" on Linux
    compilerProcess->start(cmd);
}
"""
}

# Add to Qt IDE ZIP
with zipfile.ZipFile("/mnt/data/OrthodoxIDE_Qt_Starter.zip", 'a') as zipf:
    for path, content in extended_files.items():
        zipf.writestr(path, content)

# Add autocomplete engine, stdlib viewer widget, memory visualizer, and launcher setup to Orthodox IDE
enhancements = {
    "OrthodoxIDE/autocompleteengine.h": """
#ifndef AUTOCOMPLETEENGINE_H
#define AUTOCOMPLETEENGINE_H

#include <QStringList>
#include <QCompleter>
#include <QTextEdit>
#include <QAbstractItemView>

class AutoCompleteEngine {
public:
    static QCompleter* createCompleter(QObject *parent = nullptr) {
        QStringList keywords = {
            "@truth", "@state", "@modifier", "!resolver", "!proof", "!commit",
            "::macro", "::emit_hex", "::invoke", "if", ":=", "==", "~"
        };
        QCompleter *completer = new QCompleter(keywords, parent);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setFilterMode(Qt::MatchContains);
        completer->setCompletionMode(QCompleter::PopupCompletion);
        return completer;
    }
};

#endif // AUTOCOMPLETEENGINE_H
""",
    "OrthodoxIDE/stdlibviewer.h": """
#ifndef STDLIBVIEWER_H
#define STDLIBVIEWER_H

#include <QWidget>
#include <QTextBrowser>

class StdLibViewer : public QTextBrowser {
    Q_OBJECT
public:
    explicit StdLibViewer(QWidget *parent = nullptr);
    void loadStandardLibrary(const QString &filePath);
};

#endif // STDLIBVIEWER_H
""",
    "OrthodoxIDE/stdlibviewer.cpp": """
#include "stdlibviewer.h"
#include <QFile>
#include <QTextStream>

StdLibViewer::StdLibViewer(QWidget *parent)
    : QTextBrowser(parent) {
    setReadOnly(true);
    setOpenLinks(false);
    setOpenExternalLinks(false);
}

void StdLibViewer::loadStandardLibrary(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        setPlainText(in.readAll());
    } else {
        setPlainText("Unable to load stdlib.");
    }
}
""",
    "OrthodoxIDE/memoryvisualizer.h": """
#ifndef MEMORYVISUALIZER_H
#define MEMORYVISUALIZER_H

#include <QTextBrowser>
#include <QString>

class MemoryVisualizer : public QTextBrowser {
    Q_OBJECT
public:
    explicit MemoryVisualizer(QWidget *parent = nullptr);
    void loadFromAsm(const QString &asmFile);
};

#endif // MEMORYVISUALIZER_H
""",
    "OrthodoxIDE/memoryvisualizer.cpp": """
#include "memoryvisualizer.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

MemoryVisualizer::MemoryVisualizer(QWidget *parent)
    : QTextBrowser(parent) {
    setReadOnly(true);
}

void MemoryVisualizer::loadFromAsm(const QString &asmFile) {
    QFile file(asmFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setPlainText("Memory layout file not found.");
        return;
    }

    QTextStream in(&file);
    QString content;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.contains("mov") || line.contains("db") || line.contains("dw")) {
            content += line + "\\n";
        }
    }
    setPlainText(content);
}
""",
    "OrthodoxLauncher/OrthodoxLauncher.cpp": """
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QProcess>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("Orthodox Launcher");

    QPushButton *ideButton = new QPushButton("Open IDE");
    QPushButton *runButton = new QPushButton("Run Proof");
    QPushButton *codexButton = new QPushButton("View Orthodox Codex");
    QPushButton *exitButton = new QPushButton("Exit Realm");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(ideButton);
    layout->addWidget(runButton);
    layout->addWidget(codexButton);
    layout->addWidget(exitButton);
    window.setLayout(layout);

    QObject::connect(ideButton, &QPushButton::clicked, [](){
        QProcess::startDetached("OrthodoxIDE.exe");
    });

    QObject::connect(exitButton, &QPushButton::clicked, &app, &QApplication::quit);

    window.resize(300, 200);
    window.show();
    return app.exec();
}
""",
    "OrthodoxLauncher/OrthodoxLauncher.pro": """
QT += widgets
CONFIG += c++17
SOURCES += OrthodoxLauncher.cpp
"""
}

# Add all enhancements to the zip
with zipfile.ZipFile("/mnt/data/OrthodoxIDE_Qt_Starter.zip", 'a') as zipf:
    for filepath, content in enhancements.items():
        zipf.writestr(filepath, content)

# Add Orthodox IDE Phase IV implementations: tabs, file watcher, memory sync, codex viewer, and launcher update
phase_iv_updates = {
    "OrthodoxIDE/mainwindow.h": """
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QTabWidget>
#include <QPlainTextEdit>
#include <QFileSystemWatcher>
#include <QProcess>
#include "editorwidget.h"
#include "stdlibviewer.h"
#include "memoryvisualizer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    void buildAndRun();
    void updateMemoryLayout();
    void fileSaved();

private:
    QTreeView *projectTree;
    QTabWidget *tabWidget;
    EditorWidget *editor;
    StdLibViewer *stdlibViewer;
    MemoryVisualizer *memoryVisualizer;
    QPlainTextEdit *terminal;
    QPlainTextEdit *proofLog;
    QFileSystemWatcher *fileWatcher;
    QProcess *compilerProcess;
    void setupMenu();
};

#endif // MAINWINDOW_H
""",
    "OrthodoxIDE/mainwindow.cpp": """
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), compilerProcess(new QProcess(this)), fileWatcher(new QFileSystemWatcher(this))
{
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
    QSplitter *rightSplitter = new QSplitter(Qt::Vertical, mainSplitter);

    projectTree = new QTreeView(mainSplitter);
    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::currentPath());
    projectTree->setModel(model);
    projectTree->setRootIndex(model->index(QDir::currentPath()));

    tabWidget = new QTabWidget(rightSplitter);
    editor = new EditorWidget(rightSplitter);
    stdlibViewer = new StdLibViewer(rightSplitter);
    memoryVisualizer = new MemoryVisualizer(rightSplitter);

    stdlibViewer->loadStandardLibrary("stdlib/base.opl");

    tabWidget->addTab(editor, "Editor");
    tabWidget->addTab(stdlibViewer, "Standard Library");
    tabWidget->addTab(memoryVisualizer, "Memory Layout");

    proofLog = new QPlainTextEdit(rightSplitter);
    proofLog->setReadOnly(true);
    terminal = new QPlainTextEdit(rightSplitter);
    terminal->setReadOnly(true);

    rightSplitter->addWidget(tabWidget);
    rightSplitter->addWidget(proofLog);
    rightSplitter->addWidget(terminal);
    mainSplitter->addWidget(projectTree);
    mainSplitter->addWidget(rightSplitter);

    setCentralWidget(mainSplitter);
    setWindowTitle("Orthodox IDE");
    resize(1100, 750);
    setupMenu();

    connect(compilerProcess, &QProcess::readyReadStandardOutput, [=](){
        terminal->appendPlainText(compilerProcess->readAllStandardOutput());
    });
    connect(compilerProcess, &QProcess::readyReadStandardError, [=](){
        proofLog->appendPlainText(compilerProcess->readAllStandardError());
    });

    connect(editor->document(), &QTextDocument::contentsChanged, this, &MainWindow::fileSaved);
    fileWatcher->addPath("build/output.asm");
    connect(fileWatcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::updateMemoryLayout);
}

void MainWindow::setupMenu() {
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QAction *runAction = new QAction("Build & Run", this);
    connect(runAction, &QAction::triggered, this, &MainWindow::buildAndRun);
    fileMenu->addAction(runAction);
}

void MainWindow::buildAndRun() {
    QString cmd = "cmd.exe /C build.bat";
    compilerProcess->start(cmd);
}

void MainWindow::updateMemoryLayout() {
    memoryVisualizer->loadFromAsm("build/output.asm");
    fileWatcher->addPath("build/output.asm"); // re-add if removed
}

void MainWindow::fileSaved() {
    static QTimer *compileTimer = nullptr;
    if (!compileTimer) {
        compileTimer = new QTimer(this);
        compileTimer->setSingleShot(true);
        connect(compileTimer, &QTimer::timeout, this, &MainWindow::buildAndRun);
    }
    compileTimer->start(1200); // delay compile to avoid spam
}
""",
    "OrthodoxLauncher/OrthodoxLauncher.cpp": """
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QProcess>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>

class CodexViewer : public QTextEdit {
public:
    CodexViewer(QWidget *parent = nullptr) : QTextEdit(parent) {
        setReadOnly(true);
        QFile file("OrthodoxCodex.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            setPlainText(in.readAll());
        } else {
            setPlainText("Codex not found.");
        }
        setWindowTitle("Orthodox Codex");
        resize(600, 400);
        show();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("Orthodox Launcher");

    QPushButton *ideButton = new QPushButton("Open IDE");
    QPushButton *runButton = new QPushButton("Run Proof");
    QPushButton *codexButton = new QPushButton("View Orthodox Codex");
    QPushButton *exitButton = new QPushButton("Exit Realm");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(ideButton);
    layout->addWidget(runButton);
    layout->addWidget(codexButton);
    layout->addWidget(exitButton);
    window.setLayout(layout);

    QObject::connect(ideButton, &QPushButton::clicked, [](){
        QProcess::startDetached("OrthodoxIDE.exe");
    });
    QObject::connect(runButton, &QPushButton::clicked, [](){
        QProcess::startDetached("cmd.exe", QStringList() << "/C" << "build.bat");
    });
    QObject::connect(codexButton, &QPushButton::clicked, [](){
        new CodexViewer;
    });
    QObject::connect(exitButton, &QPushButton::clicked, &app, &QApplication::quit);

    window.resize(300, 200);
    window.show();
    return app.exec();
}
"""
}

# Add all new updated files to the ZIP
with zipfile.ZipFile("/mnt/data/OrthodoxIDE_Qt_Starter.zip", 'a') as zipf:
    for path, content in phase_iv_updates.items():
        zipf.writestr(path, content)


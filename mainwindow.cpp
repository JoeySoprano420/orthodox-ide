
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

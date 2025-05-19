
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

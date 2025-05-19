
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

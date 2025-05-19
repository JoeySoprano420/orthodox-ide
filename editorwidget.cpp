
#include "editorwidget.h"
#include <QTextCharFormat>
#include <QRegularExpression>

OPLHighlighter::OPLHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {}

void OPLHighlighter::highlightBlock(const QString &text) {
    QTextCharFormat format;
    format.setForeground(Qt::darkMagenta);

    QRegularExpression keywords("@\w+|!\w+|::\w+|:=|==|~|\|.*?\|");
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

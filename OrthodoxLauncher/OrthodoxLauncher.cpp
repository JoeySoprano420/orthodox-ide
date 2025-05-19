
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QProcess>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPixmap>

class CodexViewer : public QWidget {
public:
    CodexViewer(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("📜 Orthodox Codex");
        resize(700, 500);

        QVBoxLayout *layout = new QVBoxLayout(this);
        QTextEdit *view = new QTextEdit(this);
        view->setReadOnly(true);
        QFile file("OrthodoxCodex.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            view->setPlainText(in.readAll());
        } else {
            view->setPlainText("⚠️ Codex file not found.");
        }

        layout->addWidget(view);
        setLayout(layout);
        show();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("🜏 Orthodox Launcher – VACU Tactical Suite");
    window.setStyleSheet("QWidget { background-color: #1a1a1a; color: white; font-family: Consolas; }");

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QLabel *title = new QLabel("🜏 Orthodox Launcher");
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #aaffff;");
    title->setAlignment(Qt::AlignCenter);

    QPushButton *ideBtn = new QPushButton("🛠 Open IDE");
    QPushButton *proofBtn = new QPushButton("⏳ Run Proof");
    QPushButton *codexBtn = new QPushButton("📖 View Codex");
    QPushButton *exitBtn = new QPushButton("🚪 Exit");

    QList<QPushButton*> buttons = { ideBtn, proofBtn, codexBtn, exitBtn };
    for (QPushButton* btn : buttons) {
        btn->setMinimumHeight(40);
        btn->setStyleSheet("QPushButton { font-size: 16px; background-color: #444; border: 1px solid #888; }"
                           "QPushButton:hover { background-color: #555; }");
    }

    QObject::connect(ideBtn, &QPushButton::clicked, []() {
        QProcess::startDetached("OrthodoxIDE.exe");
    });

    QObject::connect(proofBtn, &QPushButton::clicked, []() {
        QProcess::startDetached("cmd.exe", QStringList() << "/C" << "build.bat");
    });

    QObject::connect(codexBtn, &QPushButton::clicked, []() {
        new CodexViewer;
    });

    QObject::connect(exitBtn, &QPushButton::clicked, &app, &QApplication::quit);

    mainLayout->addWidget(title);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(ideBtn);
    mainLayout->addWidget(proofBtn);
    mainLayout->addWidget(codexBtn);
    mainLayout->addWidget(exitBtn);

    window.setLayout(mainLayout);
    window.resize(400, 300);
    window.show();

    return app.exec();
}

#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <cstdlib>

class TaskManagerAndRegistryControl : public QWidget {
    Q_OBJECT

public:
    TaskManagerAndRegistryControl(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        QPushButton *blockTaskManagerButton = new QPushButton("Block Task Manager", this);
        QPushButton *unblockTaskManagerButton = new QPushButton("Unblock Task Manager", this);
        QPushButton *blockRegistryEditorButton = new QPushButton("Block Registry Editor", this);
        QPushButton *unblockRegistryEditorButton = new QPushButton("Unblock Registry Editor", this);
        QPushButton *blockAllButton = new QPushButton("Block Task Manager and Registry Editor", this);
        QPushButton *unblockAllButton = new QPushButton("Unblock Task Manager and Registry Editor", this);

        layout->addWidget(blockTaskManagerButton);
        layout->addWidget(unblockTaskManagerButton);
        layout->addWidget(blockRegistryEditorButton);
        layout->addWidget(unblockRegistryEditorButton);
        layout->addWidget(blockAllButton);
        layout->addWidget(unblockAllButton);

        connect(blockTaskManagerButton, &QPushButton::clicked, this, &TaskManagerAndRegistryControl::BlockTaskManager);
        connect(unblockTaskManagerButton, &QPushButton::clicked, this, &TaskManagerAndRegistryControl::UnblockTaskManager);
        connect(blockRegistryEditorButton, &QPushButton::clicked, this, &TaskManagerAndRegistryControl::BlockRegistryEditor);
        connect(unblockRegistryEditorButton, &QPushButton::clicked, this, &TaskManagerAndRegistryControl::UnblockRegistryEditor);
        connect(blockAllButton, &QPushButton::clicked, this, &TaskManagerAndRegistryControl::BlockTaskManagerAndRegistry);
        connect(unblockAllButton, &QPushButton::clicked, this, &TaskManagerAndRegistryControl::UnblockTaskManagerAndRegistry);
    }

private slots:
    void BlockTaskManager() {
        system("reg add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\" /v DisableTaskMgr /t REG_DWORD /d 1 /f");
        QMessageBox::information(this, "Notification", "Task Manager is now blocked.");
    }

    void UnblockTaskManager() {
        system("PowerShell -Command \"Start-Process PowerShell -Verb RunAs -ArgumentList '-NoProfile -ExecutionPolicy Bypass -Command \"Remove-ItemProperty -Path \\\"HKCU:\\\\Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Policies\\\\System\\\" -Name \\\"DisableTaskMgr\\\" -ErrorAction SilentlyContinue\"'\"");
        QMessageBox::information(this, "Notification", "Unblock command for Task Manager executed. Please check permissions.");
    }

    void BlockRegistryEditor() {
        system("reg add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\" /v DisableRegistryTools /t REG_DWORD /d 1 /f");
        QMessageBox::information(this, "Notification", "Registry Editor is now blocked.");
    }

    void UnblockRegistryEditor() {
        system("PowerShell -Command \"Start-Process PowerShell -Verb RunAs -ArgumentList '-NoProfile -ExecutionPolicy Bypass -Command \"Remove-ItemProperty -Path \\\"HKCU:\\\\Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Policies\\\\System\\\" -Name \\\"DisableRegistryTools\\\" -ErrorAction SilentlyContinue\"'\"");
        QMessageBox::information(this, "Notification", "Unblock command for Registry Editor executed. Please check permissions.");
    }

    void BlockTaskManagerAndRegistry() {
        BlockTaskManager();
        BlockRegistryEditor();
        QMessageBox::information(this, "Notification", "Task Manager and Registry Editor are now blocked.");
    }

    void UnblockTaskManagerAndRegistry() {
        UnblockTaskManager();
        UnblockRegistryEditor();
        QMessageBox::information(this, "Notification", "Task Manager and Registry Editor are now unblocked.");
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TaskManagerAndRegistryControl window;
    window.setWindowTitle("Task Manager and Registry Control");
    window.resize(300, 200);
    window.show();

    return app.exec();
}
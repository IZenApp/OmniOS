#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>
#include <windows.h>
#include <vector>
#include <string>

void LockUnlockDisk(char driveLetter, bool lock) {
    std::string drivePath = "\\\\.\\";
    drivePath += driveLetter;
    drivePath += ":";

    HANDLE hDrive = CreateFile(drivePath.c_str(),
                               GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL, OPEN_EXISTING, 0, NULL);

    if (hDrive == INVALID_HANDLE_VALUE) {
        QMessageBox::warning(nullptr, "Error", "Failed to open disk");
        return;
    }

    DWORD bytesReturned;
    if (lock) {
        if (DeviceIoControl(hDrive, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, NULL)) {
            QMessageBox::information(nullptr, "Success", QString("Drive %1 locked successfully").arg(driveLetter));
        } else {
            QMessageBox::warning(nullptr, "Error", QString("Cannot lock disk %1").arg(driveLetter));
        }
    } else {
        if (DeviceIoControl(hDrive, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, NULL)) {
            QMessageBox::information(nullptr, "Success", QString("Drive %1 unlocked successfully").arg(driveLetter));
        } else {
            QMessageBox::warning(nullptr, "Error", QString("Failed to unlock disk %1").arg(driveLetter));
        }
    }

    CloseHandle(hDrive);
}

void DisplayDisks(QComboBox* comboBox) {
    DWORD drives = GetLogicalDrives();
    for (int i = 0; i < 26; ++i) {
        if (drives & (1 << i)) {
            comboBox->addItem(QString(QChar('A' + i)));
        }
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Disk Locker");

    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *label = new QLabel("Select a drive:");
    layout->addWidget(label);

    QComboBox *comboBox = new QComboBox();
    DisplayDisks(comboBox);
    layout->addWidget(comboBox);

    QPushButton *lockButton = new QPushButton("Lock Disk");
    layout->addWidget(lockButton);

    QPushButton *unlockButton = new QPushButton("Unlock Disk");
    layout->addWidget(unlockButton);

    QObject::connect(lockButton, &QPushButton::clicked, [comboBox]() {
        char driveLetter = comboBox->currentText().toStdString()[0];
        LockUnlockDisk(driveLetter, true);
    });

    QObject::connect(unlockButton, &QPushButton::clicked, [comboBox]() {
        char driveLetter = comboBox->currentText().toStdString()[0];
        LockUnlockDisk(driveLetter, false);
    });

    window.setLayout(layout);
    window.show();

    return app.exec();
}
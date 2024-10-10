#include <windows.h>
#include <iostream>
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
        std::cerr << "Error: Unable to open drive " << driveLetter << std::endl;
        return;
    }

    DWORD bytesReturned;
    if (lock) {
        if (DeviceIoControl(hDrive, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, NULL)) {
            std::cout << "Drive " << driveLetter << " locked successfully." << std::endl;
        } else {
            std::cerr << "Error: Unable to lock drive " << driveLetter << std::endl;
        }
    } else {
        if (DeviceIoControl(hDrive, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, NULL)) {
            std::cout << "Drive " << driveLetter << " unlocked successfully." << std::endl;
        } else {
            std::cerr << "Error: Unable to unlock drive " << driveLetter << std::endl;
        }
    }

    CloseHandle(hDrive);
}

void DisplayDisks() {
    DWORD drives = GetLogicalDrives();
    std::cout << "Available drives:" << std::endl;

    for (int i = 0; i < 26; ++i) {
        if (drives & (1 << i)) {
            std::cout << char('A' + i) << ":\\ ";
        }
    }
    std::cout << std::endl;
}

int main() {
    int choice;
    char driveLetter;
    std::vector<char> selectedDisks;
    bool lockAll = false, unlockAll = false;

    std::cout << "1. Lock all disks\n2. Unlock all disks\n3. Select specific disks to lock\n4. Select specific disks to unlock" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    switch (choice) {
    case 1:
        lockAll = true;
        break;
    case 2:
        unlockAll = true;
        break;
    case 3:
        std::cout << "Select disks to lock (enter drive letters, finish with 0):" << std::endl;
        DisplayDisks();
        while (true) {
            std::cin >> driveLetter;
            if (driveLetter == '0') break;
            selectedDisks.push_back(driveLetter);
        }
        break;
    case 4:
        std::cout << "Select disks to unlock (enter drive letters, finish with 0):" << std::endl;
        DisplayDisks();
        while (true) {
            std::cin >> driveLetter;
            if (driveLetter == '0') break;
            selectedDisks.push_back(driveLetter);
        }
        break;
    default:
        std::cerr << "Invalid choice" << std::endl;
        return 1;
    }

    if (lockAll) {
        DWORD drives = GetLogicalDrives();
        for (int i = 0; i < 26; ++i) {
            if (drives & (1 << i)) {
                LockUnlockDisk(char('A' + i), true);
            }
        }
    } else if (unlockAll) {
        DWORD drives = GetLogicalDrives();
        for (int i = 0; i < 26; ++i) {
            if (drives & (1 << i)) {
                LockUnlockDisk(char('A' + i), false);
            }
        }
    } else {
        for (char disk : selectedDisks) {
            LockUnlockDisk(disk, choice == 3);
        }
    }

    return 0;
}
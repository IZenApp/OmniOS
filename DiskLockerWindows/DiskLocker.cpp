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

void DisplayDisks(std::vector<char>& disks) {
    DWORD drives = GetLogicalDrives();
    for (int i = 0; i < 26; ++i) {
        if (drives & (1 << i)) {
            disks.push_back('A' + i);
        }
    }
}

int main() {
    std::vector<char> disks;
    DisplayDisks(disks);

    std::cout << "Available drives:" << std::endl;
    for (char disk : disks) {
        std::cout << disk << ": ";
    }
    std::cout << std::endl;

    char choice;
    std::cout << "Enter the drive letter you want to lock/unlock: ";
    std::cin >> choice;

    int option;
    std::cout << "Choose an option: \n1. Lock \n2. Unlock" << std::endl;
    std::cin >> option;

    if (option == 1) {
        LockUnlockDisk(choice, true);
    } else if (option == 2) {
        LockUnlockDisk(choice, false);
    } else {
        std::cerr << "Invalid option!" << std::endl;
    }

    return 0;
}

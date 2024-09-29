#include <windows.h>
#include <iostream>

void LockDrive(char driveLetter) {
    std::string drivePath = std::string("\\\\.\\") + driveLetter + ":";
    HANDLE hDevice = CreateFile(drivePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to lock drive " << driveLetter << std::endl;
        return;
    }

    DWORD bytesReturned;
    DeviceIoControl(hDevice, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, NULL);
    CloseHandle(hDevice);
    std::cout << "Drive " << driveLetter << " locked successfully." << std::endl;
}

void UnlockDrive(char driveLetter) {
    std::string drivePath = std::string("\\\\.\\") + driveLetter + ":";
    HANDLE hDevice = CreateFile(drivePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to unlock drive " << driveLetter << std::endl;
        return;
    }

    DWORD bytesReturned;
    DeviceIoControl(hDevice, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, NULL);
    CloseHandle(hDevice);
    std::cout << "Drive " << driveLetter << " unlocked successfully." << std::endl;
}

int main() {
    char driveLetter;
    char action;

    std::cout << "Enter drive letter (e.g., C): ";
    std::cin >> driveLetter;
    std::cout << "Enter action (L to lock, U to unlock): ";
    std::cin >> action;

    if (action == 'L' || action == 'l') {
        LockDrive(driveLetter);
    } else if (action == 'U' || action == 'u') {
        UnlockDrive(driveLetter);
    } else {
        std::cerr << "Invalid action." << std::endl;
    }

    return 0;
}
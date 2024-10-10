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
        MessageBox(NULL, ("Error: Unable to open drive " + std::string(1, driveLetter)).c_str(), "Error", MB_OK);
        return;
    }

    DWORD bytesReturned;
    if (lock) {
        if (DeviceIoControl(hDrive, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, NULL)) {
            MessageBox(NULL, ("Drive " + std::string(1, driveLetter) + " locked successfully.").c_str(), "Success", MB_OK);
        } else {
            MessageBox(NULL, ("Error: Unable to lock drive " + std::string(1, driveLetter)).c_str(), "Error", MB_OK);
        }
    } else {
        if (DeviceIoControl(hDrive, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, NULL)) {
            MessageBox(NULL, ("Drive " + std::string(1, driveLetter) + " unlocked successfully.").c_str(), "Success", MB_OK);
        } else {
            MessageBox(NULL, ("Error: Unable to unlock drive " + std::string(1, driveLetter)).c_str(), "Error", MB_OK);
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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static std::vector<char> disks;
    static HWND hComboBox;

    switch (uMsg) {
    case WM_CREATE:
        DisplayDisks(disks);
        hComboBox = CreateWindow("COMBOBOX", NULL,
            WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL,
            20, 20, 150, 100, hwnd, NULL, NULL, NULL);

        for (char disk : disks) {
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)(std::string(1, disk) + ":").c_str());
        }

        CreateWindow("BUTTON", "Lock Disk",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            20, 60, 100, 30, hwnd, (HMENU)1, NULL, NULL);

        CreateWindow("BUTTON", "Unlock Disk",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            130, 60, 100, 30, hwnd, (HMENU)2, NULL, NULL);

        CreateWindow("BUTTON", "Lock All Disks",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            20, 100, 100, 30, hwnd, (HMENU)3, NULL, NULL);

        CreateWindow("BUTTON", "Unlock All Disks",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            130, 100, 100, 30, hwnd, (HMENU)4, NULL, NULL);

        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) { // Lock Disk
            char disk;
            SendMessage(hComboBox, CB_GETLBTEXT, SendMessage(hComboBox, CB_GETCURSEL, 0, 0), (LPARAM)&disk);
            LockUnlockDisk(disk, true);
        }
        else if (LOWORD(wParam) == 2) { // Unlock Disk
            char disk;
            SendMessage(hComboBox, CB_GETLBTEXT, SendMessage(hComboBox, CB_GETCURSEL, 0, 0), (LPARAM)&disk);
            LockUnlockDisk(disk, false);
        }
        else if (LOWORD(wParam) == 3) { // Lock All Disks
            DWORD drives = GetLogicalDrives();
            for (int i = 0; i < 26; ++i) {
                if (drives & (1 << i)) {
                    LockUnlockDisk('A' + i, true);
                }
            }
        }
        else if (LOWORD(wParam) == 4) { // Unlock All Disks
            DWORD drives = GetLogicalDrives();
            for (int i = 0; i < 26; ++i) {
                if (drives & (1 << i)) {
                    LockUnlockDisk('A' + i, false);
                }
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int main() {
    const char CLASS_NAME[] = "DiskLockerWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "Disk Locker",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        300, 200, NULL, NULL, wc.hInstance, NULL);

    ShowWindow(hwnd, SW_SHOWDEFAULT);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, WM_QUIT)) { // Указаны минимальные и максимальные значения сообщений
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
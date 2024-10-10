#include <windows.h>
#include <vector>
#include <string>

void LockUnlockDisk(char driveLetter, bool lock);
void DisplayDisks(std::vector<char>& disks);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static std::vector<char> disks;
    static HWND comboBox, lockButton, unlockButton;

    switch (uMsg) {
    case WM_CREATE: {
        DisplayDisks(disks);

        comboBox = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
                                20, 20, 150, 200, hwnd, NULL, NULL, NULL);
        
        for (char disk : disks) {
            std::string diskStr = std::string(1, disk) + ":";
            SendMessage(comboBox, CB_ADDSTRING, 0, (LPARAM)diskStr.c_str());
        }

        lockButton = CreateWindow("BUTTON", "Lock", WS_CHILD | WS_VISIBLE,
                                  20, 60, 100, 30, hwnd, (HMENU)1, NULL, NULL);
        
        unlockButton = CreateWindow("BUTTON", "Unlock", WS_CHILD | WS_VISIBLE,
                                    130, 60, 100, 30, hwnd, (HMENU)2, NULL, NULL);
        break;
    }

    case WM_COMMAND: {
        if (HIWORD(wParam) == BN_CLICKED) {
            int itemIndex = SendMessage(comboBox, CB_GETCURSEL, 0, 0);
            if (itemIndex != CB_ERR) {
                char driveLetter = disks[itemIndex];
                
                if (LOWORD(wParam) == 1) { // Lock
                    LockUnlockDisk(driveLetter, true);
                } else if (LOWORD(wParam) == 2) { // Unlock
                    LockUnlockDisk(driveLetter, false);
                }
            }
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char* className = "DiskLockerWindowClass";
    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, className, "Disk Locker",
                               WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 150,
                               NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void LockUnlockDisk(char driveLetter, bool lock) {
    std::string drivePath = "\\\\.\\";
    drivePath += driveLetter;
    drivePath += ":";

    HANDLE hDrive = CreateFile(drivePath.c_str(),
                               GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL, OPEN_EXISTING, 0, NULL);

    if (hDrive == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, "Failed to open disk!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    DWORD bytesReturned;
    if (lock) {
        if (DeviceIoControl(hDrive, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, NULL)) {
            MessageBox(NULL, "Drive locked successfully!", "Success", MB_OK | MB_ICONINFORMATION);
        } else {
            MessageBox(NULL, "Cannot lock disk!", "Error", MB_OK | MB_ICONERROR);
        }
    } else {
        if (DeviceIoControl(hDrive, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, NULL)) {
            MessageBox(NULL, "Drive unlocked successfully!", "Success", MB_OK | MB_ICONINFORMATION);
        } else {
            MessageBox(NULL, "Failed to unlock disk!", "Error", MB_OK | MB_ICONERROR);
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
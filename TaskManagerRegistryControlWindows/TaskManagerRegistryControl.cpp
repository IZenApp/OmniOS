#include <iostream>
#include <cstdlib>

void BlockTaskManagerAndRegistry() {
    system("reg add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\" /v DisableTaskMgr /t REG_DWORD /d 1 /f");
    system("reg add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\" /v DisableRegistryTools /t REG_DWORD /d 1 /f");
    std::cout << "Task Manager and Registry Editor are blocked." << std::endl;
}

void UnblockTaskManagerAndRegistry() {
    system("reg delete \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\" /v DisableTaskMgr /f");
    system("reg delete \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\" /v DisableRegistryTools /f");
    std::cout << "Task Manager and Registry Editor are unblocked." << std::endl;
}

void ShowMenu() {
    std::cout << "Select an action:" << std::endl;
    std::cout << "1: Block Task Manager and Registry Editor" << std::endl;
    std::cout << "2: Unblock Task Manager and Registry Editor" << std::endl;
    std::cout << "0: Exit" << std::endl;
}

int main() {
    int choice;
    do {
        ShowMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                BlockTaskManagerAndRegistry();
                break;
            case 2:
                UnblockTaskManagerAndRegistry();
                break;
            case 0:
                std::cout << "Exiting the program." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Try again." << std::endl;
                break;
        }
    } while (choice != 0);
    return 0;
}
#include <iostream>
#include <cstdlib>

void BlockTaskManager() {
    system("reg add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\" /v DisableTaskMgr /t REG_DWORD /d 1 /f");
    std::cout << "\n✅ Task Manager is now blocked." << std::endl;
}

void UnblockTaskManager() {
    system("PowerShell -Command \"Start-Process PowerShell -Verb RunAs -ArgumentList '-NoProfile -ExecutionPolicy Bypass -Command \"Remove-ItemProperty -Path \\\"HKCU:\\\\Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Policies\\\\System\\\" -Name \\\"DisableTaskMgr\\\" -ErrorAction SilentlyContinue\"'\"");
    std::cout << "\n✅ Unblock command for Task Manager executed. Please check permissions." << std::endl;
}

void BlockRegistryEditor() {
    system("reg add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\" /v DisableRegistryTools /t REG_DWORD /d 1 /f");
    std::cout << "\n✅ Registry Editor is now blocked." << std::endl;
}

void UnblockRegistryEditor() {
    system("PowerShell -Command \"Start-Process PowerShell -Verb RunAs -ArgumentList '-NoProfile -ExecutionPolicy Bypass -Command \"Remove-ItemProperty -Path \\\"HKCU:\\\\Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Policies\\\\System\\\" -Name \\\"DisableRegistryTools\\\" -ErrorAction SilentlyContinue\"'\"");
    std::cout << "\n✅ Unblock command for Registry Editor executed. Please check permissions." << std::endl;
}

void BlockTaskManagerAndRegistry() {
    BlockTaskManager();
    BlockRegistryEditor();
    std::cout << "\n✅ Task Manager and Registry Editor are now blocked." << std::endl;
}

void UnblockTaskManagerAndRegistry() {
    UnblockTaskManager();
    UnblockRegistryEditor();
    std::cout << "\n✅ Unblock command executed for both. Please check permissions." << std::endl;
}

void ShowMenu() {
    std::cout << "\n|============================================|" << std::endl;
    std::cout << "|                Main Menu                   |" << std::endl;
    std::cout << "|============================================|" << std::endl;
    std::cout << "|1: Block Task Manager                       |" << std::endl;
    std::cout << "|2: Unblock Task Manager                     |" << std::endl;
    std::cout << "|3: Block Registry Editor                    |" << std::endl;
    std::cout << "|4: Unblock Registry Editor                  |" << std::endl;
    std::cout << "|5: Block Task Manager and Registry Editor   |" << std::endl;
    std::cout << "|6: Unblock Task Manager and Registry Editor |" << std::endl;
    std::cout << "|0: Exit                                     |" << std::endl;
    std::cout << "|============================================|" << std::endl;
}

int main() {
    int choice;
    do {
        ShowMenu();
        std::cout << "\nEnter your choice: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                BlockTaskManager();
                break;
            case 2:
                UnblockTaskManager();
                break;
            case 3:
                BlockRegistryEditor();
                break;
            case 4:
                UnblockRegistryEditor();
                break;
            case 5:
                BlockTaskManagerAndRegistry();
                break;
            case 6:
                UnblockTaskManagerAndRegistry();
                break;
            case 0:
                std::cout << "👋 Exiting the program." << std::endl;
                break;
            default:
                std::cout << "❌ Invalid choice. Please try again." << std::endl;
                break;
        }
    } while (choice != 0);
    return 0;
}

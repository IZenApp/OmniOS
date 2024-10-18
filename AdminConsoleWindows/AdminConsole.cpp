#include <windows.h>
#include <iostream>
#include <string>
#include <sddl.h>

// Function declarations
bool IsRunningAsAdmin();
void RequestElevation();
void RunCommand(const std::string& command, std::string& currentDir);

void RunCommand(const std::string& command, std::string& currentDir) {
    std::string modifiedCommand = command;

    // Replace "ls" with "dir"
    if (command.substr(0, 2) == "ls") {
        modifiedCommand.replace(0, 2, "dir");
    }

    // Handle "cd" command
    if (command.substr(0, 3) == "cd ") {
        std::string newDir = command.substr(3);
        if (SetCurrentDirectory(newDir.c_str())) {
            currentDir = newDir; // Update current directory
            return; // No need to execute further
        } else {
            std::cerr << "Failed to change directory to: " << newDir << std::endl;
            return;
        }
    }

    // Execute the modified command
    system(modifiedCommand.c_str());
}

bool IsRunningAsAdmin() {
    HANDLE token;
    BOOL isAdmin = FALSE;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
        DWORD size;
        GetTokenInformation(token, TokenGroups, NULL, 0, &size);
        BYTE* buffer = new BYTE[size];

        if (GetTokenInformation(token, TokenGroups, buffer, size, &size)) {
            PSID adminSid = NULL;
            ConvertStringSidToSid("S-1-5-32-544", &adminSid);

            TOKEN_GROUPS* tokenGroups = (TOKEN_GROUPS*)buffer;
            for (DWORD i = 0; i < tokenGroups->GroupCount; ++i) {
                if (EqualSid(adminSid, tokenGroups->Groups[i].Sid)) {
                    isAdmin = TRUE;
                    break;
                }
            }
            LocalFree(adminSid);
        }
        delete[] buffer;
    }

    return isAdmin; // Return whether the user is an admin
}

void RequestElevation() {
    // Request elevation
    ShellExecute(NULL, "runas", "cmd.exe", NULL, NULL, SW_SHOWNORMAL);
}

int main() {
    if (!IsRunningAsAdmin()) {
        std::cout << "Please run the program as administrator." << std::endl;
        RequestElevation();
        return 1;
    }

    std::string command;
    std::string currentDir = "AdminConsole\\"; // Start in the C: drive

    while (true) {
        std::cout << currentDir << "> "; // Show current directory
        std::getline(std::cin, command);
        
        if (command == "exit") {
            break;
        }

        RunCommand(command, currentDir);
    }

    return 0;
}

#include <iostream>
#include <cstdlib>
#include <string>

void check_network() {
    std::string os_type;
#ifdef _WIN32
    os_type = "Windows";
#else
    os_type = "Unix";
#endif

    std::string command;
    if (os_type == "Windows") {
        command = "ping 8.8.8.8 -n 10";
    } else {
        command = "ping 8.8.8.8 -c 10";
    }

    int result = std::system(command.c_str());
    if (result == 0) {
        std::cout << "Network is available" << std::endl;
    } else {
        std::cout << "Network is unavailable" << std::endl;
    }
}

int main() {
    check_network();
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}
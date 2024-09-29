#include <iostream>
#include <cstdlib>

void lock_disk(const std::string& disk) {
    std::string command = "cryptsetup luksClose " + disk;
    if (system(command.c_str()) == 0) {
        std::cout << "Disk " << disk << " locked successfully." << std::endl;
    } else {
        std::cerr << "Failed to lock the disk " << disk << "." << std::endl;
    }
}

void unlock_disk(const std::string& disk, const std::string& mapper_name) {
    std::string command = "cryptsetup luksOpen " + disk + " " + mapper_name;
    if (system(command.c_str()) == 0) {
        std::cout << "Disk " << disk << " unlocked successfully as " << mapper_name << "." << std::endl;
    } else {
        std::cerr << "Failed to unlock the disk " << disk << "." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <lock|unlock> <disk> [mapper_name]" << std::endl;
        return 1;
    }

    std::string action = argv[1];
    std::string disk = argv[2];

    if (action == "lock") {
        lock_disk(disk);
    } else if (action == "unlock") {
        if (argc < 4) {
            std::cerr << "For unlocking, you need to specify a mapper name." << std::endl;
            return 1;
        }
        std::string mapper_name = argv[3];
        unlock_disk(disk, mapper_name);
    } else {
        std::cerr << "Unknown action: " << action << std::endl;
        return 1;
    }

    return 0;
}
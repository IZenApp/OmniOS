#include <windows.h>
#include <wincrypt.h>
#include <iostream>
#include <fstream>

#pragma comment(lib, "Advapi32.lib")

// Функция для шифрования файла на выбранном диске
void EncryptFile(const std::string& filename) {
    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Ошибка: не удалось открыть файл для шифрования.\n";
        return;
    }

    std::string fileData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    HCRYPTPROV hProv;
    HCRYPTKEY hKey;

    // Инициализация криптопровайдера
    if (!CryptAcquireContext(&hProv, nullptr, nullptr, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        std::cerr << "Ошибка инициализации криптопровайдера.\n";
        return;
    }

    // Генерация симметричного ключа для шифрования
    if (!CryptGenKey(hProv, CALG_AES_256, CRYPT_EXPORTABLE, &hKey)) {
        std::cerr << "Ошибка генерации ключа.\n";
        CryptReleaseContext(hProv, 0);
        return;
    }

    // Шифрование данных
    DWORD dataLen = fileData.size();
    if (!CryptEncrypt(hKey, 0, TRUE, 0, reinterpret_cast<BYTE*>(&fileData[0]), &dataLen, fileData.size())) {
        std::cerr << "Ошибка шифрования данных.\n";
        CryptDestroyKey(hKey);
        CryptReleaseContext(hProv, 0);
        return;
    }

    // Сохраняем зашифрованный файл
    std::ofstream outputFile(filename + ".enc", std::ios::binary);
    outputFile.write(fileData.c_str(), dataLen);
    outputFile.close();

    std::cout << "Файл успешно зашифрован: " << filename + ".enc" << std::endl;

    CryptDestroyKey(hKey);
    CryptReleaseContext(hProv, 0);
}

// Функция для расшифровки файла на выбранном диске
void DecryptFile(const std::string& filename) {
    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Ошибка: не удалось открыть зашифрованный файл.\n";
        return;
    }

    std::string encryptedData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    HCRYPTPROV hProv;
    HCRYPTKEY hKey;

    if (!CryptAcquireContext(&hProv, nullptr, nullptr, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        std::cerr << "Ошибка инициализации криптопровайдера.\n";
        return;
    }

    // Восстановление ключа (упрощенный вариант, ключ должен быть сохранен)
    if (!CryptGenKey(hProv, CALG_AES_256, CRYPT_EXPORTABLE, &hKey)) {
        std::cerr << "Ошибка генерации ключа для расшифровки.\n";
        CryptReleaseContext(hProv, 0);
        return;
    }

    // Расшифровка данных
    DWORD dataLen = encryptedData.size();
    if (!CryptDecrypt(hKey, 0, TRUE, 0, reinterpret_cast<BYTE*>(&encryptedData[0]), &dataLen)) {
        std::cerr << "Ошибка расшифровки данных.\n";
        CryptDestroyKey(hKey);
        CryptReleaseContext(hProv, 0);
        return;
    }

    // Сохранение расшифрованного файла
    std::ofstream outputFile(filename + ".dec", std::ios::binary);
    outputFile.write(encryptedData.c_str(), dataLen);
    outputFile.close();

    std::cout << "Файл успешно расшифрован: " << filename + ".dec" << std::endl;

    CryptDestroyKey(hKey);
    CryptReleaseContext(hProv, 0);
}

// Функция для вывода списка дисков
void ListDisks() {
    char driveLetter[4] = "A:\\";
    DWORD drives = GetLogicalDrives();

    std::cout << "Доступные диски:\n";
    for (int i = 0; i < 26; i++) {
        if (drives & (1 << i)) {
            driveLetter[0] = 'A' + i;
            std::cout << driveLetter << std::endl;
        }
    }
}

// Основная функция
int main() {
    int choice;
    std::string disk, filename;

    std::cout << "Выберите действие:\n";
    std::cout << "1. Зашифровать файл\n";
    std::cout << "2. Расшифровать файл\n";
    std::cout << "3. Показать доступные диски\n";
    std::cout << "Введите ваш выбор: ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            std::cout << "Введите диск для шифрования (например, D:\\): ";
            std::cin >> disk;
            std::cout << "Введите имя файла для шифрования (например, D:\\file.txt): ";
            std::cin >> filename;
            EncryptFile(filename);
            break;
        case 2:
            std::cout << "Введите диск для расшифровки (например, D:\\): ";
            std::cin >> disk;
            std::cout << "Введите имя файла для расшифровки (например, D:\\file.enc): ";
            std::cin >> filename;
            DecryptFile(filename);
            break;
        case 3:
            ListDisks();
            break;
        default:
            std::cout << "Неправильный выбор!\n";
            break;
    }

    return 0;
}
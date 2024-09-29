#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

void encrypt(const std::string& inputFile, const std::string& outputFile, const std::string& password) {
    unsigned char key[32], iv[16];
    EVP_CIPHER_CTX* ctx;

    if (!RAND_bytes(key, sizeof(key)) || !RAND_bytes(iv, sizeof(iv))) {
        handleErrors();
    }

    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
    }

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        handleErrors();
    }

    std::ifstream ifs(inputFile, std::ios::binary);
    std::ofstream ofs(outputFile, std::ios::binary);

    ofs.write((char*)iv, sizeof(iv));

    std::vector<unsigned char> buffer(1024);
    std::vector<unsigned char> ciphertext(1024 + EVP_CIPHER_block_size(EVP_aes_256_cbc()));

    int len;
    while (ifs.read((char*)buffer.data(), buffer.size())) {
        if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, buffer.data(), ifs.gcount())) {
            handleErrors();
        }
        ofs.write((char*)ciphertext.data(), len);
    }

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data(), &len)) {
        handleErrors();
    }
    ofs.write((char*)ciphertext.data(), len);

    EVP_CIPHER_CTX_free(ctx);
}

void decrypt(const std::string& inputFile, const std::string& outputFile, const std::string& password) {
    unsigned char key[32], iv[16];
    EVP_CIPHER_CTX* ctx;

    std::ifstream ifs(inputFile, std::ios::binary);
    std::ofstream ofs(outputFile, std::ios::binary);

    ifs.read((char*)iv, sizeof(iv));

    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
    }

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        handleErrors();
    }

    std::vector<unsigned char> buffer(1024);
    std::vector<unsigned char> plaintext(1024 + EVP_CIPHER_block_size(EVP_aes_256_cbc()));

    int len;
    while (ifs.read((char*)buffer.data(), buffer.size())) {
        if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, buffer.data(), ifs.gcount())) {
            handleErrors();
        }
        ofs.write((char*)plaintext.data(), len);
    }

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data(), &len)) {
        handleErrors();
    }
    ofs.write((char*)plaintext.data(), len);

    EVP_CIPHER_CTX_free(ctx);
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <encrypt|decrypt> <input file> <output file> <password>" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];
    std::string password = argv[4];

    if (mode == "encrypt") {
        encrypt(inputFile, outputFile, password);
    } else if (mode == "decrypt") {
        decrypt(inputFile, outputFile, password);
    } else {
        std::cerr << "Invalid mode: " << mode << std::endl;
        return 1;
    }

    return 0;
}
# Python
import os
from cryptography.fernet import Fernet

# Генерация и сохранение ключа
def generate_key():
    key = Fernet.generate_key()
    with open("secret.key", "wb") as key_file:
        key_file.write(key)

# Загрузка ключа
def load_key():
    return open("secret.key", "rb").read()

# Шифрование файла
def encrypt_file(file_name):
    key = load_key()
    fernet = Fernet(key)
    with open(file_name, "rb") as file:
        file_data = file.read()
    encrypted_data = fernet.encrypt(file_data)
    with open(file_name, "wb") as file:
        file.write(encrypted_data)

# Дешифрование файла
def decrypt_file(file_name):
    key = load_key()
    fernet = Fernet(key)
    with open(file_name, "rb") as file:
        encrypted_data = file.read()
    decrypted_data = fernet.decrypt(encrypted_data)
    with open(file_name, "wb") as file:
        file.write(decrypted_data)

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="Disk Locker")
    parser.add_argument("action", choices=["encrypt", "decrypt"], help="Action to perform")
    parser.add_argument("file", help="File to encrypt/decrypt")
    args = parser.parse_args()

    if args.action == "encrypt":
        encrypt_file(args.file)
    elif args.action == "decrypt":
        decrypt_file(args.file)
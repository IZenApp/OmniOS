#!/bin/bash

# Функция для проверки наличия Python
check_python_installed() {
    if command -v python3 &>/dev/null; then
        echo "Python уже установлен"
    else
        install_python
    fi
}

# Функция для установки Python
install_python() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        echo "Установка Python для Linux..."
        sudo apt-get update
        sudo apt-get install -y python3
    elif [[ "$OSTYPE" == "msys" ]]; then
        echo "Установка Python для Windows..."
        powershell.exe -Command "Start-Process powershell -ArgumentList '-NoProfile -ExecutionPolicy Bypass -Command \"Invoke-WebRequest -Uri https://www.python.org/ftp/python/3.9.7/python-3.9.7-amd64.exe -OutFile python-installer.exe; Start-Process python-installer.exe -ArgumentList \"/quiet InstallAllUsers=1 PrependPath=1\" -Wait\"' -Verb RunAs"
    else
        echo "Неизвестная операционная система. Установка Python не поддерживается."
        exit 1
    fi
}

# Проверка и установка Python при необходимости
check_python_installed

# Запуск Python-скрипта для проверки сети
python3 network_check.py
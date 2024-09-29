# Функция для проверки наличия Python
function Check-PythonInstalled {
    $python = Get-Command python3 -ErrorAction SilentlyContinue
    if ($python) {
        Write-Output "Python уже установлен"
    } else {
        Install-Python
    }
}

# Функция для установки Python
function Install-Python {
    Write-Output "Установка Python для Windows..."
    Invoke-WebRequest -Uri "https://www.python.org/ftp/python/3.9.7/python-3.9.7-amd64.exe" -OutFile "python-installer.exe"
    Start-Process "python-installer.exe" -ArgumentList "/quiet InstallAllUsers=1 PrependPath=1" -Wait
    Remove-Item "python-installer.exe"
}

# Проверка и установка Python при необходимости
Check-PythonInstalled

# Запуск Python-скрипта для проверки сети
python3 network_check.py
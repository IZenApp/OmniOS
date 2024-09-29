# Функция для запрета диспетчера задач и редактора реестра
function Block-TaskManagerAndRegistry {
    New-ItemProperty -Path "HKCU:\Software\Microsoft\Windows\CurrentVersion\Policies\System" -Name "DisableTaskMgr" -Value 1 -PropertyType DWORD -Force
    New-ItemProperty -Path "HKCU:\Software\Microsoft\Windows\CurrentVersion\Policies\System" -Name "DisableRegistryTools" -Value 1 -PropertyType DWORD -Force
    Write-Output "Диспетчер задач и редактор реестра запрещены."
}

# Функция для разрешения диспетчера задач и редактора реестра
function Unblock-TaskManagerAndRegistry {
    Remove-ItemProperty -Path "HKCU:\Software\Microsoft\Windows\CurrentVersion\Policies\System" -Name "DisableTaskMgr" -ErrorAction SilentlyContinue
    Remove-ItemProperty -Path "HKCU:\Software\Microsoft\Windows\CurrentVersion\Policies\System" -Name "DisableRegistryTools" -ErrorAction SilentlyContinue
    Write-Output "Диспетчер задач и редактор реестра разрешены."
}

# Меню выбора
function Show-Menu {
    Write-Output "Выберите действие:"
    Write-Output "1: Запретить диспетчер задач и редактор реестра"
    Write-Output "2: Разрешить диспетчер задач и редактор реестра"
    Write-Output "0: Выход"
}

# Основной скрипт
do {
    Show-Menu
    $choice = Read-Host "Введите ваш выбор"
    switch ($choice) {
        1 { Block-TaskManagerAndRegistry }
        2 { Unblock-TaskManagerAndRegistry }
        0 { Write-Output "Выход из программы."; break }
        default { Write-Output "Неверный выбор. Попробуйте снова." }
    }
} while ($choice -ne 0)
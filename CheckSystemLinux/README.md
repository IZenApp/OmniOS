# Проверка системы на ошибки

Этот скрипт предназначен для проверки системы на наличие ошибок. Он выполняет следующие действия:
- Проверяет системные журналы на ошибки.
- Проверяет файловую систему на ошибки.
- Проверяет использование диска.
- Проверяет использование памяти.
- Проверяет статус основных служб.

## Установка

1. Сохраните скрипт в файл, например `CheckSystemLinux.sh`.
2. Сделайте файл исполняемым:

    ```sh
    chmod +x CheckSystemLinux.sh
    ```

## Использование

Запустите скрипт командой:

```sh
./CheckSystemLinux.sh
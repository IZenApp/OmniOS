#include <iostream>

void showMenu() {
    std::cout << "Выберите операцию:\n";
    std::cout << "1. Сложение\n";
    std::cout << "2. Вычитание\n";
    std::cout << "3. Умножение\n";
    std::cout << "4. Деление\n";
    std::cout << "5. Выход\n";
}

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (b != 0) {
        return a / b;
    } else {
        std::cerr << "Ошибка: Деление на ноль!" << std::endl;
        return 0;
    }
}

int main() {
    int choice;
    double num1, num2;

    while (true) {
        showMenu();
        std::cout << "Введите ваш выбор: ";
        std::cin >> choice;

        if (choice == 5) {
            break;
        }

        std::cout << "Введите первое число: ";
        std::cin >> num1;
        std::cout << "Введите второе число: ";
        std::cin >> num2;

        switch (choice) {
            case 1:
                std::cout << "Результат: " << add(num1, num2) << std::endl;
                break;
            case 2:
                std::cout << "Результат: " << subtract(num1, num2) << std::endl;
                break;
            case 3:
                std::cout << "Результат: " << multiply(num1, num2) << std::endl;
                break;
            case 4:
                std::cout << "Результат: " << divide(num1, num2) << std::endl;
                break;
            default:
                std::cout << "Неверный выбор. Пожалуйста, попробуйте снова." << std::endl;
        }
    }

    return 0;
}
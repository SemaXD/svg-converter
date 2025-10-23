#include "geometry2d.h"
#include <iostream>
#include <string>

#ifdef _WIN32 // Проверяем, компилируется ли программа на Windows. Если НЕТ, то элементы не для Windows отключаются
#include <windows.h>
#include <cstdlib>
#else
#include <cstdlib>
#endif

// Функция очистки консоли
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif


}
int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul"); // Устанавливает UTF-8
    #endif

    Geometry2D geom;

    std::string bmpFile = "input.bmp"; //Монохромный bmp для конвертации в txt
    std::string txtFile = "output.txt"; // Файл с булевыми данными об изображении (единицы - исследуемая область)
    std::string svgFile = "output.svg"; // Файл с веторной графикой

    std::cout << "Добро пожаловать в Geometry2D Converter!\n";

    while (true) {
        std::cout << "\n=========================================\n";
        std::cout << "Выберите действие:\n";
        std::cout << "1. Преобразовать изображение BMP в TXT\n";
        std::cout << "2. Преобразовать TXT в SVG\n";
        std::cout << "0. Закрыть программу\n";
        std::cout << "ПРИМЕЧАНИЕ! Для использования путей файлов по-умолчанию, вводите 'def'\n";
        std::cout << "=========================================\n";

        std::string inputNumber;
        std::cout << "Введите номер команды: ";
        std::cin >> inputNumber;

        if (inputNumber == "1") {
            std::cout << "Введите путь к BMP файлу: ";
            std::cin >> bmpFile;
            std::cout << "Введите путь для сохранения TXT: ";
            std::cin >> txtFile;
            if (bmpFile == "def") bmpFile = "input.bmp";
            if (txtFile == "def") txtFile = "output.txt";

            if (geom.readBMPAndSaveAsTXT(bmpFile, txtFile)) {
                std::cout << "\n✅  BMP успешно конвертирован в TXT!\n";
            } else {
                std::cout << "\n❌  Ошибка при конвертации BMP в TXT.\n";
            }
        } else if (inputNumber == "2") {
            std::cout << "Введите путь к TXT файлу: ";
            std::cin >> txtFile;
            std::cout << "Введите путь для сохранения SVG: ";
            std::cin >> svgFile;
            if (txtFile == "def") txtFile = "output.txt";
            if (svgFile == "def") svgFile = "output.svg";

            if (geom.convertTXTToSVG(txtFile, svgFile)) {
                std::cout << "\n✅  TXT успешно конвертирован в SVG!\n";
            } else {
                std::cout << "\n❌  Ошибка при конвертации TXT в SVG.\n";
            }
        } else if (inputNumber == "0") {
            std::cout << "\n👋  Программа завершена.\n";
            break;
        } else {
            std::cout << "\n⚠️  Неверный ввод. Попробуйте снова.\n";
        }

        std::cout << "\nНажмите Enter для продолжения...";
        std::cin.ignore();
        std::cin.get();

        clearScreen();
    }

    return 0;
}

// 1. Сделать очистку консоли отдельной командой ------------------- РЕШЕНО
// 2. Улучшить блок, отвечающий за интерфейс (некрасиво как-то) ---- РЕШЕНО
// 3. Рассмотреть возможность работы через tkinter------------------ Отмена, трудно будет скомпилировать в один экз
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
    std::string txtFinalFile = "output_matrix.txt"; // txt файл с финальными расчётами 

    std::cout << "Добро пожаловать в Geometry2D Converter!\n";

    while (true) {
        std::cout << "\n=========================================\n";
        std::cout << "Выберите действие:\n";
        std::cout << "1. Преобразовать изображение BMP в TXT\n";
        std::cout << "2. Преобразовать TXT в SVG\n";
        std::cout << "3. ТЕСТ! Умножить матрицу на 2 и замерить время работы\n";
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


        } else if (inputNumber == "3") {
            std::cout << "Введите путь к входному TXT файлу (с матрицей): ";
            std::cin >> txtFile;
            std::cout << "Введите путь для сохранения результата: ";
            std::cin >> txtFinalFile;
            if (txtFile == "def") txtFile = "output.txt";
            if (txtFinalFile == "def") txtFinalFile = "output_matrix.txt";

            if (geom.multiplyMatrixByTwo(txtFile, txtFinalFile)) {
                std::cout << "\n✅  Матрица успешно умножена на 2 и сохранена!\n";
            } else {
                std::cout << "\n❌  Ошибка при обработке матрицы.\n";
            }


        } else if (inputNumber == "0") {
            std::cout << "\n👋  Программа завершена.\n";
            break;
        } else {
            std::cout << "\n⚠️  Неверный ввод. Попробуйте снова.\n";
        }

        std::cout << "\nНажмите Enter для продолжения...";
        std::cin.ignore(); //удаляет один символ из буфера обмена (обычно \n)
        std::cin.get(); // Ждёт нажатия Enter для продолжения 

        clearScreen();
    }

    return 0;
}

// 1. Создать функции таймеров, понадобится для замеров 
// 2. Добавить OpenMP

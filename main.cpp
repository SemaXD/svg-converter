#include "geometry2d.h"
#include <iostream>

int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul"); // Устанавливает UTF-8
    #endif

    Geometry2D geom;

    std::string bmpFile = "input.bmp"; //Монохромный bmp для конвертации в txt
    std::string txtFile = "output.txt"; // Файл с булевыми данными об изображении (единицы - исследуемая область)
    std::string svgFile = "output.svg"; // Файл с веторной графикой

    if (geom.readBMPAndSaveAsTXT(bmpFile, txtFile)) {
        std::cout << "BMP успешно конвертирован в TXT!" << std::endl;
    } else {
        std::cout << "Ошибка при конвертации BMP в TXT." << std::endl;
    }
    
    if (geom.convertTXTToSVG(txtFile, svgFile)) {
            std::cout << "TXT успешно конвертирован в SVG!" << std::endl;
    } else {
            std::cout << "Ошибка при конвертации в SVG." << std::endl;
    }

    return 0;
}
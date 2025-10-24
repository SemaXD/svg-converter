#ifndef GEOMETRY2D_H
#define GEOMETRY2D_H

#include <vector>
#include <string>

class Geometry2D {
public:
    // Читает BMP файл и сохраняет его содержимое как TXT
    bool readBMPAndSaveAsTXT(const std::string& bmpFile, const std::string& txtFile);

    // Читает TXT файл и сохраняет содержимое как SVG
    bool convertTXTToSVG(const std::string& txtFile, const std::string& svgFile);

    // Умножает каждое значение матрицы на 2
    bool multiplyMatrixByTwo(const std::string& inputTxt, const std::string& outputTxt);

private:
    int width; // Ширина файла в пикселях
    int height; // Высота файла в пикселях
    std::vector<std::vector<int>> pixels; // Матрица для хранения данных о цвете пикселей, 0 = белый, 1 = черный

    // Сохраняет файл TXT
    void saveTXT(const std::string& filename) const;

    // Загружает файл TXT 
    bool loadTXT(const std::string& filename);
};

#endif
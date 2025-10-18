#include "geometry2d.h"
#include <fstream>
#include <iostream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Geometry2D::readBMPAndSaveAsTXT(const std::string& bmpFile, const std::string& txtFile) {
    int channels; // Переменная для проверки канала. Если 1, то файл монохромный
    unsigned char* data = stbi_load(bmpFile.c_str(), &width, &height, &channels, 1); // Указатель на массив с данными о пикселях
    if (!data) {
        std::cerr << "Не удалось загрузить BMP файл: " << bmpFile << std::endl;
        return false;
    }

    pixels.resize(height, std::vector<int>(width));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char pixel = data[y * width + x];
            pixels[y][x] = (pixel == 0) ? 1 : 0; // Массив с данными о пикселях, 0 = черный = 1, 255 = белый = 0
        }
    }

    stbi_image_free(data);
    saveTXT(txtFile);
    return true;
}

void Geometry2D::saveTXT(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось создать TXT файл: " << filename << std::endl;
        return;
    }

    file << "Geometry2d\n";
    file << width << " " << height << "\n";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            file << pixels[y][x];
            if (x < width - 1) file << " ";
        }
        file << "\n";
    }
}

bool Geometry2D::convertTXTToSVG(const std::string& txtFile, const std::string& svgFile) {
    if (!loadTXT(txtFile)) {
        std::cerr << "Не удалось загрузить TXT файл: " << txtFile << std::endl;
        return false;
    }

    std::ofstream file(svgFile);
    if (!file.is_open()) {
        std::cerr << "Не удалось создать SVG файл: " << svgFile << std::endl;
        return false;
    }

    file << "<svg width=\"" << width << "\" height=\"" << height << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (pixels[y][x] == 1) {  // черный пиксель
                file << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"1\" height=\"1\" fill=\"black\" />\n";
            }
        }
    }

    file << "</svg>\n";
    return true;
}

bool Geometry2D::loadTXT(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string header;
    std::getline(file, header);
    if (header != "Geometry2d") {
        std::cerr << "Файл не начинается с 'Geometry2d'" << std::endl;
        return false;
    }

    file >> width >> height;
    pixels.resize(height, std::vector<int>(width));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            file >> pixels[y][x];
        }
    }

    return true;
}
#include "geometry2d.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono> // библиотека для таймера
#include <vector>

// Подключаем OpenMP
#ifdef _OPENMP
#include <omp.h>
#else
// если OpenMP не включена, можно вывести предупреждение
#warning OpenMP не включена. Компилятор должен поддерживать и включать OpenMP.
#endif

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
            pixels[y][x] = (pixel == 0) ? 1 : 0; // 0 в BMP (черный) = 1 в данных, 255 в BMP (белый) = 0 в данных
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

bool Geometry2D::multiplyMatrixByTwo(const std::string& inputTxt, const std::string& outputTxt) {
    if (!loadTXT(inputTxt)) {
        std::cerr << "Не удалось загрузить TXT файл: " << inputTxt << std::endl;
        return false;
    }

    // Копируем матрицу для последовательной обработки
    auto sequentialPixels = pixels; // Матрица для однопоточной обработки

    // 1. Последовательный цикл
    auto start_seq = std::chrono::high_resolution_clock::now(); 

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            sequentialPixels[y][x] *= 2;
        }
    }

    auto end_seq = std::chrono::high_resolution_clock::now();
    auto duration_seq = std::chrono::duration_cast<std::chrono::microseconds>(end_seq - start_seq); //Время обработки на 1 потоке

    std::cout << "Время последовательного умножения: " << duration_seq.count() << " мкс\n";

    // 2. Распараллеленный цикл (OpenMP)
    auto start_omp = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pixels[y][x] *= 2;  
        }
    }

    auto end_omp = std::chrono::high_resolution_clock::now();
    auto duration_omp = std::chrono::duration_cast<std::chrono::microseconds>(end_omp - start_omp); //Время обработки на многопотоке

    std::cout << "Время распараллеленного умножения: " << duration_omp.count() << " мкс\n";

    // Сравнение
    if (duration_omp.count() > 0) {
        double speedup = static_cast<double>(duration_seq.count()) / duration_omp.count();
        int num_threads = omp_get_max_threads();  // Максимальное количество потоков. ВАЖНО! выдаёт ошибку, но это нормально
        double efficiency = speedup / num_threads;

        std::cout << "Ускорение (Speedup): " << speedup << "x\n";
        std::cout << "Количество потоков: " << num_threads << "\n";
        std::cout << "Эффективность: " << efficiency << "\n";
    } else {
        std::cout << "Время распараллеливания слишком мало для расчёта.\n";
    }


    
    saveTXT(outputTxt);
    return true;
}

//Есть вариант ООП реализации экспорта в svg. Надо будет попробовать.
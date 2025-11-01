#include "SvgColors.hpp"
#include "SvgWriter.hpp"
#include "geometry2d.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono> // библиотека для таймера
#include <vector>
#include <thread>

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

// bool Geometry2D::convertTXTToSVG(const std::string& txtFile, const std::string& svgFile) {
//     if (!loadTXT(txtFile)) {
//         std::cerr << "Не удалось загрузить TXT файл: " << txtFile << std::endl;
//         return false;
//     }

//     //Новый вариант
//     SVG::SvgWriter svg(width, height, "none");

//     SVG::Style blackRectStyle;
//     blackRectStyle.setFill(SVG::Color(0,0,0));

//     for (int y = 0; y < height; y++) {
//         for (int x = 0; x < width; x++) {
//             if (pixels[y][x] != 0) {  // черный пиксель
//                 svg.addRectangle(x,y,1,1, blackRectStyle);
//             }
//         }
//     }

//     if (svg.saveToFile(svgFile)) {
//         std::cout << "SVG-Файл сохранён с именем: "<< svgFile << std::endl;
//     } else {
//         std::cerr << "Ошибка при создании SVG-Файла" << std::endl;
//         //return 1;
//     }

//     //Старый вариант
//     // std::ofstream file(svgFile);
//     // if (!file.is_open()) {
//     //     std::cerr << "Не удалось создать SVG файл: " << svgFile << std::endl;
//     //     return false;
//     // }

//     // file << "<svg width=\"" << width << "\" height=\"" << height << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";

//     // for (int y = 0; y < height; y++) {
//     //     for (int x = 0; x < width; x++) {
//     //         if (pixels[y][x] != 0) {  // черный пиксель
//     //             file << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"1\" height=\"1\" fill=\"black\" />\n";
//     //         }
//     //     }
//     // }

//     // file << "</svg>\n";
    
//     return true;
// }

bool Geometry2D::convertTXTToSVG(const std::string& txtFile, const std::string& svgFile) {
    if (!loadTXT(txtFile)) {
        std::cerr << "Не удалось загрузить TXT файл: " << txtFile << std::endl;
        return false;
    }

    int target_size = 1000;
    int skip_factor_x = 1;
    int skip_factor_y = 1;

    // Проверяем, нужно ли сжимать
    if (width > target_size || height > target_size) {
        std::cout << "Размеры изображения: " << width << "x" << height << " превышают " << target_size << "x" << target_size << std::endl;
        std::cout << "Рекомендуется сжать изображение для корректного открытия SVG." << std::endl;

        char choice;
        std::cout << "Сжать изображение до ~" << target_size << "x" << target_size << "? (y/n): ";
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            skip_factor_x = width / target_size;
            skip_factor_y = height / target_size;

            // Округляем в меньшую сторону (целочисленное деление)
            if (skip_factor_x < 1) skip_factor_x = 1;
            if (skip_factor_y < 1) skip_factor_y = 1;

            std::cout << "Используется пропуск: каждый " << skip_factor_x << "-й пиксель по X и каждый " << skip_factor_y << "-й по Y." << std::endl;
        } else {
            std::cout << "Сжатие отменено. Продолжение с исходным размером." << std::endl;
        }
    }

    // Создаём SVGWriter с новыми размерами (после сжатия)
    int new_width = width / skip_factor_x;
    int new_height = height / skip_factor_y;

    SVG::SvgWriter svg(new_width, new_height, "none");
    SVG::Style blackRectStyle;
    blackRectStyle.setFill(SVG::Color(0,0,0));

    for (int y = 0; y < height; y += skip_factor_y) {
        for (int x = 0; x < width; x += skip_factor_x) {
            if (pixels[y][x] != 0) {  // черный пиксель
                // Масштабируем координаты
                int scaled_x = x / skip_factor_x;
                int scaled_y = y / skip_factor_y;
                svg.addRectangle(scaled_x, scaled_y, 1, 1, blackRectStyle);
            }
        }
    }

    if (svg.saveToFile(svgFile)) {
        std::cout << "SVG-Файл сохранён с именем: " << svgFile << std::endl;
    } else {
        std::cerr << "Ошибка при создании SVG-Файла" << std::endl;
        return false;
    }

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

bool Geometry2D::computeGradient(const std::string& inputTxt, const std::string& outputTxt, int n_threads_requested) {
    if (!loadTXT(inputTxt)) {
        std::cerr << "Не удалось загрузить TXT файл: " << inputTxt << std::endl;
        return false;
    }

    // Определяем реальное количество потоков
    int actual_num_threads = n_threads_requested;
    unsigned int max_hardware_threads = std::thread::hardware_concurrency();
    if (max_hardware_threads == 0) max_hardware_threads = 1; // страховка от систем с закрытым доступом к подобной информации

    if (n_threads_requested <= 0 || n_threads_requested > static_cast<int>(max_hardware_threads)) {
        actual_num_threads = max_hardware_threads;
        std::cout << "Количество потоков установлено: " << actual_num_threads << " (максимальное доступное)\n";
    } else {
        std::cout << "Количество потоков установлено: " << actual_num_threads << "\n";
    }

    // 1. Последовательный цикл
    auto start_seq = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<double>> gradX_seq(height, std::vector<double>(width, 0.0));
    std::vector<std::vector<double>> gradY_seq(height, std::vector<double>(width, 0.0));

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            gradX_seq[y][x] = (pixels[y][x + 1] - pixels[y][x - 1]) / 2.0;
            gradY_seq[y][x] = (pixels[y + 1][x] - pixels[y - 1][x]) / 2.0;
        }
    }

    auto end_seq = std::chrono::high_resolution_clock::now();
    auto duration_seq = std::chrono::duration_cast<std::chrono::microseconds>(end_seq - start_seq);

    std::cout << "Время последовательного вычисления градиента: " << duration_seq.count() << " мкс\n";

    // 2. Распараллеленный цикл (std::thread)
    auto start_omp = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<double>> gradX_omp(height, std::vector<double>(width, 0.0));
    std::vector<std::vector<double>> gradY_omp(height, std::vector<double>(width, 0.0));

    // Разбиваем диапазон на части для потоков
    int start_y = 1;
    int end_y = height - 1;
    int chunk_size = (end_y - start_y + actual_num_threads - 1) / actual_num_threads; // создаём блок для потоковой обработки

    std::vector<std::thread> threads;

    for (int t = 0; t < actual_num_threads; t++) {
        int thread_start_y = start_y + t * chunk_size;
        int thread_end_y = std::min(thread_start_y + chunk_size, end_y);

        threads.emplace_back([this, thread_start_y, thread_end_y, &gradX_omp, &gradY_omp]() { 
            for (int y = thread_start_y; y < thread_end_y; y++) {
                for (int x = 1; x < width - 1; x++) {
                    gradX_omp[y][x] = (pixels[y][x + 1] - pixels[y][x - 1]) / 2.0;
                    gradY_omp[y][x] = (pixels[y + 1][x] - pixels[y - 1][x]) / 2.0;
                }
            }
        });
    }

    // Ждём завершения всех потоков
    for (auto& t : threads) {
        t.join();
    }

    auto end_omp = std::chrono::high_resolution_clock::now();
    auto duration_omp = std::chrono::duration_cast<std::chrono::microseconds>(end_omp - start_omp);

    std::cout << "Время распараллеленного вычисления градиента: " << duration_omp.count() << " мкс\n";

    // Сравнение
    if (duration_omp.count() > 0) {
        double speedup = static_cast<double>(duration_seq.count()) / duration_omp.count();
        double efficiency = speedup / actual_num_threads;

        std::cout << "Ускорение (Speedup): " << speedup << "x\n";
        std::cout << "Количество потоков (реальное): " << actual_num_threads << "\n";
        std::cout << "Эффективность: " << efficiency << "\n";
    } else {
        std::cout << "Время распараллеливания слишком мало для расчёта.\n";
    }

    // Сохраняем результат (например, модуль градиента)
    std::ofstream file(outputTxt);
    if (!file.is_open()) {
        std::cerr << "Не удалось создать TXT файл: " << outputTxt << std::endl;
        return false;
    }

    file << "Geometry2d\n";
    file << width << " " << height << "\n";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double magnitude = std::sqrt(gradX_omp[y][x] * gradX_omp[y][x] + gradY_omp[y][x] * gradY_omp[y][x]);
            file << magnitude;
            if (x < width - 1) file << " ";
        }
        file << "\n";
    }

    return true;
}

//Нужно поработать с функциями, много лишнего кода

//Наблюдения на процессора Intel Core i7 12700H (6 мощностных ядер + hyper treading, 8 энергоэффективных ядер)
//При значении 12+ ядер - выдаёт плохую эффективность
//При значении 4 ядра - выдаёт эффективность 1+ , что странно, ведь система не с распределённой памятью

//Самый эффективный вариант: 2 ядра (70% эффективности)
//Самый быстрый вариант: 12 ядер ( 2.5 ускорения, то же самое, что и при 20)
//Самый оптимальный: 6 ядер (2.1-2.3 ускорения, 0.4 эффективность)
#include "class.hpp"

void CDOCS_files::save_file(const std::string& filename, const std::vector<std::string>& lines) {
    try {
        // Формируем полный путь для файла
        std::filesystem::path filepath = std::filesystem::path(output_path_) / filename;
        
        // Создаем родительские директории, если они не существуют
        std::filesystem::path parent_path = filepath.parent_path();
        if (!parent_path.empty() && !std::filesystem::exists(parent_path)) {
            if (!std::filesystem::create_directories(parent_path)) {
                throw std::runtime_error("Failed to create directories for path: " + parent_path.string());
            }
        }
        
        // Открываем файл по полному пути
        std::ofstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Error opening file: " + filepath.string());
        }
        
        // Записываем строки
        for (const auto& line : lines) {
            file << line << '\n';
        }
        // file.close() не нужен, деструктор ofstream закроет файл
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << '\n';
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    catch (...) {
        std::cerr << "Unknown error occurred while saving file: " << filename << '\n';
    }
}
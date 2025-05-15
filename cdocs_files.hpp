#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

class CDOCS_files {
public:
    CDOCS_files(const std::string& doc_root_path, 
        const std::string& doc_output_path,
        const std::vector<std::string>& extensions)
        : root_path_(doc_root_path), extensions_(), output_path_(doc_output_path) {
        
        // Check if the root path is valid
        if (!fs::exists(root_path_) || !fs::is_directory(root_path_)) {
            throw std::invalid_argument("Invalid documentation root path: " + root_path_);
        }

        // Check if the output path is valid
        if (!fs::exists(output_path_) || !fs::is_directory(output_path_)) {
            throw std::invalid_argument("Invalid documentation output path: " + output_path_);
        }

        // Нормализуем расширения: все приводим к формату с точкой (например, ".md")
        extensions_.reserve(extensions.size()); // Оптимизация памяти
        for (const auto& ext : extensions) {
            if (ext.empty()) {
                continue; // Пропускаем пустые расширения
            }
            // Если расширение уже с точкой, добавляем как есть, иначе добавляем точку
            extensions_.push_back(ext[0] == '.' ? ext : "." + ext);
        }
    }

    std::vector<std::string> get_files_list() {
        std::vector<std::string> found_files;

        try {
            for (const auto& entry : fs::recursive_directory_iterator(root_path_, fs::directory_options::skip_permission_denied)) {
                if (entry.is_regular_file()) {
                    std::string ext = entry.path().extension().string();
                    if (!ext.empty()) {
                        for (const auto& valid_ext : extensions_) {
                            if (ext == valid_ext) { // Прямое сравнение, так как все расширения уже с точкой
                                fs::path relative_path = fs::relative(entry.path(), root_path_);
                                found_files.push_back(relative_path.string());
                                break;
                            }
                        }
                    }
                }
            }
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << '\n';
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }

        return found_files;
    }

    std::vector<std::string> read_file(const std::string& filename) {
    std::vector<std::string> output;

    try {
        std::ifstream file(filename, std::ios::in);
        if (!file.is_open()) {
            throw std::runtime_error("Error opening file: " + filename);
        }

        std::string line;
        while (std::getline(file, line)) {
            output.push_back(line);
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << '\n';
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    catch (...) {
        std::cerr << "Unknown error occurred while reading file: " << filename << '\n';
    }

    return output;
    }

    void save_file(const std::string& filename, const std::vector<std::string>& lines) {
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

private:
    std::string root_path_;
    std::string output_path_;
    std::vector<std::string> extensions_;
};
#include "class.hpp"

CDOCS_files::CDOCS_files(const std::string& doc_root_path, 
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
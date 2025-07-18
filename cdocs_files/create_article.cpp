#include "class.hpp"

void CDOCS_files::create_article(std::filesystem::path& filename) {
    try {
        if (std::filesystem::exists(filename)) {
            throw std::runtime_error("Article already exists: " + filename.string());
        }
        std::ofstream file(filename); // Create and open the file for writing
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename.string());
        }
        file << "This is some text in the new file." << std::endl; // Записываем в файл
        file.close(); // Закрываем файл
        std::cout << "Success create empty article from template." << std::endl;
    }
    catch (const std::filesystem::filesystem_error& ex) {
        std::cout << "what():  " << ex.what() << '\n';
    }
    catch (const std::runtime_error& ex) {
        std::cout << "Error: " << ex.what() << '\n';
    }
}

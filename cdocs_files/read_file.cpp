#include "class.hpp"

std::vector<std::string> CDOCS_files::read_file(const std::string& filename) {
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
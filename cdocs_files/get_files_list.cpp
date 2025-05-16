#include "class.hpp"

std::vector<std::string> CDOCS_files::get_files_list() {
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
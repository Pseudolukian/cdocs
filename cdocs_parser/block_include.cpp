#include "class.hpp"

vector<string> CDOCS_parser::block_include(vector<string> blocks, const string& file_name, int depth, std::regex& block_include_, std::regex& block_include_no_title) {
    // Защита от циклических инклудов
    constexpr int MAX_INCLUDE_DEPTH = 10;
    if (depth > MAX_INCLUDE_DEPTH) {
        throw std::runtime_error("Maximum include depth (" + 
                               std::to_string(MAX_INCLUDE_DEPTH) + 
                               ") exceeded");
    }

    // 1. Находим все @include директивы
    vector<string> out;
    smatch matches;

    struct FileData {
        fs::path path;
        std::string anchor;
        bool title;
    };

    std::map<size_t, FileData> includes_map;

    // Собираем точки включения и пути
    for (size_t i = 0; i < blocks.size(); ++i) {
        const string& block = blocks[i];
        if (block.size() > 15 && block.size() < 100) {
            if (regex_search(block, matches, block_include_no_title)) {
                auto result = CDOCS_parser::resolve_include_path(matches[1].str(), file_name);
                includes_map[i] = FileData{result.first, result.second, false};
            }
            else if (regex_search(block, matches, block_include_)) {
                auto result = CDOCS_parser::resolve_include_path(matches[1].str(), file_name);
                includes_map[i] = FileData{result.first, result.second, true};
            }
        }
    }

    // Собираем итоговый контент с подстановкой включений
    size_t last_pos = 0;
    for (const auto& [pos, file_data] : includes_map) {
        // Добавляем блоки до текущего include
        for (size_t i = last_pos; i < pos; ++i) {
            out.push_back(blocks[i]);
        }
        
        // Добавляем содержимое включаемого файла
        try {
            auto included_content = CDOCS_files::read_file_ext(
                file_data.path.string(), 
                file_data.anchor, 
                file_data.title
            );
            
            // Рекурсивная обработка вложенных include
            auto processed_content = block_include(included_content, file_data.path.string(), depth + 1, block_include_, block_include_no_title);
            out.insert(out.end(), processed_content.begin(), processed_content.end());
        } catch (const std::exception& e) {
            throw std::runtime_error("Error processing include at position " + 
                                   std::to_string(pos) + ": " + e.what());
        }
        
        last_pos = pos + 1; // Пропускаем сам блок include
    }
    
    // Добавляем оставшиеся блоки после последнего include
    for (size_t i = last_pos; i < blocks.size(); ++i) {
        out.push_back(blocks[i]);
    }

    return out;
}
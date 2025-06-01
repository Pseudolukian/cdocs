#include "class.hpp"

vector<string> CDOCS_parser::block_include(vector<string> blocks, const string& file_name, int depth, 
                                          std::regex& block_include_, std::regex& block_include_no_title, 
                                          std::unordered_map<string, std::vector<std::string>>& buffer_includes) {
    // Защита от циклических инклудов
    constexpr int MAX_INCLUDE_DEPTH = 10;
    if (depth > MAX_INCLUDE_DEPTH) {
        throw std::runtime_error("Maximum include depth (" + 
                               std::to_string(MAX_INCLUDE_DEPTH) + 
                               ") exceeded");
    }

    vector<string> out;
    smatch matches;

    struct FileData {
        fs::path path;
        std::string anchor;
        bool title;
        std::string cache_key;
        size_t line_number;  // Для отладочной информации
    };

    std::map<size_t, FileData> includes_map;

    // Собираем точки включения и пути
    for (size_t i = 0; i < blocks.size(); ++i) {
        const string& block = blocks[i];
        if (block.size() > 15 && block.size() < 100) {
            if (regex_search(block, matches, block_include_no_title)) {
                auto result = CDOCS_parser::resolve_include_path(matches[1].str(), file_name);
                std::string cache_key = result.first.string() + "#" + result.second;
                includes_map[i] = FileData{result.first, result.second, false, cache_key, i};
                
            }
            else if (regex_search(block, matches, block_include_)) {
                auto result = CDOCS_parser::resolve_include_path(matches[1].str(), file_name);
                std::string cache_key = result.first.string() + "#" + result.second;
                includes_map[i] = FileData{result.first, result.second, true, cache_key, i};
                
            }
        }
    }

    size_t last_pos = 0;
    for (const auto& [pos, file_data] : includes_map) {
        // Добавляем блоки до текущего include
        for (size_t i = last_pos; i < pos; ++i) {
            out.push_back(blocks[i]);
        }
        
        std::vector<std::string> included_content;
        auto it = buffer_includes.find(file_data.cache_key);
        
        if (it != buffer_includes.end()) {
            // Контент найден в кеше
            included_content = it->second;
                      
        } else {
            try {
                // Читаем файл
                
                included_content = CDOCS_files::read_file_ext(
                    file_data.path.string(), 
                    file_data.anchor, 
                    file_data.title
                );
                
                // Сохраняем в буфер
                buffer_includes[file_data.cache_key] = included_content;
            } catch (const std::exception& e) {
                throw std::runtime_error("Error processing include at position " + 
                                       std::to_string(pos) + ": " + e.what());
            }
        }
        
        // Рекурсивная обработка
        auto processed_content = block_include(included_content, file_data.path.string(), 
                                             depth + 1, block_include_, block_include_no_title,
                                             buffer_includes);
        
        out.insert(out.end(), processed_content.begin(), processed_content.end());
        
        last_pos = pos + 1;
    }
    
    // Добавляем оставшиеся блоки
    for (size_t i = last_pos; i < blocks.size(); ++i) {
        out.push_back(blocks[i]);
    }

    return out;
}
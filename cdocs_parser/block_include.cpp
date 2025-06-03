#include "class.hpp"

std::vector<std::string> CDOCS_parser::block_include(const std::vector<std::string>& blocks, 
                               const std::string& file_name, 
                               const std::regex& include_regex,
                               const std::unordered_map<string, std::vector<std::string>>& buffer_content,
                               std::unordered_map<string, std::vector<std::string>>& buffer_include) 
{
    constexpr size_t MAX_INCLUDE_LENGTH = 80;
    
    std::vector<std::string> out;
    std::map<int, std::vector<string>> content_to_include;
    
    // Первый проход: собираем информацию о включениях
    for (size_t i = 0; i < blocks.size(); ++i) {
        const std::string& block = blocks[i];
        
        if (block.empty() || block[0] != '@' || block.length() > MAX_INCLUDE_LENGTH) {
            continue;
        }
        
        std::smatch matches;
        if (std::regex_search(block, matches, include_regex)) {
            const std::string& rel_path = matches[1].str();
            const bool notitle = matches[2].matched;
            const std::string anchor = matches[3].matched ? matches[3].str() : "none";

            fs::path current_file_path = file_name;
            fs::path include_path = rel_path;
            fs::path full_path = current_file_path.parent_path() / include_path;
            full_path = full_path.lexically_normal();

            // Проверяем буфер включений
            auto include_it = buffer_include.find(full_path);
            if (include_it != buffer_include.end()) {
                content_to_include[i] = include_it->second;
                continue;
            }

            // Проверяем основной буфер
            auto content_it = buffer_content.find(full_path);
            if (content_it != buffer_content.end()) {
                content_to_include[i] = content_it->second;
                buffer_include[full_path] = content_it->second;
                continue;
            }

            std::cerr << "Error: Could not find content for included file: " << full_path << std::endl;
            // Если файл не найден, оставляем оригинальный блок
            continue;
        }
    }

    // Второй проход: собираем итоговый вектор
    out.reserve(blocks.size()); // Минимальный резерв
    
    for (size_t i = 0; i < blocks.size(); ++i) {
        auto it = content_to_include.find(i);
        if (it != content_to_include.end()) {
            // Вставляем включенный контент вместо текущего блока
            out.insert(out.end(), it->second.begin(), it->second.end());
        } else {
            // Просто копируем оригинальный блок
            out.push_back(blocks[i]);
        }
    }

    return out;
}
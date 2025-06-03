#include "class.hpp"

void CDOCS_parser::block_include(const std::vector<std::string>& blocks, 
                               const std::string& file_name, 
                               const std::regex& include_regex,
                               const std::unordered_map<string, std::vector<std::string>>& buffer_content,
                               std::unordered_map<string, std::vector<std::string>>& buffer_include) 
{
    constexpr size_t MAX_INCLUDE_LENGTH = 80;

    std::vector<std::string> include_content;
    
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

            // Создаём fs::path для текущего файла и включаемого файла
            fs::path current_file_path = file_name;
            fs::path include_path = rel_path;

            // Объединяем пути корректно
            fs::path full_path = current_file_path.parent_path() / include_path;
            full_path = full_path.lexically_normal();

            // Проверяем сначала буфер включений
            auto include_it = buffer_include.find(full_path);
            if (include_it != buffer_include.end()) {
                include_content = include_it->second;
                std::cout << "Take content from include buffer." << std::endl;
                continue;
            }

            // Если нет в буфере включений, проверяем основной буфер
            auto content_it = buffer_content.find(full_path);
            if (content_it != buffer_content.end()) {
                include_content = content_it->second;
                buffer_include[full_path] = include_content;
                std::cout << "Take content from global buffer and add to include buffer." << std::endl;
                continue;
            }

            // Если файл не найден ни в одном из буферов
            std::cerr << "Error: Could not find content for included file: " << full_path << std::endl;
            continue;
        }
    }
}
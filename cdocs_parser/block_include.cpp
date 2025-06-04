#include "class.hpp"

std::vector<std::string> CDOCS_parser::block_include(const std::vector<std::string>& blocks, 
                               const std::string& file_name, 
                               const std::regex& include_regex,
                               const std::regex& header_regex,
                               const std::unordered_map<std::string, std::vector<std::string>>& buffer_content,
                               std::unordered_map<std::string, std::vector<std::string>>& buffer_include,
                               int depth) 
{
    constexpr size_t MAX_INCLUDE_LENGTH = 80;
    constexpr int MAX_DEPTH = 5;
    
    if (depth > MAX_DEPTH) {
        std::cerr << "Error: Maximum include depth (" << MAX_DEPTH << ") exceeded in file: " << file_name << std::endl;
        return blocks;
    }
    
    std::map<int, std::vector<std::string>> content_to_include;
    
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

            // Вывод отладочной информации
            std::cerr << "File: " << file_name << std::endl;
            std::cerr << "Find @include: " << block << std::endl;
            std::cerr << "Path for include: " << rel_path << std::endl;
            std::cerr << "Anchor: " << (anchor != "none" ? "yes" : "no") << std::endl;
            std::cerr << "Notitle: " << (notitle ? "yes" : "no") << std::endl;

            fs::path current_file_path = file_name;
            fs::path include_path = rel_path;
            fs::path full_path = current_file_path.parent_path() / include_path;
            full_path = full_path.lexically_normal();

            // Проверяем буфер включений
            auto include_it = buffer_include.find(full_path.string());
            if (include_it != buffer_include.end()) {
                std::cerr << "Find in include_buffer: yes" << std::endl;
                // Рекурсивно обрабатываем вложенные include
                std::vector<std::string> processed_content = CDOCS_parser::block_include(include_it->second, full_path.string(), 
                                                                        include_regex, header_regex, 
                                                                        buffer_content, buffer_include, depth + 1);
                // Применяем обработку anchor и notitle
                if (!anchor.empty() && anchor != "none") {
                    processed_content = CDOCS_parser::anchor(processed_content, anchor, header_regex);
                }
                if (notitle) {
                    processed_content = CDOCS_parser::notitle(processed_content, header_regex);
                }
                content_to_include[i] = std::move(processed_content);
                continue;
            } else {
                std::cerr << "Find in include_buffer: no" << std::endl;
            }

            // Проверяем основной буфер
            auto content_it = buffer_content.find(full_path.string());
            if (content_it != buffer_content.end()) {
                std::cerr << "Find in main buffer: yes" << std::endl;
                // Рекурсивно обрабатываем вложенные include
                std::vector<std::string> processed_content = block_include(content_it->second, full_path.string(), 
                                                                        include_regex, header_regex, 
                                                                        buffer_content, buffer_include, depth + 1);
                // Применяем обработку anchor и notitle
                if (!anchor.empty() && anchor != "none") {
                    processed_content = CDOCS_parser::anchor(processed_content, anchor, header_regex);
                }
                if (notitle) {
                    processed_content = CDOCS_parser::notitle(processed_content, header_regex);
                }
                content_to_include[i] = std::move(processed_content);
                buffer_include[full_path.string()] = content_to_include[i];
                continue;
            } else {
                std::cerr << "Find in main buffer: no" << std::endl;
            }

            std::cerr << "Error: Could not find content for included file: " << full_path << std::endl;
            continue;
        }
    }

    // Вычисляем итоговый размер выходного вектора
    size_t total_size = blocks.size();
    for (const auto& [_, content] : content_to_include) {
        total_size += content.size() - 1; // Вычитаем 1, так как заменяем один блок
    }
    
    // Резервируем память для выходного вектора
    std::vector<std::string> out;
    out.reserve(total_size);
    
    // Второй проход: формируем выходной вектор
    for (size_t i = 0; i < blocks.size(); ++i) {
        auto it = content_to_include.find(i);
        if (it != content_to_include.end()) {
            // Вставляем содержимое включенного файла
            out.insert(out.end(), 
                      std::make_move_iterator(it->second.begin()),
                      std::make_move_iterator(it->second.end()));
        } else {
            // Копируем оригинальный блок, если нет включения
            out.emplace_back(blocks[i]);
        }
    }

    return out;
}
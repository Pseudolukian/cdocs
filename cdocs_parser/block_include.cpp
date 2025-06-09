#include "class.hpp"

std::vector<std::string> CDOCS_parser::block_include(const std::vector<std::string>& blocks, 
                                const std::string& file_name, 
                                const std::regex& include_regex,
                                const std::regex& header_regex,
                                const std::unordered_map<std::string, std::vector<std::string>>& buffer_content,
                                std::unordered_map<std::string, std::vector<std::string>>& buffer_include,
                                int depth,
                                bool log_save, 
                                const std::string log_path) 
{
    vector<std::unique_ptr<ILogModel>> log_data;
    constexpr size_t MAX_INCLUDE_LENGTH = 80;
    constexpr int MAX_DEPTH = 8;
    
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

            fs::path current_file_path = file_name;
            fs::path include_path = rel_path;
            fs::path full_path = current_file_path.parent_path() / include_path;
            full_path = full_path.lexically_normal();

            // Проверяем буфер включений
            auto include_it = buffer_include.find(full_path.string());
            if (include_it != buffer_include.end()) {
                std::vector<std::string> processed_content = CDOCS_parser::block_include(include_it->second, full_path.string(), 
                                                                        include_regex, header_regex, 
                                                                        buffer_content, buffer_include, depth + 1);
                if (!anchor.empty() && anchor != "none") {
                    processed_content = CDOCS_parser::anchor(processed_content, anchor);
                }
                if (notitle) {
                    processed_content = CDOCS_parser::notitle(processed_content);
                }
                content_to_include[i] = std::move(processed_content);
                
                if (log_save) {
                    auto log_entry = make_unique<LogInclude>();
                    log_entry->File = file_name;
                    log_entry->Include_block = block;
                    log_entry->Notitle = notitle ? "true" : "false";
                    log_entry->Anchor = anchor;
                    
                    std::string content_str;
                    for (const auto& line : content_to_include[i]) {
                        content_str += line + "\n";
                    }
                    log_entry->Include_content = content_str;
                    log_data.push_back(move(log_entry));
                }
                continue;
            }

            // Проверяем основной буфер
            auto content_it = buffer_content.find(full_path.string());
            if (content_it != buffer_content.end()) {
                std::vector<std::string> processed_content = block_include(content_it->second, full_path.string(), 
                                                                        include_regex, header_regex, 
                                                                        buffer_content, buffer_include, depth + 1);
                if (!anchor.empty() && anchor != "none") {
                    processed_content = CDOCS_parser::anchor(processed_content, anchor);
                }
                if (notitle) {
                    processed_content = CDOCS_parser::notitle(processed_content);
                }
                content_to_include[i] = std::move(processed_content);
                buffer_include[full_path.string()] = content_to_include[i];
                
                if (log_save) {
                    auto log_entry = make_unique<LogInclude>();
                    log_entry->File = file_name;
                    log_entry->Include_block = block;
                    log_entry->Notitle = notitle ? "true" : "false";
                    log_entry->Anchor = anchor;
                    
                    std::string content_str;
                    for (const auto& line : content_to_include[i]) {
                        content_str += line + "\n";
                    }
                    log_entry->Include_content = content_str;
                    log_data.push_back(move(log_entry));
                }
                continue;
            }
            std::cerr << "Error: Could not find content for included file: " << full_path << std::endl;
        }
    }

    // Формируем выходной вектор
    std::vector<std::string> out;
    for (size_t i = 0; i < blocks.size(); ++i) {
        auto it = content_to_include.find(i);
        if (it != content_to_include.end()) {
            out.insert(out.end(), it->second.begin(), it->second.end());
        } else {
            out.push_back(blocks[i]);
        }
    }

    if (log_save && !log_data.empty()) {
        CDOCS_log::log("Includes", log_data, log_path);
    }

    return out;
}
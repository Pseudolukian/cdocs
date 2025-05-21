#include "class.hpp"

using namespace std;
using Value = variant<string, unsigned, double>;


vector<string> CDOCS_parser::block_include(vector<string>& blocks, const map<string, Value>& vars_list, string file_name, int depth) {
    const int MAX_RECURSION_DEPTH = 10;
    if (depth > MAX_RECURSION_DEPTH) {
        cerr << "Maximum include recursion depth reached (" << MAX_RECURSION_DEPTH << ")" << endl;
        return blocks;
    }

    // Регулярные выражения
    regex include_regex(R"(\s*@include\s*(notitle\s*)?\(\s*([^)#]+)(?:#([^)]+))?\s*\))");
    regex header_regex(R"(^\s*#{1,3}\s+.+\{\s*#\s*([^}]+)\s*\})");
    regex empty_line_regex(R"(^\s*$)");
    
    vector<string> result_blocks;
    
    for (const auto& block : blocks) {
        smatch match;
        if (regex_search(block, match, include_regex) && match.size() > 2) {
            bool no_title = match[1].matched;
            string path = match[2].str();
            string anchor = match.size() > 3 ? match[3].str() : "";
            
            // Удаляем пробелы
            path.erase(0, path.find_first_not_of(" \t"));
            path.erase(path.find_last_not_of(" \t") + 1);
            if (!anchor.empty()) {
                anchor.erase(0, anchor.find_first_not_of(" \t"));
                anchor.erase(anchor.find_last_not_of(" \t") + 1);
            }
            
            string final_path = resolve_include_path(path, file_name);
            
            try {
                vector<string> included_blocks = CDOCS_files::read_file(final_path);
                vector<string> filtered_blocks;
                
                if (no_title) {
                    bool found_anchor = anchor.empty();
                    bool content_started = false;
                    
                    for (const auto& line : included_blocks) {
                        smatch header_match;
                        if (regex_search(line, header_match, header_regex)) {
                            string current_anchor = header_match[1].str();
                            current_anchor.erase(0, current_anchor.find_first_not_of(" \t"));
                            current_anchor.erase(current_anchor.find_last_not_of(" \t") + 1);
                            
                            if (found_anchor) {
                                // Если нашли следующий заголовок после нужного якоря
                                break;
                            }
                            
                            if (anchor.empty() || current_anchor == anchor) {
                                found_anchor = true;
                                continue; // Пропускаем заголовок
                            }
                        } else if (found_anchor) {
                            // Пропускаем пустые строки в начале
                            if (!content_started && regex_search(line, empty_line_regex)) {
                                continue;
                            }
                            content_started = true;
                            filtered_blocks.push_back(line);
                        }
                    }
                } else {
                    filtered_blocks = included_blocks;
                }
                
                // Рекурсивная обработка
                filtered_blocks = block_include(filtered_blocks, vars_list, final_path, depth + 1);
                result_blocks.insert(result_blocks.end(), filtered_blocks.begin(), filtered_blocks.end());
                
            } catch (const exception& e) {
                cerr << "Error processing include '" << path << (anchor.empty() ? "" : "#" + anchor) 
                     << "': " << e.what() << endl;
                result_blocks.push_back(block);
            }
            
            continue;
        }
        
        result_blocks.push_back(block);
    }
    
    return result_blocks;
}

string CDOCS_parser::resolve_include_path(const string& path, const string& base_file) {
    string final_include_path;
    
    // Получаем директорию исходного файла
    size_t last_slash = base_file.find_last_of('/');
    string file_dir = (last_slash != string::npos) ? 
        base_file.substr(0, last_slash + 1) : "./";
    
    if (path.find("./") == 0) {
        // Относительный путь с ./
        final_include_path = file_dir + path.substr(2);
    } 
    else if (path.find("../") == 0) {
        // Относительный путь с ../
        string current_dir = file_dir;
        string rel_path = path;
        
        while (rel_path.find("../") == 0) {
            // Поднимаемся на один уровень вверх для каждого ../
            size_t dir_slash = current_dir.find_last_of('/', current_dir.length() - 2);
            if (dir_slash != string::npos) {
                current_dir = current_dir.substr(0, dir_slash + 1);
            }
            rel_path = rel_path.substr(3);
        }
        final_include_path = current_dir + rel_path;
    } 
    else if (path.find('/') == 0) {
        // Абсолютный путь (начинается с /)
        final_include_path = path;
    } 
    else {
        // Относительный путь без ./ или ../
        final_include_path = file_dir + path;
    }
    
    return final_include_path;
}
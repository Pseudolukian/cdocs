#include "class.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <variant>
#include <regex>
#include <vector>
#include <map>
#include <algorithm>
#include "../cdocs_files/class.hpp"

using namespace std;
using Value = variant<string, unsigned, double>;


vector<string> CDOCS_parser::block_include(vector<string>& blocks, const map<string, Value>& vars_list, string file_name, int depth) {
    // Защита от слишком глубокой рекурсии
    const int MAX_RECURSION_DEPTH = 10;
    if (depth > MAX_RECURSION_DEPTH) {
        cerr << "Maximum include recursion depth reached (" << MAX_RECURSION_DEPTH << ")" << endl;
        return blocks;
    }

    // Регулярные выражения для поиска директив
    regex include_pattern(R"(\s*@include\s*)");
    regex include_body_pattern(R"(\s*\((?:\.{0,2}/)?[^)]+\.md\))");
    regex block_if_pattern(R"(\s*@if\s*)");
    
    vector<string> result_blocks;
    
    for (const auto& block : blocks) {
        // Проверка на @include
        if (regex_search(block, include_pattern)) {
            smatch match;
            if (regex_search(block, match, include_body_pattern)) {
                // Извлекаем путь из скобок
                string path = match.str();
                path = path.substr(path.find('(') + 1);
                path = path.substr(0, path.find(')'));
                
                // Удаляем возможные пробелы
                path.erase(0, path.find_first_not_of(" \t"));
                path.erase(path.find_last_not_of(" \t") + 1);
                
                // Формируем абсолютный путь
                string final_include_path = resolve_include_path(path, file_name);
                
                // Читаем содержимое файла
                vector<string> included_blocks = CDOCS_files::read_file(final_include_path);
                
                // Рекурсивно обрабатываем вложенные инклюды
                included_blocks = block_include(included_blocks, vars_list, final_include_path, depth + 1);
                
                // Добавляем обработанное содержимое в результат
                result_blocks.insert(result_blocks.end(), included_blocks.begin(), included_blocks.end());
                
                continue; // Пропускаем добавление оригинального блока с @include
            }
        }
        
        // Добавляем оригинальный блок, если это не @include
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
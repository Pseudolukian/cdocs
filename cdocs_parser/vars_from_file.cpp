#include "class.hpp"

map<string, map<string, Value>> CDOCS_parser::vars_from_file(std::vector<std::string>& vars_list, std::regex& md_vars_group) {
    map<string, map<string, Value>> result;
    string current_group = "main"; // По умолчанию группа main

    std::smatch match;

    for (string line : vars_list) {
        // Удаляем только начальные и конечные пробелы
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        if (line.empty()) continue;

        // Проверяем, является ли строка объявлением группы
        if (std::regex_search(line, match, md_vars_group)) {
            current_group = match[1].str(); // Извлекаем название группы
            continue; // Переходим к следующей строке
        }

        // Парсим переменные только если это не пустая строка и не объявление группы
        size_t eq_pos = line.find('=');
        if (eq_pos != string::npos && eq_pos > 0) {
            // Ключ - все до знака = (удаляем пробелы только вокруг ключа)
            string key = line.substr(0, eq_pos);
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            
            // Значение - все после знака = (сохраняем оригинальные пробелы, кроме окружающих)
            string value = line.substr(eq_pos + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            if (!key.empty()) {
                result[current_group][key] = Value(value);
            }
        }
    }

    return result;
}
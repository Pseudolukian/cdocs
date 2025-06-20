#include "class.hpp"

using namespace std;

vector<string> CDOCS_parser::vars_in_docs(const vector<string>& file_lines, 
                                        const std::map<std::string, std::map<std::string, Value>>& vars_list,
                                        const std::string& file_name,
                                        bool log_save, 
                                        const std::string log_path) {
    vector<string> result;
    map<string, string> cache;
    string var_for_log;
    vector<unique_ptr<ILogModel>> log_data;
    const std::unordered_set<char> ignor = {',', '{', '}', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                      '$', '%', '&', '*', '(', ')', '<', '>'};


    for (const string& line : file_lines) {
        
        if (line.find("{{") == string::npos) {
            result.push_back(line); // Пропускаем строки без {{
            continue;
        }

        string processed_line = line;
        vector<pair<size_t, size_t>> positions; // Позиции и длины для замен
        vector<string> replacements; // Значения для замен

        size_t pos = 0;
        while ((pos = processed_line.find("{{", pos)) != string::npos) {
            size_t start = pos;
            size_t end = processed_line.find("}}", pos);
            if (end == string::npos) break; // Нет закрывающей }}

            // Извлекаем содержимое между {{ и }}
            string full_var_path = processed_line.substr(pos + 2, end - pos - 2);
            // Удаляем пробелы в начале и конце
            size_t first_non_space = full_var_path.find_first_not_of(" \t");
            size_t last_non_space = full_var_path.find_last_not_of(" \t");
            if (first_non_space != string::npos && last_non_space != string::npos) {
                full_var_path = full_var_path.substr(first_non_space, last_non_space - first_non_space + 1);
            } else {
                full_var_path.clear(); // Если строка состоит только из пробелов
            }

            // Проверяем наличие ignor-символов
            bool should_ignore = false;
            for (char c : full_var_path) {
                if (ignor.count(c)) {
                    should_ignore = true;
                    break;
                }
            }
            if (should_ignore) {
                pos = end + 2;
                continue;
            }

            // Проверяем кэш
            auto cache_it = cache.find(full_var_path);
            if (cache_it != cache.end()) {
                positions.emplace_back(start, end - start + 2);
                replacements.push_back(cache_it->second);
                pos = end + 2;
                continue;
            }

            // Разбиваем full_var_path на group_name и var_key
            string group_name = "main";
            string var_key = full_var_path;
            size_t dot_pos = full_var_path.find('.');
            if (dot_pos != string::npos) {
                group_name = full_var_path.substr(0, dot_pos);
                var_key = full_var_path.substr(dot_pos + 1);
            }

            // Ищем значение в vars_list
            string value;
            auto group_it = vars_list.find(group_name);
            if (group_it != vars_list.end()) {
                auto var_it = group_it->second.find(var_key);
                if (var_it != group_it->second.end()) {
                    // Конвертируем Value в string
                    value = visit([](const auto& v) -> string {
                        if constexpr (is_same_v<decay_t<decltype(v)>, string>) {
                            return v;
                        } else {
                            return to_string(v);
                        }
                    }, var_it->second);
                }
            }

            // Сохраняем в кэш и добавляем для замены
            cache[full_var_path] = value;
            positions.emplace_back(start, end - start + 2);
            replacements.push_back(value);
            pos = end + 2;

            var_for_log = full_var_path;
        }

        // Применяем замены в обратном порядке
        for (int i = replacements.size() - 1; i >= 0; --i) {
            const auto& pos = positions[i];
            if (!replacements[i].empty()) { // Заменяем только если значение найдено
                processed_line.replace(pos.first, pos.second, replacements[i]);
            }
        }

        result.push_back(processed_line);
    
    if (log_save) {
        if (!replacements.empty()) {
            auto log_entry = make_unique<LogVar>();
            log_entry->File = file_name;
            log_entry->Var = var_for_log;
            log_entry->Line_before_swap = line;
            log_entry->Line_after_swap = processed_line;
            log_entry->Value = replacements.back();
            log_data.push_back(move(log_entry));
        }
        CDOCS_log::log("Vars", log_data, log_path);
    }
    
    }
    return result;
}
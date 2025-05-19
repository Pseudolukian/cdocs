#include "class.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <variant>
#include <regex>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;
using Value = variant<string, unsigned, double>;

struct CompareVisitor {
    bool operator()(const std::string& a, const std::string& b) const { return a == b; }
    bool operator()(unsigned a, unsigned b) const { return a == b; }
    bool operator()(double a, double b) const { return a == b; }
    template<typename T, typename U>
    bool operator()(const T&, const U&) const { return false; } // Разные типы не равны
};



vector<string> CDOCS_parser::structure_parser(vector<string>& file_lines, map<string, Value>& vars_list, string file_name) {
    vector<string> output;
    int line_count = 0;
    vector<size_t> lines_to_remove; // Собираем индексы строк для удаления
    vector<pair<int, int>> if_blocks; // Храним пары (start, end) для блоков @if-@endif

    for (size_t i = 0; i < file_lines.size(); ++i) {
        string& line = file_lines[i];
        string processed_line = line;
        line_count++;

        if (processed_line.find("@if") != string::npos) {
            regex condition_regex(R"(\{\{\s*(\w+)\s*\}\}\s*(==|!=)\s*([^&)]+))");
            int block_if_start = line_count;
            map<string, pair<string, Value>> conditions_map; // Храним оператор и значение
            sregex_iterator it(line.begin(), line.end(), condition_regex);
            sregex_iterator end;

            for (; it != end; ++it) {
                smatch match = *it;
                string var_name = match[1].str();
                string op = match[2].str();
                string value_str = match[3].str();

                // Удаляем возможные пробелы вокруг значения
                value_str.erase(value_str.begin(),
                                find_if(value_str.begin(), value_str.end(),
                                        [](int ch) { return !isspace(ch); }));
                value_str.erase(find_if(value_str.rbegin(), value_str.rend(),
                                        [](int ch) { return !isspace(ch); }).base(),
                                value_str.end());

                // Пытаемся преобразовать value_str в подходящий тип
                Value value;
                try {
                    if (value_str.find('.') != string::npos) {
                        value = stod(value_str); // double
                    } else {
                        value = static_cast<unsigned>(stoul(value_str)); // unsigned
                    }
                } catch (...) {
                    value = value_str; // По умолчанию строка
                }

                conditions_map[var_name] = {op, value};
            }

            // Сравниваем переменные из conditions_map с vars_list
            bool condition_met = true;
            for (const auto& [var_name, op_value] : conditions_map) {
                const auto& [op, cond_value] = op_value;
                auto it = vars_list.find(var_name);
                if (it == vars_list.end()) {
                    condition_met = false; // Переменная не найдена
                    break;
                }

                const Value& list_value = it->second;
                bool equal = std::visit(CompareVisitor{}, cond_value, list_value);

                if (op == "==" && !equal) {
                    condition_met = false;
                    break;
                }
                if (op == "!=" && equal) {
                    condition_met = false;
                    break;
                }
            }

            // Если условие выполнено, удаляем только строку @if
            if (condition_met) {
                lines_to_remove.push_back(i);
            } else {
                // Если условие не выполнено, сохраняем начало блока для удаления до @endif
                if_blocks.emplace_back(line_count, -1); // -1 пока не найден @endif
            }
        }

        if (line.find("@endif") != string::npos) {
            int block_if_end = line_count;
            // Находим последний незакрытый блок @if
            for (auto& block : if_blocks) {
                if (block.second == -1) { // Незакрытый блок
                    block.second = block_if_end;
                    break;
                }
            }
        }

        output.push_back(processed_line);
    }

    // Обрабатываем блоки @if-@endif для удаления
    for (const auto& [start, end] : if_blocks) {
        if (end != -1) { // Блок закрыт
            for (size_t j = 0; j < file_lines.size(); ++j) {
                int current_line = j + 1; // Нумерация строк начинается с 1
                if (current_line >= start && current_line <= end) {
                    lines_to_remove.push_back(j);
                }
            }
        }
    }

    // Удаляем строки в обратном порядке, чтобы избежать смещения индексов
    sort(lines_to_remove.begin(), lines_to_remove.end(), greater<size_t>());
    for (size_t idx : lines_to_remove) {
        if (idx < file_lines.size()) {
            file_lines.erase(file_lines.begin() + idx);
            output.erase(output.begin() + idx);
        }
    }

    return output;
}
//inline_if.cpp
#include "class.hpp"

using namespace std;


vector<string> CDOCS_parser::inline_if(const vector<string>& file_lines) {
    vector<string> result_lines;
    const string start_delim = "{% if ";
    const string end_delim = " %}";
    const string endif_delim = "{% endif %}";

    for (size_t line_idx = 0; line_idx < file_lines.size(); ++line_idx) {
        string line = file_lines[line_idx];
        string new_line;
        size_t pos = 0;

        // Ищем все вхождения {% if ... %} в текущей строке
        while (pos < line.length()) {
            // Находим начало {% if
            size_t start_pos = line.find(start_delim, pos);
            if (start_pos == string::npos) {
                // Нет больше условий, добавляем остаток строки
                new_line += line.substr(pos);
                break;
            }

            // Добавляем текст до {% if
            new_line += line.substr(pos, start_pos - pos);

            // Находим конец %}
            size_t content_start = start_pos + start_delim.length();
            size_t end_pos = line.find(end_delim, content_start);
            if (end_pos == string::npos) {
                cout << "Warning: Invalid format, missing %} in line " << line_idx + 1 << endl;
                new_line += line.substr(start_pos); // Добавляем остаток как есть
                break;
            }

            // Извлекаем содержимое условия
            string if_body = line.substr(content_start, end_pos - content_start);

            // Находим {% endif %}
            size_t endif_pos = line.find(endif_delim, end_pos + end_delim.length());
            if (endif_pos == string::npos) {
                cout << "Warning: Missing {% endif %} in line " << line_idx + 1 << endl;
                new_line += line.substr(start_pos); // Добавляем остаток как есть
                break;
            }

            // Извлекаем содержимое блока
            size_t block_start = end_pos + end_delim.length();
            string block_content = line.substr(block_start, endif_pos - block_start);

            // Проверяем условие
            if (if_cond_parser(if_body)) {
                // Если условие истинно, добавляем содержимое блока
                new_line += block_content;
            } else {
                // Если условие ложно, ничего не добавляем (блок удаляется)
            }

            // Продолжаем поиск после {% endif %}
            pos = endif_pos + endif_delim.length();
        }

        result_lines.push_back(new_line);
    }

    return result_lines;
}
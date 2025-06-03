#include "class.hpp"

std::vector<std::string> CDOCS_parser::block_if(const std::vector<std::string>& lines, std::regex& block_if_regex_start, std::regex& block_if_regex_end) {
    std::smatch match;
    std::vector<int> break_points;
    std::vector<bool> condition_stack = {true}; // Стек для условий

    int line_pos = 0;
    for (const std::string& line : lines) {
        bool current_condition = condition_stack.back();

        if (!current_condition) {
            break_points.push_back(line_pos);
        }

        if (line.size() > 10 && line.size() < 50) {
            if (std::regex_search(line, match, block_if_regex_start)) {
                std::string condition = match[1].str();
                bool condition_met = CDOCS_parser::if_cond_parser(condition);
                condition_stack.push_back(current_condition && condition_met);
                break_points.push_back(line_pos); // Удаляем строку с @if
            }
        } else if (line.size() < 10) {
            if (std::regex_search(line, block_if_regex_end)) {
                condition_stack.pop_back(); // Завершаем текущий блок
                break_points.push_back(line_pos); // Удаляем строку с @endif
            }
        }

        line_pos++;
    }

    // Преобразуем break_points в set для быстрого поиска
    std::set<int> break_points_set(break_points.begin(), break_points.end());
    std::vector<std::string> result;

    // Проходим по индексам lines и добавляем только те строки, которых нет в break_points
    for (size_t i = 0; i < lines.size(); ++i) {
        if (break_points_set.find(i) == break_points_set.end()) {
            result.push_back(std::move(lines[i]));
        }
    }

    return result;
}
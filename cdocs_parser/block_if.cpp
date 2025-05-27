#include "class.hpp"

std::vector<std::string> CDOCS_parser::block_if(std::vector<std::string>& lines) {
    std::vector<std::string> result;
    std::regex if_regex(R"(@if\s*\(\s*([^)]+)\s*\))");
    std::regex endif_regex(R"(@endif)");
    std::smatch match;

    bool inside_if_block = false;
    bool condition_met = false;
    std::vector<std::string> temp_block; // Временное хранилище для контента внутри @if ... @endif

    for (const auto& line : lines) {
        if (std::regex_search(line, match, if_regex)) {
            if (inside_if_block) {
                // Вложенные @if не поддерживаются (можно добавить стек для поддержки)
                std::cerr << "Error: Nested @if blocks are not supported!" << std::endl;
                return lines; // Возвращаем исходный текст в случае ошибки
            }

            inside_if_block = true;
            std::string condition = match[1].str();
            condition_met = CDOCS_parser::if_cond_parser(condition);

            // Пропускаем строку с @if (не добавляем в результат)
            continue;
        }
        else if (std::regex_search(line, endif_regex)) {
            if (!inside_if_block) {
                std::cerr << "Error: @endif without matching @if!" << std::endl;
                return lines; // Возвращаем исходный текст в случае ошибки
            }

            inside_if_block = false;

            // Если условие true — добавляем контент блока в результат
            if (condition_met) {
                result.insert(result.end(), temp_block.begin(), temp_block.end());
            }

            temp_block.clear(); // Очищаем временный блок
            continue; // Пропускаем строку с @endif
        }

        // Если мы внутри блока @if — сохраняем строку во временный блок
        if (inside_if_block) {
            temp_block.push_back(line);
        }
        else {
            // Иначе — добавляем строку в результат
            result.push_back(line);
        }
    }

    // Если остался незакрытый блок @if — возвращаем исходный текст
    if (inside_if_block) {
        std::cerr << "Error: Unclosed @if block!" << std::endl;
        return lines;
    }

    return result;
}
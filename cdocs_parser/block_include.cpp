#include "class.hpp"

vector<string> CDOCS_parser::block_include(vector<string> blocks, const string& file_name, int depth) {
    // Защита от циклических инклудов
    constexpr int MAX_INCLUDE_DEPTH = 10;
    if (depth > MAX_INCLUDE_DEPTH) {
        throw std::runtime_error("Maximum include depth (" + 
                               std::to_string(MAX_INCLUDE_DEPTH) + 
                               ") exceeded");
    }

    // 1. Находим все @include директивы
    vector<string> out;
    vector<int> break_points;
    map<size_t, string> include_stack;
    regex include_regex(R"(@include\s*\(\s*([^)]+)\s*\))");
    smatch matches;

    // Собираем точки включения и пути
    for (size_t i = 0; i < blocks.size(); ++i) {
        const string& block = blocks[i];
        if (block.size() > 15 && block.size() < 100) {
            if (regex_search(block, matches, include_regex)) {
                fs::path full_path = resolve_include_path(matches[1].str(), file_name);
                include_stack[i] = full_path.string();
                break_points.push_back(i);
            }
        }
    }

    // 2. Формируем новый вектор
    size_t current_pos = 0;
    for (size_t break_point : break_points) {
        // Добавляем блоки до точки включения
        while (current_pos < break_point) {
            out.push_back(blocks[current_pos]);
            ++current_pos;
        }
        
        // Пропускаем сам блок с @include (current_pos == break_point)
        ++current_pos;
        
        // Добавляем содержимое включенного файла
        const string& include_path = include_stack[break_point];
        vector<string> included_content = CDOCS_files::read_file(include_path);
        included_content = block_include(std::move(included_content), include_path, depth + 1);
        
        // Вставляем обработанное содержимое
        out.insert(out.end(), 
                  make_move_iterator(included_content.begin()),
                  make_move_iterator(included_content.end()));
    }
    
    // Добавляем оставшиеся блоки после последнего include
    while (current_pos < blocks.size()) {
        out.push_back(blocks[current_pos]);
        ++current_pos;
    }

    return out;
}

fs::path CDOCS_parser::resolve_include_path(const string& path, const string& base_file) {
    fs::path include_path(path);
    fs::path base_path(base_file);

    if (include_path.is_absolute()) {
        return include_path.lexically_normal();
    }
    
    // Автоматическая обработка ./ и ../
    return (base_path.parent_path() / include_path).lexically_normal();
}
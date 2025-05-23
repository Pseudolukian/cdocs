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
    map<size_t, string> include_stack;
    regex include_regex(R"(@include\s*\(\s*([^)]+)\s*\))");

    for (size_t i = 0; i < blocks.size(); ++i) {
        smatch matches;
        if (regex_search(blocks[i], matches, include_regex)) {
            fs::path full_path = resolve_include_path(matches[1].str(), file_name);
            include_stack[i] = full_path.string();
        }
    }

    // 2. Обрабатываем в обратном порядке
    for (auto it = include_stack.rbegin(); it != include_stack.rend(); ++it) {
        const auto& [index, include_path] = *it;
        
        // Читаем файл (с рекурсивной обработкой инклудов)
        vector<string> included_content = CDOCS_files::read_file(include_path);
        included_content = block_include(std::move(included_content), include_path, depth + 1);

        // Заменяем блок
        blocks.erase(blocks.begin() + index);
        blocks.insert(blocks.begin() + index, 
                    make_move_iterator(included_content.begin()),
                    make_move_iterator(included_content.end()));
    }

    return blocks;
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
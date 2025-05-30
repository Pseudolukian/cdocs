#include "class.hpp"

std::pair<fs::path, std::string> CDOCS_parser::resolve_include_path(const std::string& path, const std::string& base_file) {
    // Ищем последнее вхождение '#' (якорь всегда в конце)
    size_t anchor_pos = path.rfind('#');
    std::string file_part = path;
    std::string anchor = "";

    // Если '#' найден и это не первый символ (например, не "#anchor")
    if (anchor_pos != std::string::npos && anchor_pos > 0) {
        file_part = path.substr(0, anchor_pos);
        anchor = path.substr(anchor_pos);  // сохраняем '#' для совместимости с read_file_ext
    }

    fs::path include_path(file_part);
    fs::path base_path(base_file);

    // Обрабатываем абсолютные пути
    if (include_path.is_absolute()) {
        return {include_path.lexically_normal(), anchor};
    }

    // Обрабатываем относительные пути (с учётом ./ и ../)
    fs::path resolved_path = (base_path.parent_path() / include_path).lexically_normal();
    return {resolved_path, anchor};
}
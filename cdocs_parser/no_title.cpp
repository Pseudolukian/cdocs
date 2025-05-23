#include "class.hpp"

vector<string> CDOCS_parser::no_title(vector<string>& blocks) {
    vector<string> output;
    regex title_regex(R"(^#{1,4}\s+.+)"); // Регулярное выражение для заголовков Markdown
    
    bool title_found = false;
    for (size_t i = 0; i < blocks.size(); i++) {
        if (!title_found && regex_match(blocks[i], title_regex)) {
            title_found = true;
            // Пропускаем следующую строку, если она пустая
            if (i + 1 < blocks.size() && blocks[i + 1].empty()) {
                i++;
            }
            continue;
        }
        output.push_back(blocks[i]);
    }
    
    return output;
}
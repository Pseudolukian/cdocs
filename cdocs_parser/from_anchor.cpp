//from anchor.cpp
#include "class.hpp"

vector<string> CDOCS_parser::from_anchor(vector<string>& blocks, string& anchor) {
    vector<string> output;
    regex anchor_regex(R"(\s*\{" + anchor + R"(\}\s*$)"); // Регулярка для якоря
    regex heading_regex(R"(^#{1,4}\s+.+)"); // Регулярка для заголовков
    
    bool anchor_found = false;
    
    for (const auto& line : blocks) {
        if (!anchor_found) {
            // Ищем строку с заголовком, содержащим нужный якорь
            if (regex_search(line, heading_regex) && regex_search(line, anchor_regex)) {
                anchor_found = true;
                output.push_back(line);
            }
        } else {
            // Если нашли следующий заголовок - прерываем сбор
            if (regex_match(line, heading_regex)) {
                break;
            }
            output.push_back(line);
        }
    }
    
    return output;
}
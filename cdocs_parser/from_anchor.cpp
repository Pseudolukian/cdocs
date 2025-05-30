//from anchor.cpp
#include "class.hpp"

vector<string> CDOCS_parser::from_anchor(vector<string>& blocks, string& anchor, std::regex& md_header) {
    vector<string> output;
    string anchor_pattern = "{" + anchor + "}";
    
    bool anchor_found = false;
    
    for (const auto& line : blocks) {
        if (!anchor_found) {
            // Ищем строку с заголовком, содержащим нужный якорь
            if (regex_search(line, md_header) && line.find(anchor_pattern) != string::npos) {
                anchor_found = true;
                output.push_back(line);
            }
        } else {
            // Если нашли следующий заголовок - прерываем сбор
            if (regex_match(line, md_header)) {
                break;
            }
            output.push_back(line);
        }
    }
    
    return output;
}
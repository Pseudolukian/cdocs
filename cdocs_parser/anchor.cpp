#include "class.hpp"

std::vector<std::string> CDOCS_parser::anchor(std::vector<std::string>& lines, std::string& anchor, std::regex& header) {
    std::vector<std::string> out;
    std::stack<int> range;

    size_t i = 0;
    for (const auto& line : lines) {
        if (line.size() < 80 && !line.empty() && line.find(anchor) != std::string::npos) {
            range.push(i);
        }

        if (std::regex_search(line, header)) {
            range.push(i);
        }
        i++;
    }

    
    if (range.size() >= 2) {
        int end = range.top(); range.pop();
        int start = range.top(); range.pop();
        
        // Проверка корректности диапазона
        if (start >= 0 && end < lines.size() && start <= end) {
            out.insert(out.end(), lines.begin() + start, lines.begin() + end + 1);
        }
    }

    return out;
}
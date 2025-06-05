#include "class.hpp"

std::vector<std::string> CDOCS_parser::notitle(std::vector<std::string>& lines) {
    std::vector<std::string> out;
    out.reserve(lines.size());
    
    bool headerFound = false;
    
    for (auto& line : lines) {
        // Check for the first header (starts with '#', not empty, and reasonable length)
        if (!headerFound && !line.empty() && line.size() < 80 && line[0] == '#') {
            headerFound = true;
            continue;
        }
        out.push_back(std::move(line));
    }
    
    return out;
}
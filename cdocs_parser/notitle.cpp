#include "class.hpp"

std::vector<std::string> CDOCS_parser::notitle(std::vector<std::string>& lines, const std::regex& header) {
    std::vector<std::string> out;
    out.reserve(lines.size());
    
    bool headerFound = false;
    
    for (auto& line : lines) {
        if (!headerFound && !line.empty() && line.size() < 80 && line.find('#') != std::string::npos) {
            if (std::regex_search(line, header)) {
                headerFound = true;
                continue;
            }
        }
        out.push_back(std::move(line));
    }
    
    return out;
}
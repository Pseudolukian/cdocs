#include "class.hpp"

std::vector<std::string> CDOCS_parser::block_if(const std::vector<std::string>& lines) {
    std::vector<int> break_points;
    std::vector<bool> condition_stack = {true}; // Stack for conditions

    int line_pos = 0;
    for (const std::string& line : lines) {
        bool current_condition = condition_stack.back();

        if (!current_condition) {
            break_points.push_back(line_pos);
        }

        if (line.size() > 10 && line.size() < 50) {
            // Check for @if ( conditions ) pattern
            if (line.find("@if (") != std::string::npos && line.back() == ')') {
                // Extract condition between "@if (" and ")"
                size_t start = line.find('(') + 1;
                size_t end = line.rfind(')');
                if (end != std::string::npos && end > start) {
                    std::string condition = line.substr(start, end - start);
                    bool condition_met = CDOCS_parser::if_cond_parser(condition);
                    condition_stack.push_back(current_condition && condition_met);
                    break_points.push_back(line_pos); // Remove @if line
                }
            }
        } else if (line.size() < 10) {
            // Check for @endif
            if (line.find("@endif") != std::string::npos) {
                condition_stack.pop_back(); // End current block
                break_points.push_back(line_pos); // Remove @endif line
            }
        }

        line_pos++;
    }

    // Convert break_points to set for fast lookup
    std::set<int> break_points_set(break_points.begin(), break_points.end());
    std::vector<std::string> result;

    // Add lines not in break_points
    for (size_t i = 0; i < lines.size(); ++i) {
        if (break_points_set.find(i) == break_points_set.end()) {
            result.push_back(std::move(lines[i]));
        }
    }

    return result;
}
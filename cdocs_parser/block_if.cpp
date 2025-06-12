#include "class.hpp"

std::vector<std::string> CDOCS_parser::block_if(const std::vector<std::string>& lines,
                                                const std::string& file_name,
                                                bool log_save, 
                                                const std::string log_path) {
    std::vector<int> break_points;
    std::vector<bool> condition_stack = {true}; // Stack for conditions
    std::vector<std::unique_ptr<ILogModel>> log_data;
    std::string current_condition; // Content inside @if parentheses
    std::string current_if_line; // Full @if line
    bool last_condition_met = false; // Track current @if condition result

    // Store original content for logging
    std::string origin_content;
    for (const auto& line : lines) {
        origin_content += line + "\n";
    }

    int line_pos = 0;
    for (const std::string& line : lines) {
        bool current_condition_met = condition_stack.back();

        if (!current_condition_met) {
            break_points.push_back(line_pos);
        }

        if (line.size() > 10 && line.size() < 50) {
            // Check for @if ( conditions ) pattern
            if (line.find("@if (") != std::string::npos && line.back() == ')') {
                size_t start = line.find('(') + 1;
                size_t end = line.rfind(')');
                if (end != std::string::npos && end > start) {
                    current_condition = line.substr(start, end - start);
                    current_if_line = line; // Store full @if line
                    last_condition_met = CDOCS_parser::if_cond_parser(current_condition);
                    condition_stack.push_back(current_condition_met && last_condition_met);
                    break_points.push_back(line_pos); // Remove @if line
                }
            }
        } else if (line.size() < 10) {
            // Check for @endif
            if (line.find("@endif") != std::string::npos) {
                if (log_save && !current_if_line.empty()) {
                    // Create log entry when we encounter @endif
                    auto log_entry = std::make_unique<LogBlockIf>();
                    log_entry->File = file_name;
                    log_entry->Block_if = current_if_line;
                    log_entry->Condition = current_condition;
                    log_entry->MET = last_condition_met ? "true" : "false";
                    log_entry->Origin_content = origin_content;
                    log_data.push_back(std::move(log_entry));
                }
                condition_stack.pop_back(); // End current block
                break_points.push_back(line_pos); // Remove @endif line
                current_condition.clear();
                current_if_line.clear();
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

    // Store final content in log entries (after all processing)
    if (log_save && !log_data.empty()) {
        std::string final_content;
        for (const auto& line : result) {
            final_content += line + "\n";
        }
        
        CDOCS_log::log("Block_IF", log_data, log_path);
        
        // Now that we've logged, we can update the final content
        for (auto& entry : log_data) {
            auto* block_if_entry = dynamic_cast<LogBlockIf*>(entry.get());
            if (block_if_entry) {
                block_if_entry->Final_content = final_content;
            }
        }
        
        // Write the updated log with final content
        CDOCS_log::log("Block_IF", log_data, log_path);
    }

    return result;
}
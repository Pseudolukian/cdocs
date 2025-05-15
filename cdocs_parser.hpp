#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <variant>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>

using namespace std;
using Value = variant<string, unsigned, double>;

class CDOCS_parser {
public:
    CDOCS_parser(std::vector<std::string> vars_list_) : vars_list_(vars_list_) {}

    map<string, Value> vars_from_txt() {
        map<string, Value> result;

        for (string var:vars_list_) {
            // Remove leading/trailing whitespace
            var.erase(0, var.find_first_not_of(" \t"));
            var.erase(var.find_last_not_of(" \t") + 1);
            
            // Skip empty vars
            if (var.empty()) continue;

            // Find the position of '='
            size_t eq_pos = var.find('=');
            if (eq_pos == string::npos) continue;

            // Extract key and value
            string key = var.substr(0, eq_pos);
            string value = var.substr(eq_pos + 1);

            // Trim spaces around key and value
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            if (key.empty() || value.empty()) continue;

            // Try to parse the value
            try {
                // Try as unsigned integer
                if (all_of(value.begin(), value.end(), ::isdigit)) {
                    size_t pos;
                    unsigned num = stoul(value, &pos);
                    if (pos == value.length()) {
                        result[key] = num;
                        continue;
                    }
                }

                // Try as double
                size_t pos;
                double num = stod(value, &pos);
                if (pos == value.length()) {
                    result[key] = num;
                    continue;
                }
            } catch (...) {
                // If number parsing fails, treat as string
            }

            // Store as string if not a number
            result[key] = value;
        }
        return result;
    }

    vector<string> vars_in_docs(vector<string>& file_lines, map<string, Value>& vars_list) {
    vector<string> result;
    regex var_pattern(R"(\{\{\s*(\w+)\s*\}\})"); // Matches {{ var_name }} with optional spaces

    for (const auto& line : file_lines) {
        string new_line = line;
        smatch match;

        // Find all variable placeholders in the line
        string temp_line = new_line;
        string result_line;
        size_t last_pos = 0;

        // Use regex_search to find all matches in the line
        while (regex_search(temp_line, match, var_pattern)) {
            // Add text before the match
            result_line += temp_line.substr(0, match.position());

            // Get the variable name (capture group 1)
            string var_name = match[1].str();

            // Find the variable in vars_list
            auto it = vars_list.find(var_name);
            if (it != vars_list.end()) {
                // Convert variant value to string
                string value_str = visit([](const auto& value) -> string {
                    if constexpr (is_same_v<decay_t<decltype(value)>, string>) {
                        return value;
                    } else {
                        ostringstream oss;
                        oss << value;
                        return oss.str();
                    }
                }, it->second);

                result_line += value_str;
            } else {
                // If variable not found, keep the original placeholder
                result_line += match[0].str();
            }

            // Move to the part of the string after the match
            last_pos += match.position() + match.length();
            temp_line = temp_line.substr(match.position() + match.length());
        }

        // Add remaining text after last match
        result_line += temp_line;

        result.push_back(result_line);
    }

    return result;
    }

private:
    std::vector<std::string> vars_list_;
};
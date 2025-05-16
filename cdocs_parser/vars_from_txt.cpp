#include "class.hpp"

map<string, Value> CDOCS_parser::vars_from_txt() {
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
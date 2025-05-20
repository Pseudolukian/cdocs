#include "class.hpp"
#include <iostream>
#include <regex>

using namespace std;

vector<string> CDOCS_parser::vars_in_docs(vector<string>& file_lines, const map<string, Value>& vars_list) {
    vector<string> result;
    const regex var_pattern(R"(\{\{\s*([a-zA-Z0-9_.-]+)\s*\}\})");

    for (const string& line : file_lines) {
        string processed_line = line;
        sregex_iterator it(processed_line.begin(), processed_line.end(), var_pattern);
        sregex_iterator end;

        // Store replacements to apply after iteration
        vector<pair<size_t, size_t>> positions;
        vector<string> replacements;

        for (; it != end; ++it) {
            string var_name = it->str(1);
            string full_match = it->str(0);

            auto var_it = vars_list.find(var_name);
            if (var_it != vars_list.end()) {
                // Convert Value to string
                string value = visit([](const auto& v) -> string {
                    if constexpr (is_same_v<decay_t<decltype(v)>, string>) {
                        return v;
                    } else {
                        return to_string(v);
                    }
                }, var_it->second);

                positions.emplace_back(it->position(0), full_match.length());
                replacements.push_back(value);
            }
        }

        // Apply replacements in reverse order to avoid offset issues
        for (int i = replacements.size() - 1; i >= 0; --i) {
            const auto& pos = positions[i];
            processed_line.replace(pos.first, pos.second, replacements[i]);
        }

        result.push_back(processed_line);
    }

    return result;
}
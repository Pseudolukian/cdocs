#include "class.hpp"
#include <iostream>
#include <regex>

using namespace std;

vector<string> CDOCS_parser::vars_in_docs(vector<string>& file_lines, map<string, Value>& vars_list) {
    vector<string> result;
    const regex var_pattern(R"(\s*\{\{\s*([a-zA-Z0-9_.-]+)\s*\}\}\s*)");

    for (const string& line : file_lines) {
        string processed_line = line;
        //cout << "Processing line: " << processed_line << endl;

        // Use regex_iterator to find all matches
        sregex_iterator it(processed_line.begin(), processed_line.end(), var_pattern);
        sregex_iterator end;

        // Store replacements to apply after iteration
        vector<pair<size_t, pair<size_t, string>>> replacements;

        for (; it != end; ++it) {
            string var_name = it->str(1);
            //cout << "Found variable: " << var_name << endl;

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

                // Store position, length, and replacement value
                replacements.emplace_back(it->position(0), make_pair(it->length(0), value));
            }
        }

        // Apply replacements in reverse order to avoid offset issues
        for (auto rit = replacements.rbegin(); rit != replacements.rend(); ++rit) {
            processed_line.replace(rit->first, rit->second.first, rit->second.second);
        }

        result.push_back(processed_line);
    }

    return result;
}
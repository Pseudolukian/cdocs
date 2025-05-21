#pragma once
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <variant>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <iostream>
#include "../cdocs_files/class.hpp"

using namespace std;
using Value = variant<string, unsigned, double>;

class CDOCS_parser {
public:
    CDOCS_parser();
    static map<string, Value> vars_from_file(std::vector<std::string> vars_list_);
    vector<string> block_include(vector<string>& file_lines, const map<string, Value>& vars_list, string file_name, int depth = 0);
    string resolve_include_path(const string& path, const string& base_file);
    vector<string> vars_in_docs(vector<string>& file_lines, const map<string, Value>& vars_list);
    
private:
    std::vector<std::string> vars_list_;
};
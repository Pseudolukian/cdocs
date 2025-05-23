//class.hpp
#pragma once
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <variant>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <regex>
#include <iostream>
#include "../cdocs_files/class.hpp"

using namespace std;
namespace fs = std::filesystem;
using Value = variant<string, unsigned, double>;

class CDOCS_parser {
public:
    CDOCS_parser();
    static map<string, Value> vars_from_file(std::vector<std::string> vars_list_);
    vector<string> block_include(vector<string> blocks, const string& file_name, int depth = 0);
    fs::path resolve_include_path(const string& path, const string& base_file);
    vector<string> vars_in_docs(vector<string>& file_lines, const map<string, Value>& vars_list);
    vector<string> inline_if(vector<string>& file_lines);
    bool if_cond_parser(const string& condition);
    
private:
    std::vector<std::string> vars_list_;
};
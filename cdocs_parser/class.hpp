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
#include <array>
#include <set>
#include "../cdocs_files/class.hpp"

using namespace std;
namespace fs = std::filesystem;
using Value = variant<string, unsigned, double>;

class CDOCS_parser {
public:
    CDOCS_parser();
    // Preprocessing inline
    static map<string, map<string, Value>> vars_from_file(std::vector<std::string>& vars_list, std::regex& md_vars_group);
    vector<string> vars_in_docs(vector<string>& file_lines, const std::map<std::string, std::map<std::string, Value>>& vars_list, std::regex& md_vars);
    vector<string> inline_if(vector<string>& file_lines);
    bool if_cond_parser(const string& condition);
    vector<string> no_title(vector<string>& blocks);
    vector<string> from_anchor(vector<string>& blocks, string& anchor, std::regex& md_header);
    
    // Preprocessing block
    vector<string> block_include(vector<string> blocks, const string& file_name, int depth, std::regex& block_include_, std::regex& block_include_no_title);
    std::pair<fs::path, std::string> resolve_include_path(const std::string& path, const std::string& base_file);
    std::vector<std::string> block_if(std::vector<std::string>& lines, std::regex& block_if_regex_start, std::regex& block_if_regex_end);
    
private:
    std::vector<std::string> vars_list_;
};
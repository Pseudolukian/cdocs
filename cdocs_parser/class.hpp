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
#include <unordered_map>

using namespace std;
namespace fs = std::filesystem;
using Value = variant<string, unsigned, double>;

class CDOCS_parser {
public:
    CDOCS_parser();
    // Preprocessing inline
    static map<string, map<string, Value>> vars_from_file(std::vector<std::string>& vars_list, std::regex& md_vars_group);
    vector<string> vars_in_docs(const vector<string>& file_lines, const std::map<std::string, std::map<std::string, Value>>& vars_list, std::regex& md_vars);
    vector<string> inline_if(const vector<string>& file_lines);
    bool if_cond_parser(const string& condition);
    vector<string> no_title(vector<string>& blocks);
    vector<string> from_anchor(vector<string>& blocks, string& anchor, std::regex& md_header);
    
    // Preprocessing block
    std::vector<std::string> block_include(const std::vector<std::string>& blocks, 
                               const std::string& file_name, 
                               const std::regex& include_regex,
                               const std::regex& header_regex,
                               const std::unordered_map<string, std::vector<std::string>>& buffer_content,
                               std::unordered_map<string, std::vector<std::string>>& buffer_include);
    std::vector<std::string> block_if(const std::vector<std::string>& lines, std::regex& block_if_regex_start, std::regex& block_if_regex_end);

    // Additions
    static std::vector<std::string> notitle(std::vector<std::string>& lines, std::regex& header);
    static std::vector<std::string> anchor(std::vector<std::string>& lines, std::string& anchor, std::regex& header);
    
private:
    std::vector<std::string> vars_list_;
};
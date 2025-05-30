#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <regex>
#include "../cdocs_parser/class.hpp"

namespace fs = std::filesystem;

class CDOCS_files {
public:
    std::string root_path_;
    std::string output_path_;
    std::vector<std::string> extensions_;

    CDOCS_files(const std::string& doc_root_path, 
    const std::string& doc_output_path,
    const std::vector<std::string>& extensions);
    
    std::vector<std::string> get_files_list();
    static std::vector<std::string> read_file(const std::string& filename);
    static std::vector<std::string> read_file_ext(const std::string& filename, std::string anchor = "", bool title = true);
    void save_file(const std::string& filename, const std::vector<std::string>& lines);
};
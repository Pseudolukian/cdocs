// class.hpp
#pragma once
#include <vector>
#include <string>
#include <map>
#include <variant>
#include <sstream>
#include "../cdocs_files/class.hpp"
#include "../cdocs_parser/class.hpp"

class Config {
public:
    std::string config_file_name;
    std::string docs_root_path;
    std::string docs_out_path;
    std::string docs_tmp_path;
    std::string files_extansions;
    std::map<std::string, std::variant<std::string, unsigned int, double>> global_vars;
    std::vector<std::string> vars_for_parsing;

    Config(const std::string& config_file_name) : config_file_name(config_file_name) {
        auto config_vars = CDOCS_parser::vars_from_file(CDOCS_files::read_file(config_file_name));
        docs_root_path = std::get<std::string>(config_vars["Docs_root"]);
        docs_out_path = std::get<std::string>(config_vars["Docs_output"]);
        docs_tmp_path = std::get<std::string>(config_vars["Docs_tmp"]);
        files_extansions = std::get<std::string>(config_vars["Files_extansions"]);
        
        

        for (const auto& [key, value] : config_vars) {

            if (key.find("_vars") != std::string::npos) {
                if (std::holds_alternative<std::string>(value)) {
                    vars_for_parsing.push_back(std::get<std::string>(value));
                }
            }
        }
        
        for (const auto& var : vars_for_parsing) {
            auto parsed_vars = CDOCS_parser::vars_from_file(CDOCS_files::read_file(var));
            for (const auto& [key, value] : parsed_vars) {
                global_vars[key] = value;
            }
        }
    }
};

static auto variantToString = [](const auto& value) -> std::string {
    std::ostringstream oss;
    oss << value;
    return oss.str();
};
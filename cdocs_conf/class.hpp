// class.hpp
#pragma once
#include <variant>
#include <sstream>
#include "../cdocs_files/class.hpp"
#include "../cdocs_parser/class.hpp"

class Config {
public:
    //Paths
    std::string config_file_name;
    std::string docs_root_path;
    std::string docs_out_path;
    std::string docs_tmp_path;
    std::string log_path;
    
    // Extansions
    std::string files_extansions;
    
    // Regex
    std::regex vars_group_regex;
    
    //Vars
    std::map<std::string, std::map<std::string, Value>> global_vars;
    std::map<std::string, std::map<std::string, Value>> vars_for_add;

    //Log
    bool log_vars;
    bool log_inline_if;
    bool log_include;

    Config(const std::string& config_file_name, std::regex vars_group_regex) : config_file_name(config_file_name), vars_group_regex(vars_group_regex) {
        std::vector<std::string> config = CDOCS_files::read_file(config_file_name);
        std::map<std::string, std::map<std::string, Value>> config_vars = CDOCS_parser::vars_from_file(config, vars_group_regex);
        docs_root_path = std::get<std::string>(config_vars["Paths"]["Docs_root"]);
        docs_out_path = std::get<std::string>(config_vars["Paths"]["Docs_output"]);
        docs_tmp_path = std::get<std::string>(config_vars["Paths"]["Docs_tmp"]);
        files_extansions = std::get<std::string>(config_vars["Parser"]["Files_extansions"]);
        std::stringstream ss(std::get<std::string>(config_vars["Parser"]["Vars"]));
        
        //Log
        log_vars = to_bool(std::get<std::string>(config_vars["Log"]["Log_module_vars"]));
        log_inline_if = to_bool(std::get<std::string>(config_vars["Log"]["Log_module_inline_if"]));
        log_include = to_bool(std::get<std::string>(config_vars["Log"]["log_module_block_include"]));
        log_path = std::get<std::string>(config_vars["Log"]["Log_dir"]);

        std::string token;
        while (std::getline(ss, token, ',')) {
            vars_files.push_back(token);
        }

        for (const std::string& file : vars_files) {
            std::vector<std::string> vars_set = CDOCS_files::read_file(file);
            auto file_vars = CDOCS_parser::vars_from_file(vars_set, vars_group_regex);
    
            // Вставляем все элементы из file_vars в global_vars
            for (const auto& [section, vars] : file_vars) {
                global_vars[section].insert(vars.begin(), vars.end());
                }
        }


    }

    static bool to_bool(const std::string& str);
    
    private:
        std::vector<std::string> vars_files;
        std::vector<std::string> vars_for_parsing;
};
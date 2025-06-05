// class.hpp
#pragma once
#include <vector>
#include <string>
#include <memory>
#include "models.hpp"

class CDOCS_log {
public:
    static void log(const std::string& log_group_name, 
            const std::vector<std::unique_ptr<ILogModel>>& log_data,
            const std::string& log_file_path);
private:
    static size_t global_counter;            
};
// log.cpp
#include "class.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <ctime>

namespace fs = std::filesystem;
size_t CDOCS_log::global_counter = 0;

void CDOCS_log::log(const std::string& log_group_name, 
                   const std::vector<std::unique_ptr<ILogModel>>& log_data,
                   const std::string& log_file_path) {
    if (log_data.empty()) {
        //std::cout << "Logging 0 entries to group: " << log_group_name << " (skipped, empty)\n";
        return;
    }

    // Формируем имя файла с текущей датой
    std::time_t now = std::time(nullptr);
    std::tm tm = *std::localtime(&now);
    std::ostringstream date_ss;
    date_ss << std::put_time(&tm, "%Y-%m-%d");
    std::string filename = log_group_name + "_" + date_ss.str() + ".log";
    fs::path full_path = fs::path(log_file_path) / filename;

    try {
        // Создаем директории, если их нет
        if (!fs::exists(full_path.parent_path())) {
            fs::create_directories(full_path.parent_path());
        }

        // Определяем, нужно ли писать заголовок (если файл новый)
        bool write_header = !fs::exists(full_path);

        // Открываем файл для записи (добавляем в конец)
        std::ofstream log_file(full_path, write_header ? std::ios::out : std::ios::app);
        if (!log_file.is_open()) {
            std::cerr << "Error: Failed to open log file: " << full_path.string() << "\n";
            return;
        }

        // Записываем заголовок группы логов (только если файл новый)
        if (write_header) {
            log_file << "[" << log_group_name << "]\n";
        }

        // Записываем каждую запись лога
        for (size_t i = 0; i < log_data.size(); ++i) {
            const auto& entry = dynamic_cast<const LogVar&>(*log_data[i]);
            
            log_file << ++global_counter << ". File: " << entry.File << "\n"
            << "   Var: {{ " << entry.Var << " }}\n"
            << "   Value: " << entry.Value << "\n"
            << "   Line before swap:" << "\n" << "         " << entry.Line_before_swap << "\n\n"
            << "   Line after swap: " << "\n" << "         " << entry.Line_after_swap << "\n\n";
        }

        log_file.close();
    } 
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
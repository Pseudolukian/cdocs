//main.cpp
#include "cdocs_conf/class.hpp"
#include "cdocs_regex/class.hpp"
#include "cdocs_buffer/class.hpp"
#include "cdocs_threadpool/class.hpp
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() {
    // Start total execution time measurement
    auto total_start = high_resolution_clock::now();

    CDOCS_buffer buffer = CDOCS_buffer();

    cout << "Compiling regex ..." << endl;
    auto comp_regex_start = high_resolution_clock::now();
    CDOCS_regex cdocs_regex = CDOCS_regex();
    auto comp_regex_end = high_resolution_clock::now();
    auto comp_regex_duration = duration_cast<milliseconds>(comp_regex_end - comp_regex_start);


    cout << "Uploading config and vars..." << endl;
    auto config_start = high_resolution_clock::now();
    Config conf = Config("cdocs.cfg", cdocs_regex.file_vars_group);
    CDOCS_files files = CDOCS_files(conf.docs_root_path, conf.docs_out_path, { conf.files_extansions });
    CDOCS_parser parser = CDOCS_parser();
    auto config_end = high_resolution_clock::now();
    auto config_duration = duration_cast<milliseconds>(config_end - config_start);


    cout << "Get file list...." << endl;
    auto get_file_list_start = high_resolution_clock::now();
    vector<string> files_list = files.get_files_list();
    auto get_file_list_end = high_resolution_clock::now();
    auto get_file_list_duration = duration_cast<milliseconds>(get_file_list_end - get_file_list_start);

    CDOCS_threadpool threadpool = CDOCS_threadpool(files_list);

    threadpool.thread();
    
    /*
    // Step 1. Preprocessing vars
    cout << "Preprocessing vars..." << endl;
    auto vars_start = high_resolution_clock::now();
    
    for (const auto& file : files_list) {
        vector<string> lines = files.read_file(conf.docs_root_path + file);
        buffer.after_vars_preproc[file] = parser.vars_in_docs(lines, conf.global_vars, file, conf.log_vars, conf.log_path);
    }
    
    auto vars_end = high_resolution_clock::now();
    auto vars_duration = duration_cast<milliseconds>(vars_end - vars_start);

    // Step 2. Preprocessing inline_if
    cout << "Preprocessing inline_if..." << endl;
    auto if_start = high_resolution_clock::now();

    for (const auto& [file, content] : buffer.after_vars_preproc) {
        buffer.after_inline_if_preproc[file] = parser.inline_if(content, file, conf.log_inline_if, conf.log_path);
    }
    buffer.after_vars_preproc.clear();
    
    auto if_end = high_resolution_clock::now();
    auto if_duration = duration_cast<milliseconds>(if_end - if_start);
    
    // Step 3. Preprocessing block_include
    auto include_start = high_resolution_clock::now();
    cout << "Preprocessing block_include..." << endl;
    
    for (const auto& [file, content] : buffer.after_inline_if_preproc) {
        buffer.after_includes_preproc[file] = parser.block_include(content, file, 
                                                cdocs_regex.md_block_include_, 
                                                cdocs_regex.md_header,
                                                buffer.after_inline_if_preproc, 
                                                buffer.includes,
                                                5,
                                                conf.log_include,
                                                conf.log_path);
    }
    
    buffer.after_inline_if_preproc.clear();
    
    auto include_end = high_resolution_clock::now();
    auto include_duration = duration_cast<milliseconds>(include_end - include_start);

    
    // Step 4. Preprocessing block_if
    auto block_if_start = high_resolution_clock::now();
    cout << "Preprocessing block_if..." << endl;

    for (const auto& [file, content] : buffer.after_includes_preproc) {
        files.save_file(conf.docs_out_path + file, parser.block_if(content, file, conf.log_block_if,conf.log_path));
    }

    buffer.after_includes_preproc.clear();
    buffer.includes.clear();
    
    auto block_if_end = high_resolution_clock::now();
    auto block_if_duration = duration_cast<milliseconds>(block_if_end - block_if_start);

    // Calculate total execution time
    auto total_end = high_resolution_clock::now();
    auto total_duration = duration_cast<milliseconds>(total_end - total_start);

    
    cout << "Files processed: " << files_list.size() << endl;


    cout << "Time taken to compile regex: " 
        << duration_cast<duration<double>>(comp_regex_duration).count() << " s" << endl;
    cout << "Time taken to load conf and vars: " 
        << duration_cast<duration<double>>(config_duration).count() << " s" << endl;

    cout << "Time taken to get file list to preproc: " 
        << duration_cast<duration<double>>(get_file_list_duration).count() << " s" << endl;

    cout << "Time taken to preprocess vars: " 
        << duration_cast<duration<double>>(vars_duration).count() << " s" << endl;
    cout << "Time taken to preprocess inline if: " 
        << duration_cast<duration<double>>(if_duration).count() << " s" << endl;
    cout << "Time taken to preprocess include: " 
        << duration_cast<duration<double>>(include_duration).count() << " s" << endl;
    cout << "Time taken to preprocess block_if: " 
        << duration_cast<duration<double>>(block_if_duration).count() << " s" << endl;
    cout << "Total execution time: " 
        << duration_cast<duration<double>>(total_duration).count() << " s" << endl;
    
    cout << "Count of includes in buffer: " << buffer.includes.size() << endl;
    */

    return 0;
}
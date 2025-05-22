//main.cpp
#include "cdocs_conf/class.hpp"
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() {
    // Start total execution time measurement
    auto total_start = high_resolution_clock::now();

    Config conf = Config("cdocs.cfg");
    CDOCS_files files = CDOCS_files(conf.docs_root_path, conf.docs_out_path, { conf.files_extansions });
    CDOCS_parser parser = CDOCS_parser();
    vector<string> files_list = files.get_files_list();
    
    // Step 1. Preprocessing vars
    cout << "Preprocessing vars..." << endl;
    auto vars_start = high_resolution_clock::now();
    
    for (const auto& file : files_list) {
        vector<string> lines = files.read_file(conf.docs_root_path + file);
        files.save_file(conf.docs_tmp_path + file, parser.vars_in_docs(lines, conf.global_vars));
    }
    
    auto vars_end = high_resolution_clock::now();
    auto vars_duration = duration_cast<milliseconds>(vars_end - vars_start);

    // Step 2. Preprocessing inline_if
    cout << "Preprocessing inline_if..." << endl;
    auto if_start = high_resolution_clock::now();

    for (const auto& file : files_list) {
        vector<string> lines = files.read_file(conf.docs_tmp_path + file);
        files.save_file(conf.docs_tmp_path + file, parser.inline_if(lines));
    }
    
    auto if_end = high_resolution_clock::now();
    auto if_duration = duration_cast<milliseconds>(if_end - if_start);

    
    // Step 3. Preprocessing block_include
    auto include_start = high_resolution_clock::now();
    cout << "Preprocessing block_include..." << endl;
    
    for (const auto& file : files_list) {
        vector<string> lines = files.read_file(conf.docs_tmp_path + file);
        files.save_file(conf.docs_tmp_path + file, parser.block_include(lines, conf.docs_tmp_path + file));
    }
    
    auto include_end = high_resolution_clock::now();
    auto include_duration = duration_cast<milliseconds>(include_end - include_start);
    

    // Calculate total execution time
    auto total_end = high_resolution_clock::now();
    auto total_duration = duration_cast<milliseconds>(total_end - total_start);

    cout << "Files processed: " << files_list.size() << endl;
    cout << "Time taken to preprocess vars: " 
        << duration_cast<duration<double>>(vars_duration).count() << " s" << endl;
    cout << "Time taken to preprocess inline if: " 
        << duration_cast<duration<double>>(if_duration).count() << " s" << endl;
    cout << "Time taken to preprocess include: " 
        << duration_cast<duration<double>>(include_duration).count() << " s" << endl;
    cout << "Total execution time: " 
        << duration_cast<duration<double>>(total_duration).count() << " s" << endl;
    
    return 0;
}
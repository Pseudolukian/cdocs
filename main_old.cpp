#include "cdocs_files/class.hpp"
#include "cdocs_parser/class.hpp"
#include "threadpool.hpp"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

auto variantToString = [](const auto& value) -> string {
    std::ostringstream oss;
    oss << value;
    return oss.str();
};

CDOCS_parser YDB_parser = CDOCS_parser();
CDOCS_files YDB_docs = CDOCS_files();

std::vector<std::string> row_conf_vars = YDB_docs.read_file("cdocs.cfg"); 
map<string, Value> config_vars = YDB_parser.vars_from_file(row_conf_vars);
string root_path = std::visit(variantToString, config_vars["Docs_root"]);
string output_path = std::visit(variantToString, config_vars["Docs_output"]);
string extantions = std::visit(variantToString, config_vars["Files_extansions"]);
string global_vars_file = std::visit(variantToString, config_vars["Vars_file"]);

std::vector<std::string> row_vars = YDB_docs.read_file(global_vars_file);         // Read the vars file and store the variables in a vector
map<string, Value> global_vars = YDB_parser.vars_from_file(row_vars);
vector<string> files_in_documentation = YDB_docs.get_files_list();          // Get the list of files in the docs directory

// Sync or Async choose
bool async = true;

int main () {
    if (async) {
        ThreadPool pool;
        // Define the task to be executed for each file
        auto process_file = [&](const string& file) {
            vector<string> file_lines = YDB_docs.read_file(root_path + file);
            vector<string> result_lines = YDB_parser.vars_in_docs(file_lines, global_vars);
            YDB_docs.save_file(output_path + file, result_lines);
        };

        // Execute tasks using thread pool
        pool.execute(files_in_documentation, process_file);
        
        // Wait for all threads to complete
        pool.wait();
    } 
    
    else {
        for (string file:files_in_documentation) {
        vector<string> file_lines = YDB_docs.read_file(root_path + file);
        vector<string> result_lines = YDB_parser.vars_in_docs(file_lines, global_vars);
        YDB_docs.save_file(output_path + file, result_lines);
        }
    }
    return 0;
}
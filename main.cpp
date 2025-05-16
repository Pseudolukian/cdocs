#include "cdocs_files/class.hpp"
#include "cdocs_parser/class.hpp"
#include <iostream>

using namespace std;

string root_path = "/home/pynan/C++_exp/docs/in/ydb_doc_ru/";
string output_path = "/home/pynan/C++_exp/docs/out/";

CDOCS_files YDB_docs = CDOCS_files(root_path, output_path, {".md"});

std::vector<std::string> row_vars = YDB_docs.read_file("vars.txt"); // Read the vars file and store the variables in a vector
CDOCS_parser YDB_parser = CDOCS_parser(row_vars);                   // Create an instance of the parser with the variables
map<string, Value> global_vars = YDB_parser.vars_from_txt();        // Read the variables from the file and store them in a map
vector<string> files_in_documentation = YDB_docs.get_files_list();  // Get the list of files in the docs directory

int main () {
    for (string file:files_in_documentation) {
        vector<string> file_lines = YDB_docs.read_file(root_path + file);
        vector<string> result_lines = YDB_parser.vars_in_docs(file_lines, global_vars);
        YDB_docs.save_file(output_path + file, result_lines);
    }
}
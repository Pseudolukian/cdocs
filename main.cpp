#include "cdocs_files.hpp"
#include "cdocs_parser.hpp"
#include <iostream>

using namespace std;

string root_path = "/home/pynan/C++_exp/doc_project/docs/in/ydb_doc_ru";
string output_path = "/home/pynan/C++_exp/doc_project/docs/out/";

CDOCS_files YDB_docs = CDOCS_files(root_path, output_path, {".md"});


std::vector<std::string> row_vars = YDB_docs.read_file("vars.txt");                        
CDOCS_parser YDB_parser = CDOCS_parser(row_vars);
map<string, Value> global_vars = YDB_parser.vars_from_txt();
vector<string> files_in_docs = YDB_docs.get_files_list();

int main () {
    for (string file:files_in_docs) {
        vector<string> file_lines = YDB_docs.read_file(root_path + file);
        vector<string> result_lines = YDB_parser.vars_in_docs(file_lines, global_vars);
        YDB_docs.save_file(file, result_lines);
        
    }

    return 0;
}
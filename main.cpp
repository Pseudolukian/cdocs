//main.cpp
#include "cdocs_conf/class.hpp"

using namespace std;



int main() {

    Config conf = Config("cdocs.cfg");
    CDOCS_files files = CDOCS_files(conf.docs_root_path, conf.docs_out_path, { conf.files_extansions });
    CDOCS_parser parser = CDOCS_parser();
    vector<string> files_list = files.get_files_list();
    
    for (const auto& file : files_list) {
        vector<string> lines = files.read_file(conf.docs_root_path + file);
        files.save_file(conf.docs_tmp_path + file, parser.vars_in_docs(lines, conf.global_vars));
    }

    for (const auto& file : files_list) {
    vector<string> lines = files.read_file(conf.docs_tmp_path + file);
    files.save_file(conf.docs_tmp_path + file, parser.block_include(lines, conf.global_vars, conf.docs_tmp_path + file));
    }
    
    return 0;

}
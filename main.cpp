#include "cdocs_conf/class.hpp"
#include "threadpool.hpp"

using namespace std;

/* Upload vars from configuration file */
CDOCS_conf YDB_conf = CDOCS_conf();
vector<string> row_conf_vars = YDB_conf.read_conf("cdocs.cfg");
map<string, Value> config_vars = YDB_conf.vars_from_conf(row_conf_vars);

/* Set config vars */
string root_path = std::visit(variantToString, config_vars["Docs_root"]);
string output_path = std::visit(variantToString, config_vars["Docs_output"]);
string extantions = std::visit(variantToString, config_vars["Files_extansions"]);
string global_vars_file = std::visit(variantToString, config_vars["Global_vars"]);
string team_vars_file = std::visit(variantToString, config_vars["Team_vars"]);


CDOCS_files YDB_docs = CDOCS_files(root_path, output_path, {extantions});
CDOCS_parser YDB_parser = CDOCS_parser();

std::vector<std::string> row_global_vars = YDB_docs.read_file(global_vars_file);
std::vector<std::string> row_team_vars = YDB_docs.read_file(team_vars_file);


map<string, Value> global_vars = YDB_parser.vars_from_file(row_global_vars);
map<string, Value> team_vars = YDB_parser.vars_from_file(row_team_vars);
map<string, Value> all_vars = global_vars;  // Копируем global_vars


vector<string> files_in_documentation = YDB_docs.get_files_list();

// Sync or Async choose
bool async = true;

int main () {
    all_vars.merge(team_vars);  // Переносим элементы из team_vars (без перезаписи)
    
    if (async) {
        ThreadPool pool;
        // Define the task to be executed for each file
        auto process_file = [&](const string& file) {
            vector<string> file_lines = YDB_docs.read_file(root_path + file);
            //vector<string> result_lines = YDB_parser.structure_parser(file_lines, global_vars, root_path + file);
            vector<string> result_lines = YDB_parser.vars_in_docs(file_lines, all_vars);
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
        vector<string> result_lines = YDB_parser.vars_in_docs(file_lines, all_vars);
        YDB_docs.save_file(output_path + file, result_lines);
        }
    }
    return 0;
}
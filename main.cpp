#include "cdocs_process/class.hpp"
#include "cdocs_conf/class.hpp"

using namespace std;

int main() {
    try {
        /* Upload vars from configuration file */
        CDOCS_conf YDB_conf = CDOCS_conf();
        vector<string> row_conf_vars = YDB_conf.read_conf("cdocs.cfg");
        map<string, Value> config_vars = YDB_conf.vars_from_conf(row_conf_vars);

        /* Set config vars */
        string root_path = std::visit(variantToString, config_vars["Docs_root"]);
        string output_path = std::visit(variantToString, config_vars["Docs_output"]);
        string tmp_path = std::visit(variantToString, config_vars["Docs_tmp"]);
        string extantions = std::visit(variantToString, config_vars["Files_extansions"]);
        string global_vars_file = std::visit(variantToString, config_vars["Global_vars"]);
        string team_vars_file = std::visit(variantToString, config_vars["Team_vars"]);

        // Добавляем слеш в конец путей, если его нет
        if (root_path.back() != '/') root_path += '/';
        if (tmp_path.back() != '/') tmp_path += '/';
        if (output_path.back() != '/') output_path += '/';

        // Создаем временную директорию, если её нет
        if (!filesystem::exists(tmp_path)) {
            filesystem::create_directories(tmp_path);
        }

        CDOCS_files YDB_docs = CDOCS_files(root_path, output_path, {extantions});
        CDOCS_parser YDB_parser = CDOCS_parser();

        cout << "Reading global vars from: " << global_vars_file << endl;
        std::vector<std::string> row_global_vars = YDB_docs.read_file(global_vars_file);
        
        cout << "Reading team vars from: " << team_vars_file << endl;
        std::vector<std::string> row_team_vars = YDB_docs.read_file(team_vars_file);

        map<string, Value> global_vars = YDB_parser.vars_from_file(row_global_vars);
        map<string, Value> team_vars = YDB_parser.vars_from_file(row_team_vars);
        map<string, Value> all_vars = global_vars;
        all_vars.insert(team_vars.begin(), team_vars.end());

        cout << "Getting files list..." << endl;
        vector<string> files_in_documentation = YDB_docs.get_files_list();
        cout << "Found " << files_in_documentation.size() << " files to process" << endl;
        CDOCS_process YDB_docs_process = CDOCS_process(YDB_docs, YDB_parser, root_path, tmp_path, all_vars);

        // Этап 1: Обработка переменных
        cout << "\n=== Processing variables stage ===" << endl;
        for (const string& file : files_in_documentation) {
            YDB_docs_process.process_vars_stage(file);
        }

        // Этап 2: Обработка include
        cout << "\n=== Processing includes stage ===" << endl;
        for (const string& file : files_in_documentation) {
            YDB_docs_process.process_includes_stage(file);
        }

        cout << "\nProcessing completed!" << endl;
    } catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
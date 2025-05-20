#pragma once
#include "../cdocs_files/class.hpp"
#include "../cdocs_parser/class.hpp"

class CDOCS_process {
    
    public:
        CDOCS_process(CDOCS_files& docs, CDOCS_parser& parser, const string& root_path,
                    const string& tmp_path, const map<string, Value>& all_vars) : docs(docs), parser(parser), root_path(root_path), tmp_path(tmp_path), all_vars(all_vars){};
        void process_vars_stage(const string& file) {
            cout << "Processing variables in: " << file << endl;
            
            try {
                // Читаем исходный файл
                vector<string> file_blocks = docs.read_file(root_path + file);
                
                // Заменяем переменные
                vector<string> blocks_preproc_vars = parser.vars_in_docs(file_blocks, all_vars);
                
                // Сохраняем результат с переменными в tmp
                docs.save_file(tmp_path + file, blocks_preproc_vars);
            } catch (const exception& e) {
                cerr << "Error processing variables in " << file << ": " << e.what() << endl;
            }
        }
        // Функция для обработки одного файла (включение файлов)
        void process_includes_stage(const string& file) {
            cout << "Processing includes in: " << file << endl;
            
            try {
                // Читаем файл с замененными переменными из tmp
                vector<string> file_blocks = docs.read_file(tmp_path + file);
                
                // Обрабатываем @include
                vector<string> include_block_parse = parser.block_parser(file_blocks, global_vars, tmp_path + file);
                
                // Сохраняем результат с обработанными include обратно в tmp
                docs.save_file(tmp_path + file, include_block_parse);
            } catch (const exception& e) {
                cerr << "Error processing includes in " << file << ": " << e.what() << endl;
            }
        }
    private:
        CDOCS_files& docs;
        CDOCS_parser& parser;
        string root_path;
        string tmp_path;
        map<string, Value> all_vars;
        map<string, Value> global_vars;
};
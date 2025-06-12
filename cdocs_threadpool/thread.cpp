#include "class.hpp"

void CDOCS_threadpool::thread () {
        
        // Step 1. Preprocessing vars   
        for (const auto& file : files_list) {
            vector<string> lines = files.read_file(conf.docs_root_path + file);
            buffer.after_vars_preproc[file] = parser.vars_in_docs(lines, conf.global_vars, file, conf.log_vars, conf.log_path);
        }
        

        // Step 2. Preprocessing inline_if
        for (const auto& [file, content] : buffer.after_vars_preproc) {
            buffer.after_inline_if_preproc[file] = parser.inline_if(content, file, conf.log_inline_if, conf.log_path);
        }
        
        
        // Step 3. Preprocessing block_include
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
        
      
        
        // Step 4. Preprocessing block_if
        for (const auto& [file, content] : buffer.after_includes_preproc) {
            buffer.after_block_if_preproc[file] = parser.block_if(content, file, conf.log_block_if,conf.log_path);
        }

};
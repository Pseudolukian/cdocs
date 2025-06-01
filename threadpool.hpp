

void thread (std::vector<std::string>& files_list) {
    // Step 1. Preprocessing vars
    for (const auto& file : files_list) {
        vector<string> lines = files.read_file(conf.docs_root_path + file);
        files.save_file(conf.docs_tmp_path + file, parser.vars_in_docs(lines, conf.global_vars, cdocs_regex.md_vars));
    }

    // Step 2. Preprocessing inline_if
    for (const auto& file : files_list) {
        vector<string> lines = files.read_file(conf.docs_tmp_path + file);
        files.save_file(conf.docs_tmp_path + file, parser.inline_if(lines));
    }
    
    // Step 3. Preprocessing block_include
    for (const auto& file : files_list) {
        vector<string> lines = files.read_file(conf.docs_tmp_path + file);
        files.save_file(conf.docs_tmp_path + file, 
            parser.block_include(lines, conf.docs_tmp_path + file, 1, cdocs_regex.md_block_include_, cdocs_regex.md_block_include_no_title)
        );
    }
    
    // Step 4. Preprocessing block_if
    for (const auto& file : files_list) {
        vector<string> lines = files.read_file(conf.docs_tmp_path + file);
        files.save_file(conf.docs_out_path + file, parser.block_if(lines, cdocs_regex.md_block_if_start, cdocs_regex.md_block_if_end));
    }
}


    



    
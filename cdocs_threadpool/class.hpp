#pragma once
#include "../cdocs_conf/class.hpp"
#include "../cdocs_regex/class.hpp"
#include "../cdocs_buffer/class.hpp"
#include "../cdocs_files/class.hpp"

class CDOCS_threadpool {
public:
    CDOCS_threadpool () = default;
    CDOCS_buffer buffer = CDOCS_buffer();
    CDOCS_regex cdocs_regex = CDOCS_regex();
    Config conf = Config("cdocs.cfg", cdocs_regex.file_vars_group);
    CDOCS_files files = CDOCS_files(conf.docs_root_path, conf.docs_out_path, { conf.files_extansions });
    CDOCS_parser parser = CDOCS_parser();
    std::vector<std::string> files_list = files.get_files_list();
    
    void thread ();
    
    void save_md_preproc();
};
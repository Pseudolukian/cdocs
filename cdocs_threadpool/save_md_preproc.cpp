#include "class.hpp"
void CDOCS_threadpool::save_md_preproc() {
    for (const auto& [file,content] : buffer.after_block_if_preproc) {
        if (file.find("_includes") != std::string::npos) {
            continue;
        }
        files.save_file(conf.docs_out_path + file, content);
    }
}
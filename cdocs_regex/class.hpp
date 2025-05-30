//class.hpp
#include <regex>

class CDOCS_regex {
public:
    std::regex md_vars;
    std::regex md_header;
    std::regex md_block_if_start;
    std::regex md_block_if_end;
    std::regex md_block_include_;
    std::regex md_block_include_no_title;
    std::regex md_include_path;
    std::regex file_vars_group;

    CDOCS_regex() : 
        md_header(R"(#{1,4}\s.*\s)"),
        md_block_if_start(R"(@if\s*\(\s*([^)]+)\s*\))"),
        md_block_if_end(R"(@endif)"),
        md_block_include_(R"(@include\s*\(\s*([^)]+)\s*\))"),
        md_block_include_no_title(R"(@include\s*notitle\s*\(\s*([^)]+)\s*\))"),
        md_include_path(R"(^#{1,4}\s+.+)"),
        file_vars_group(R"(\[([^\]]+)\])"),
        md_vars(R"(\{\{\s*([a-zA-Z0-9_.-]+)\s*\}\})")
    {}
};
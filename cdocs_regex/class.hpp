//class.hpp
#include <regex>

class CDOCS_regex {
public:
    std::regex md_vars;
    std::regex md_header;
    std::regex md_header_anchor;
    std::regex md_block_if_start;
    std::regex md_block_if_end;
    std::regex md_block_include_;
    std::regex file_vars_group;

    CDOCS_regex() : 
        md_header(R"(#{1,4}\s.*\s)"),
        md_header_anchor(R"(#{1,4}\s.*\s\{\s\.*\s\}\n)"),
        md_block_if_start(R"(@if\s*\(\s*([^)]+)\s*\))"),
        md_block_if_end(R"(@endif)"),
        md_block_include_(R"(@include\s*\(\s*([^,)]+)\s*(?:,\s*(notitle)\s*)?(?:,\s*(#[^)]+)\s*)?\)\s*(\n)?)"),
        file_vars_group(R"(\[([^\]]+)\])"),
        md_vars(R"(\{\{\s*([a-zA-Z0-9_.-]+)\s*\}\})")
    {}
};
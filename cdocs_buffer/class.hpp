class CDOCS_buffer {
    public:
        std::unordered_map<string, std::vector<std::string>> after_vars_preproc;
        std::unordered_map<string, std::vector<std::string>> after_inline_if_preproc;
        std::unordered_map<string, std::vector<std::string>> after_includes_preproc;
        std::unordered_map<string, std::vector<std::string>> after_block_if_preproc;
        std::unordered_map<string, std::vector<std::string>> includes;
        CDOCS_buffer () {}
};
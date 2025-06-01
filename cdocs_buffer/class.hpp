

class CDOCS_buffer {
    public:
        std::unordered_map<string, std::vector<std::string>> doc_files;
        std::unordered_map<string, std::vector<std::string>> includes;
        CDOCS_buffer () {}
};
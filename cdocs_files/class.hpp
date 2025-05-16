#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

class CDOCS_files {
public:
    CDOCS_files(const std::string& doc_root_path, 
    const std::string& doc_output_path,
    const std::vector<std::string>& extensions);
    
    std::vector<std::string> get_files_list();
    static std::vector<std::string> read_file(const std::string& filename);
    void save_file(const std::string& filename, const std::vector<std::string>& lines);

private:
    std::string root_path_;
    std::string output_path_;
    std::vector<std::string> extensions_;
};
#include <vector>
#include <string>
#include "../cdocs_files/class.hpp"
#include "../cdocs_parser/class.hpp"

class CDOCS_conf {
    public:
        CDOCS_conf() = default;
        std::vector<std::string> read_conf(const std::string& filename) {return CDOCS_files::read_file(filename);}
        map<string, Value> vars_from_conf(std::vector<std::string> vars_list_) {return CDOCS_parser::vars_from_file(vars_list_);}
};
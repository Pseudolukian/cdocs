#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <variant>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>

using namespace std;
using Value = variant<string, unsigned, double>;

class CDOCS_parser {
public:
    CDOCS_parser();
    static map<string, Value> vars_from_file(std::vector<std::string> vars_list_);
    vector<string> vars_in_docs(vector<string>& file_lines, map<string, Value>& vars_list);

private:
    std::vector<std::string> vars_list_;
};
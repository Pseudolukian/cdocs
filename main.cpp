#include "cdocs_conf/class.hpp"

using namespace std;

CDOCS_conf YDB_conf = CDOCS_conf();
vector<string> row_conf_vars = YDB_conf.read_conf("cdocs.cfg");
map<string, Value> config_vars = YDB_conf.vars_from_conf(row_conf_vars);

int main() {
    for (const auto& var:config_vars) {
        cout << var.first << endl;
    }
    return 0;
}
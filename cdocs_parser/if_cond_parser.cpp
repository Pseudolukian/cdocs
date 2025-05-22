//if_cond_parser.cpp
#include "class.hpp"

bool CDOCS_parser::if_cond_parser(const string& condition) {
    // Разбиваем условие на части по "&"
    stringstream ss(condition);
    string token;
    bool result = true;

    while (getline(ss, token, '&')) {
        // Удаляем пробелы в начале и конце
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);

        // Ищем операторы "==" или "!="
        size_t eq_pos = token.find("==");
        size_t neq_pos = token.find("!=");
        size_t op_pos = string::npos;
        bool is_equal = true; // true для ==, false для !=

        if (eq_pos != string::npos) {
            op_pos = eq_pos;
            is_equal = true;
        } else if (neq_pos != string::npos) {
            op_pos = neq_pos;
            is_equal = false;
        } else {
            cout << "Warning: Invalid condition format (missing == or !=): " << token << endl;
            return false;
        }

        // Разбиваем на левую и правую части
        string left = token.substr(0, op_pos);
        left.erase(left.find_last_not_of(" \t") + 1); // Удаляем пробелы
        string right = token.substr(op_pos + 2);
        right.erase(0, right.find_first_not_of(" \t")); // Удаляем пробелы

        // Сравниваем строки
        bool sub_result = is_equal ? (left == right) : (left != right);
        result = result && sub_result;
        if (!result) {
            return false; // Если хотя бы одно подусловие ложно, возвращаем false
        }
    }

    return result;
}
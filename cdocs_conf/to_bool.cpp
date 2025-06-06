//tobool.cpp
#include "class.hpp"

bool Config::to_bool(const std::string& str) {
    std::string lower;
    lower.reserve(str.size());
    for (char c : str) {
        lower.push_back(std::tolower(c));
    }
    
    // Быстрая проверка для часто встречающихся значений
    if (lower == "true" || lower == "1" || lower == "on" || lower == "yes") return true;
    if (lower == "false" || lower == "0" || lower == "off" || lower == "no") return false;
    
    // Более сложные случаи через stringstream
    std::istringstream is(lower);
    bool b;
    is >> std::boolalpha >> b;
    
    if (is.fail() || !is.eof()) {
        throw std::invalid_argument("Invalid boolean string: " + str);
    }
    
    return b;
}
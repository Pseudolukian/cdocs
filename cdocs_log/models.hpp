// models.hpp
#pragma once
#include <string>
#include <memory>
#include <format>  // Добавляем для std::format

struct ILogModel {
    virtual std::string format_entry(int index) const = 0;
    virtual ~ILogModel() = default;
};

struct LogVar : ILogModel {
    std::string File;
    std::string Var;  // Теперь хранит только имя переменной без {{ }}
    std::string Line_before_swap;
    std::string Line_after_swap;
    std::string Value;

    std::string format_entry(int index) const override {
        return std::format(
            "{}. File: {}\n"
            "   Var: {{ {} }}\n"
            "   Value: {}\n"
            "   Line_before_swap: {}\n\n"
            "   Line_after_swap: {}\n\n",
            index, File, Var, Value, Line_before_swap, Line_after_swap
        );
    }
};
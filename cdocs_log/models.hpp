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


struct LogInlineIf : ILogModel {
    std::string File;
    std::string Condition;
    std::string MET;
    std::string Line_original;
    std::string Line_final;

    std::string format_entry(int index) const override {
        return std::format(
            "{}. File: {}\n"
            "   Condition: {{ {} }}\n"
            "   MET: {}\n"
            "   Line_original: {}\n\n"
            "   Line_final: {}\n\n",
            index, File, Condition, MET, Line_original, Line_final
        );
    }
};


struct LogInclude : ILogModel {
    std::string File;
    std::string Include_block;
    std::string Notitle;
    std::string Anchor;
    std::string Include_content;

    std::string format_entry(int index) const override {
        return std::format(
            "{}. File: {}\n"
            "   Include_block: {}\n"
            "       Include options:\n"
            "           Notitle: {}\n"
            "           Anchor: {}\n"
            "   Include_content:\n{}\n\n",
            index, File, Include_block, Notitle, Anchor, Include_content
        );
    }
};


struct LogBlockIf : ILogModel {
    std::string File;
    std::string Block_if;
    std::string Condition;
    std::string MET;
    std::string Origin_content;
    std::string Final_content;

    std::string format_entry(int index) const override {
        return std::format(
            "{}. File: {}\n"
            "   Block if: {}\n"
            "   Condition: {}\n"
            "   MET: {}\n"
            "   Origin content:\n{}\n"
            "   Final content:\n{}\n\n",
            index, File, Block_if, Condition, MET, Origin_content, Final_content
        );
    }
};
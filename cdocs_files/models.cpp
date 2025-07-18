#include "class.hpp"
#include <uuid/uuid.h>
#include <string>
#include <vector>
#include <format>

struct AricleMetaData {
    uuid_t CDOCS_ID; // Используем uuid_t без std::
    std::string TOC_TITLE;
    std::string HTML_TITLE;
    std::string HTML_SNIPPET_PREVIEW;
    std::string HTML_SNIPPET_BANNER_PATH; 
    std::vector<std::string> HTML_KEYWORDS;
    std::string CDOCS_CATEGORY;
    std::vector<std::string> CDOCS_TAGS;
    std::string TOC_STATUS;
    int CDOCS_OPTIMIZE_DATA;

    AricleMetaData() {
        uuid_generate(CDOCS_ID); // Генерируем UUID
    }

    std::string metadata_prepare() {
        std::string keywords_str;
        for (const auto& kw : HTML_KEYWORDS) {
            if (!keywords_str.empty()) keywords_str += ", ";
            keywords_str += kw;
        }
        
        std::string tags_str;
        for (const auto& tag : CDOCS_TAGS) {
            if (!tags_str.empty()) tags_str += ", ";
            tags_str += tag;
        }

        // Преобразуем UUID в строку
        char uuid_str[37]; // UUID в строковом формате занимает 36 символов + '\0'
        uuid_unparse(CDOCS_ID, uuid_str);

        return std::format(
            "---\n"
            "CDOCS_ID: {}\n"
            "TOC_TITLE: {}\n"
            "HTML_TITLE: {}\n"
            "HTML_SNIPPET_PREVIEW: {}\n"
            "HTML_SNIPPET_BANNER_PATH: {}\n"
            "HTML_KEYWORDS: [{}]\n"
            "CDOCS_CATEGORY: {}\n"
            "CDOCS_TAGS: [{}]\n"
            "TOC_STATUS: {}\n"
            "CDOCS_OPTIMIZE_DATA: {}\n"
            "---\n",
            uuid_str, // Используем строковое представление UUID
            TOC_TITLE, HTML_TITLE, HTML_SNIPPET_PREVIEW, 
            HTML_SNIPPET_BANNER_PATH, keywords_str, CDOCS_CATEGORY, 
            tags_str, TOC_STATUS, CDOCS_OPTIMIZE_DATA
        );
    }
};
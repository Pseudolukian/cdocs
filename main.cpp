#include "cdocs_threadpool/class.hpp"
#include "cli11/CLI11.hpp"
#include "cdocs_files/class.hpp"

int main(int argc, char** argv) {
    CLI::App app{"Create a markdown documentation file"};
    argv = app.ensure_utf8(argv);

    std::string filename = "default";
    app.add_option("-f,--file", filename, "Name of the markdown file to create (without .md extension)");

    CLI11_PARSE(app, argc, argv);

    // Add .md extension if not present
    if (filename.find(".md") == std::string::npos) {
        filename += ".md";
    }

    // Get current working directory and construct full path
    std::filesystem::path filepath = std::filesystem::current_path() / filename;

    try {
        CDOCS_files::create_article(filepath);
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
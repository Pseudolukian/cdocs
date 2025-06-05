#include "class.hpp"

std::vector<std::string> CDOCS_parser::anchor(std::vector<std::string>& lines, std::string anchor) {
    std::vector<std::string> out;
    size_t start_index = 0;
    bool found_anchor = false;

    // Construct the anchor pattern to search for: {anchor} or { anchor }
    std::string anchor_pattern = "{" + anchor + "}";
    std::string anchor_pattern_spaced = "{ " + anchor + " }";

    for (size_t i = 0; i < lines.size(); ++i) {
        const auto& line = lines[i];

        // Check if line contains the anchor pattern
        if (!found_anchor && line.size() < 80 && !line.empty()) {
            if (line.find(anchor_pattern) != std::string::npos || 
                line.find(anchor_pattern_spaced) != std::string::npos) {
                // Look for closing brace '}' using rfind
                size_t closing_brace = line.rfind('}');
                if (closing_brace != std::string::npos) {
                    start_index = i;
                    found_anchor = true;
                }
            }
        }

        // If anchor is found, collect content until the next header of the same level
        if (found_anchor) {
            // Check if the current line is a header (starts with '#')
            if (!line.empty() && line[0] == '#') {
                // Count the number of '#' to determine header level
                size_t header_level = 0;
                while (header_level < line.size() && line[header_level] == '#') {
                    ++header_level;
                }

                // If this is the first line (the anchor header)
                if (i == start_index) {
                    out.push_back(line);
                    continue;
                }

                // If we encounter another header of the same or higher level (fewer or equal #), stop
                if (header_level <= out[0].find_first_not_of('#')) {
                    break;
                }
            }

            // Add the line to output if it's not the next header
            if (i != start_index || out.empty()) {
                out.push_back(line);
            }
        }
    }

    return out;
}
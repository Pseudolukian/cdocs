#include "class.hpp"

/*

Combinations:
Case 1: `anchor.empty() && title == true`:  @include (some/path/to/file.md)  
Case 2: `!anchor.empty() && title == true`: @include (some/path/to/file.md#some_anchor)
Case 3: `anchor.empty() && title == false`: @include notitle (some/path/to/file.md)  
Case 4: `!anchor.empty() && title == false`: @include notitle (some/path/to/file.md#some_anchor.md)

*/
std::vector<std::string> CDOCS_files::read_file_ext(const std::string& filename, 
                                                   std::string anchor, 
                                                   bool title) {
    std::vector<std::string> out;
    std::vector<int> break_points;
    std::regex header(R"(#{1,4}\s.*\s)");
    std::smatch match;
    
    std::string final_anchor = "{" + anchor + "}";

    // Case 1: No anchor and title=true - return entire file
    if (anchor.empty() && title == true) {
        return CDOCS_files::read_file(filename);
    }
    
    // Case 2: Anchor provided and title=true - return section between anchor and next header (including current header)
    if (!anchor.empty() && title == true) {
        std::vector<std::string> file_lines = CDOCS_files::read_file(filename);
        int line_count = 0;
        bool in_section = false;
        
        for (const std::string& line : file_lines) {
            if (line.size() < 100) {
                // Check if we found the anchor
                if (line.find(final_anchor) != std::string::npos) {
                    in_section = true;
                    break_points.push_back(line_count);
                    continue;
                }
                
                // Check if we found a header while in a section
                if (in_section && std::regex_search(line, match, header)) {
                    break;  // End of section
                }
                
                // If we're in a section, add the line index
                if (in_section) {
                    break_points.push_back(line_count);
                }
            }
            line_count++;
        }
        
        // Now extract the lines we marked
        for (int idx : break_points) {
            if (idx >= 0 && idx < file_lines.size()) {
                out.push_back(file_lines[idx]);
            }
        }
    }
    
    // Case 3: No anchor and title=false - skip first header, return rest
    if (anchor.empty() && title == false) {
        std::vector<std::string> file_lines = CDOCS_files::read_file(filename);
        bool first_header_found = false;
        
        for (const std::string& line : file_lines) {
            if (!first_header_found && std::regex_search(line, match, header)) {
                first_header_found = true;
                continue;  // Skip the first header
            }
            out.push_back(line);
        }
    }
    
    // Case 4: Anchor provided and title=false - find section but skip its header
    if (!anchor.empty() && title == false) {
        std::vector<std::string> file_lines = CDOCS_files::read_file(filename);
        bool in_section = false;
        bool section_header_skipped = false;
        
        for (const std::string& line : file_lines) {
            if (line.size() < 100) {
                // Check if we found the anchor
                if (line.find(final_anchor) != std::string::npos) {
                    in_section = true;
                    continue;  // Skip the anchor line itself
                }
                
                // If we're in section, check for header
                if (in_section) {
                    // Skip the first header in this section
                    if (!section_header_skipped && std::regex_search(line, match, header)) {
                        section_header_skipped = true;
                        continue;
                    }
                    
                    // Stop at next section header
                    if (section_header_skipped && std::regex_search(line, match, header)) {
                        break;
                    }
                    
                    // Add content lines
                    if (section_header_skipped) {
                        out.push_back(line);
                    }
                }
            }
        }
    }
    
    return out;
}
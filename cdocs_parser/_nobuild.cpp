// Check for inline variable: {{...}} with max 100 chars between
        size_t var_start = line.find("{{");
        if (var_start != string::npos) {
            size_t var_end = line.find("}}", var_start);
            if (var_end != string::npos && (var_end - var_start - 2) <= 100) {
                //cout << "Found inline var" << endl;
            }
        }
        
        // Check for inline condition: {% if ... {%endif%}} on same line
        size_t if_start = line.find("{% if");
        if (if_start != string::npos) {
            size_t endif_start = line.find("{%endif%}");
            if (endif_start != string::npos && endif_start > if_start) {
                //cout << "Found inline condition" << endl;
            }
            // Also check for {% endif %}
            endif_start = line.find("{% endif %}");
            if (endif_start != string::npos && endif_start > if_start) {
                //cout << "Found inline condition" << endl;
            }
        }
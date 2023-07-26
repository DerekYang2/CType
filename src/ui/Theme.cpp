#include "Theme.h"

const string THEMES_FOLDER = "themes";
unordered_map<string, Theme> theme_map;

Theme::Theme() : background(BLACK), main(BLACK), caret(BLACK), sub(BLACK), text(BLACK), error(BLACK), error_extra(BLACK) {}

Theme::Theme(Color background, Color main, Color caret, Color sub, Color sub_alt, Color text, Color error, Color error_extra): background(background), main(main), caret(caret), sub(sub), sub_alt(sub_alt), text(text), error(error), error_extra(error_extra)
{
}


/**
 * For now, only meant for fetching specific css variables used by monkeytype 
*/
void fetch_themes()
{
    namespace fs = std::filesystem;
    for (const auto& file : fs::directory_iterator(THEMES_FOLDER))
    {
        if (file.is_directory()) continue;
        string filepath = file.path().string();
        string extension = file.path().extension().string();
        string name = file.path().stem().string();
        // Replace all '_' with ' ' 
        replace(name.begin(), name.end(), '_', ' ');
        if (extension == ".css")
        {
            std::ifstream file_stream(filepath);
            std::stringstream buffer;
            buffer << file_stream.rdbuf();
            string line;
            unordered_map<string, Color> color_map;
            // Proccess file
            while (getline(buffer, line))
            {
                // erase tabs and spaces
                line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
                line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
                // Skip empty lines and lines that don't contain ':'
                if (line.empty() || line.find(':') == std::string::npos)
                {
                    continue;
                }
                // Split line by ':'
                vector<string> halves = split_string(line, ':');
                if (halves.size() != 2) continue;
                // first half (name) 
                if (halves[0].find("--") == std::string::npos) continue;
                int dash_idx = halves[0].find("--");
                std::string name = halves[0].substr(dash_idx + 2);
                // Second half (value)
                if (halves[1].empty()) continue;
                std::string value = halves[1];
                value.erase(std::remove(value.begin(), value.end(), '#'), value.end());
                value.erase(std::remove(value.begin(), value.end(), ';'), value.end());
                // If valid hex string
                if (std::all_of(value.begin(), value.end(), ::isxdigit))
                {
                    Color col = hexToColor(value);
                    color_map[name] = col;
                }
            }
            if (color_map.contains("bg-color") && color_map.contains("main-color") && color_map.contains("caret-color") && color_map.contains("sub-color") && color_map.contains("sub-alt-color") && color_map.contains("text-color") && color_map.contains("error-color") && color_map.contains("error-extra-color"))
            {
                theme_map[name] = Theme(color_map["bg-color"], color_map["main-color"], color_map["caret-color"], color_map["sub-color"], color_map["sub-alt-color"], color_map["text-color"], color_map["error-color"], color_map["error-extra-color"]);
            }
        }
    }
}

void init_theme(string theme_name)
{
    if (theme_map.contains(theme_name))
        theme = theme_map[theme_name];
    else
    {
        theme = Theme(
            rgb(36, 41, 51), // background
            rgb(217, 222, 233), // main
            rgb(217, 222, 233), // caret
            rgb(118, 139, 165), // sub 
            rgb(27, 31, 39), // sub-alt
            rgb(217, 222, 233), // text 
            rgb(179, 93, 105), // error
            rgb(117, 61, 69) // error_extra
        );
    }
}
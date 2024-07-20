#include "Theme.h"
#include <filesystem>
#include "jsonparser.h"

const string THEMES_FOLDER = "themes";
const string THEMES_JSON = THEMES_FOLDER + "/_themes.json";
unordered_map<string, Theme> theme_map;

Theme::Theme() : background(BLACK), main(BLACK), caret(BLACK), sub(BLACK), text(BLACK), error(BLACK), error_extra(BLACK) {}

Theme::Theme(Color background, Color main, Color caret, Color sub, Color sub_alt, Color text, Color error, Color error_extra): background(background), main(main), caret(caret), sub(sub), sub_alt(sub_alt), text(text), error(error), error_extra(error_extra)
{
}


/**
 * For now, only meant for fetching specific css variables used by monkeytype
 * TODO: check if themes folder includes new themes
*/
void fetch_themes()
{
    if (!FileExists(THEMES_JSON.c_str()))
    {
        refresh_themes();
    }
    // read 
    RSJresource themes_json(readFile(THEMES_JSON));
    for (auto& [theme_name, json_obj] : themes_json.as_object())
    {
        theme_map[theme_name] = Theme(hexToColor(json_obj["bg-color"].as<string>()), hexToColor(json_obj["main-color"].as<string>()), hexToColor(json_obj["caret-color"].as<string>()), hexToColor(json_obj["sub-color"].as<string>()), hexToColor(json_obj["sub-alt-color"].as<string>()), hexToColor(json_obj["text-color"].as<string>()), hexToColor(json_obj["error-color"].as<string>()), hexToColor(json_obj["error-extra-color"].as<string>()));
    }
}

void refresh_themes()
{
    namespace fs = std::filesystem;
    RSJresource themes_json("{}");
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
            unordered_map<string, string> object_map;
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
                if (halves[0].empty() || halves[0].find("--") == std::string::npos) continue;
                int dash_idx = halves[0].find("--");
                std::string name = halves[0].substr(dash_idx + 2);
                // Second half (value)
                if (halves[1].empty() || halves[1].find('#') == std::string::npos || halves[1].find(';') == std::string::npos) continue;
                // Substring from # to ;
                int hashtag_idx = halves[1].find('#'), semicolon_idx = halves[1].find(';', hashtag_idx);
                std::string value = substrE(halves[1], hashtag_idx+1, semicolon_idx);
                
                // If valid hex string
                if (std::all_of(value.begin(), value.end(), ::isxdigit))
                {
                    object_map[name] = value;
                }
            }
            if (object_map.contains("bg-color") && object_map.contains("main-color") && object_map.contains("caret-color") && object_map.contains("sub-color") && object_map.contains("sub-alt-color") && object_map.contains("text-color") && object_map.contains("error-color") && object_map.contains("error-extra-color"))
            {
                themes_json[name] = RSJresource("{}");
                themes_json[name]["bg-color"] = object_map["bg-color"];
                themes_json[name]["main-color"] = object_map["main-color"];
                themes_json[name]["caret-color"] = object_map["caret-color"];
                themes_json[name]["sub-color"] = object_map["sub-color"];
                themes_json[name]["sub-alt-color"] = object_map["sub-alt-color"];
                themes_json[name]["text-color"] = object_map["text-color"];
                themes_json[name]["error-color"] = object_map["error-color"];
                themes_json[name]["error-extra-color"] = object_map["error-extra-color"];
            } else
            {
                cout << "ERROR loading theme: " << name << endl;
            }
        }
    }
    writeText(THEMES_JSON, themes_json.as_str());
}

void init_theme(string theme_name)
{
    replace(theme_name.begin(), theme_name.end(), '_', ' ');
    if (theme_map.contains(theme_name))
        theme = theme_map[theme_name];
    else  // TODO: should this be changed to a different default theme?
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
#include "Settings.h"

const float SETTING_PADDING = 100;
string setting_path = "./data/settings.json";
RSJresource setting_json;

void init_settings()
{
    RSJresource default_json(
    R"(
    { 
        'general' : {
            'show wpm' : { options: ['off', 'on'], default: 1 }, 
            'strict space' : { options: ['off', 'on'], default: 0 }, 
            'tape mode' : { options: ['off', 'on'], default: 0 }, 
            'debug mode' : { options: ['off', 'on'], default: 0 } 
        }, 
        'appearance' : {
            'theme' : 'vscode' 
        }
    }  
    )");
    // check if settings file exists
    if (!FileExists(setting_path.c_str()))
    {
        string setting_str = default_json.as_str();
        replace(setting_str.begin(), setting_str.end(), '\'', '"');
        writeText(setting_path, setting_str);
    }

    setting_json = RSJresource(readFile(setting_path));

    // Fill any missing parameters from default
    // Fill in sections
    auto setting_map = setting_json.as_map<string>();
    for (auto& [key, value] : default_json.as_map<string>())
    {
        if (!setting_map.contains(key))
        {
            setting_json[key] = value;
        }
    }
    // Fill in each section
    for (auto& [section, section_content] : default_json.as_object())
    {
        auto section_map = setting_json[section].as_map<string>();
        for (auto& [key, value] : section_content.as_map<string>())
        {
            if (!section_map.contains(key))
            {
                setting_json[section][key] = value;
            }
        }
    }
    float setting_h = 20;

    vector<ToggleGroup*> toggle_pointers;  // issue toggle pointers not in order because of unordered map
    for (auto& [key, value] : setting_json["general"].as_object())
    {
        vector<string> options;
        for (auto& str : value["options"].as_array())
            options.push_back(str.as<string>());
        setting_toggle[key] = new ToggleGroup(0, 0, setting_h, value["default"].as<int>(), options, true);
        ui_objects.alloc(setting_toggle[key], SETTINGS);
    }

    vector<string> general_toggle_ordered = default_json["general"].get_keys();
    for (auto& str : general_toggle_ordered)
    {
        toggle_pointers.push_back(setting_toggle[str]);
    }

    Rectangle boundary{SETTING_PADDING, SETTING_PADDING, gameScreenWidth - 2 * SETTING_PADDING, gameScreenHeight - 2 * SETTING_PADDING};  // Mouse will be registered 
    float y_pos = SETTING_PADDING;

    behavior_panel = new TogglePanel(SETTING_PADDING, y_pos, gameScreenWidth - 2 * SETTING_PADDING, toggle_pointers, {
        {"Show Live WPM", "Displays the live WPM on the test screen."},
        {"Strict Space", "When enabled, pressing space at the beginning of a word will insert a space character."},
        {"Tape Mode", "Only shows one line which scrolls horizontally."},
        {"Debug Mode", "Allows debugging functions."}
    });
    behavior_panel->set_bounds(boundary);

    // Initialize themes
    fetch_themes();
    theme_toggle = new ThemeToggle(SETTING_PADDING, y_pos, gameScreenWidth - 2 * SETTING_PADDING, 40, setting_json["appearance"]["theme"].as<string>());
    theme_toggle->set_bounds(boundary);
    init_theme(theme_toggle->get_selected());

    for (UIObject* obj : list<UIObject*>{ behavior_panel, theme_toggle })
    {
        obj->set_pos(SETTING_PADDING, y_pos);
        y_pos += obj->get_height();
        ui_objects.alloc(obj, SETTINGS);
    }
}

void write_settings()
{
    // update settings json general toggles
    for (auto &[label, toggle] : setting_toggle)
    {
        setting_json["general"][label]["default"] = toggle->selected_index();
    }
    setting_json["appearance"]["theme"] = RSJresource("'" + theme_toggle->get_selected() + "'");
    // update settings file
    // replace single quote with double quote 
    string setting_str = setting_json.as_str();
    replace(setting_str.begin(), setting_str.end(), '\'', '"');
    writeText(setting_path, setting_str);
}

void update_settings()
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        switch_start();
    }
}

void draw_settings()
{
    ClearBackground(theme.background);  // Clear render texture background color
}

// Draw padding and taskbar on top
void draw_borders()
{
    DrawRectangle(0, 0, gameScreenWidth, SETTING_PADDING, theme.background);
    DrawRectangleAlign(0, gameScreenHeight, gameScreenWidth, SETTING_PADDING, theme.background, LEFT, BOTTOM);
    draw_taskbar();
}
#include "Settings.h"

const float SETTING_PADDING = 100;
string setting_path = "./data/settings.json";
RSJresource setting_json;

ThemeToggle* theme_toggle;

void init_settings()
{
    RSJresource default_json(
    "{ \n"
    "    'show wpm' : { options: ['off', 'on'], default: 1 }, \n"
    "    'strict space' : { options: ['off', 'on'], default: 0 }, \n"
    "    'tape mode' : { options: ['off', 'on'], default: 0 }, \n"
    "    'debug mode' : { options: ['off', 'on'], default: 0 } \n"
    "}  \n");

    vector<string> key_ordered = default_json.get_keys(true);

    // check if settings file exists
    if (!FileExists(setting_path.c_str()))
    {
        writeText(setting_path, default_json.as_str());
    }

    setting_json = RSJresource(readFile(setting_path));
    float setting_h = 20;

    vector<ToggleGroup*> toggle_pointers;  // issue toggle pointers not in order because of unordered map
    for (auto& [key, value] : setting_json.as_object())
    {
        vector<string> options;
        for (auto &str : value["options"].as_array())
            options.push_back(str.as<string>());
        setting_toggle[key] = new ToggleGroup(0, 0, setting_h, value["default"].as<int>(), options, true);
        ui_objects.alloc(setting_toggle[key], SETTINGS);
    }

    for (auto& str : key_ordered)
        toggle_pointers.push_back(setting_toggle[str]);

    float y_pos = SETTING_PADDING;
    
    behavior_panel = new TogglePanel(SETTING_PADDING, y_pos, gameScreenWidth - 2 * SETTING_PADDING, toggle_pointers, {
        {"Show Live WPM", "Displays the live WPM on the test screen."},
        {"Strict Space", "When enabled, pressing space at the beginning of a word will insert a space character."},
        {"Tape Mode", "Only shows one line which scrolls horizontally."},
        {"Debug Mode", "Allows debugging functions."}
    });
    
    // theme toggle TODO: add TogglePanel height
    theme_toggle = new ThemeToggle(SETTING_PADDING, y_pos, gameScreenWidth - 2 * SETTING_PADDING, 40, "stealth");

    for (UIObject* obj : list<UIObject*>{ behavior_panel, theme_toggle })
    {
        obj->set_pos(SETTING_PADDING, y_pos);
        y_pos += obj->get_height();
        ui_objects.alloc(obj, SETTINGS);
    }
}

void write_settings()
{
    // update settings json object
    for (auto &[label, toggle] : setting_toggle)
    {
        setting_json[label]["default"] = toggle->selected_index();
    }
    // update settings file
    writeText(setting_path, setting_json.as_str());
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
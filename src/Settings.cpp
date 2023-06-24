#include "Settings.h"

#define SETTING_PADDING 200
string setting_path = "./data/settings.json";
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

    RSJresource setting_json(readFile(setting_path));


    float setting_h = 30;

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

    behavior_panel = new TogglePanel(SETTING_PADDING, 200, gameScreenWidth - 2 * SETTING_PADDING, toggle_pointers, {
        {"Show Live WPM", "Displays the live WPM on the test screen."},
        {"Strict Space", "When enabled, pressing space at the beginning of a word will insert a space character."},
        {"Tape Mode", "Only shows one line which scrolls horizontally."},
        {"Debug Mode", "Allows debugging functions."}
    });
    ui_objects.alloc(behavior_panel, SETTINGS);
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
    Rectangle taskbar_rect = taskbar->bounding_box();
    taskbar_rect.height *= 2;  // goes of screen
    DrawRectangleRounded(taskbar_rect, 0.1f, 10, theme.background_shade);
}
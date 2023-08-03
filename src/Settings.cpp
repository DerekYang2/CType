#include "Settings.h"

const float MENU_HEIGHT = 50;
const float SETTING_PADDING = 100;
string setting_path = "./data/settings.json";
RSJresource setting_json;
Scrollbar* scrollbar;
Rectangle boundary;
unordered_map<string, Button*> menu_button;
unordered_map<string, Textbox*> heading_text;
unordered_map<string, vector<UIObject*>> heading_objects;

/**
 * TODO: fix menu button, maybe center
*/

void init_settings()
{
    RSJresource default_json(
    R"(
    { 
        'behavior' : {
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
    auto setting_map = setting_json.as_map<string>();
    for (auto& [key, value] : default_json.as_map<string>())  // Fill in sections
    {
        if (!setting_map.contains(key))
        {
            setting_json[key] = value;
        }
    }
    for (auto& [section, section_content] : default_json.as_object())  // Fill in each section
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
    float setting_h = 30;

    vector<ToggleGroup*> toggle_pointers;  // issue toggle pointers not in order because of unordered map
    for (auto& [key, value] : setting_json["behavior"].as_object())
    {
        vector<string> options;
        for (auto& str : value["options"].as_array())
            options.push_back(str.as<string>());
        setting_toggle[key] = new ToggleGroup(0, 0, setting_h, value["default"].as<int>(), options, true);
        ui_objects.alloc(setting_toggle[key], SETTINGS);
    }

    vector<string> behavior_toggle_ordered = default_json["behavior"].get_keys();
    for (auto& str : behavior_toggle_ordered)
    {
        toggle_pointers.push_back(setting_toggle[str]);
    }

    // Initialise UI ---------------------------------------------------------------------------
    boundary = Rectangle(SETTING_PADDING, SETTING_PADDING + MENU_HEIGHT, gameScreenWidth - 2 * SETTING_PADDING, gameScreenHeight - 2 * SETTING_PADDING - MENU_HEIGHT);  // Mouse will be registered 
    
    // Menu buttons
    list<string> headings_list{ "Behavior", "Appearance" };
    float x_pos = SETTING_PADDING;
    for (string heading : headings_list)
    {
        menu_button[heading] = new Button(x_pos, SETTING_PADDING, MENU_HEIGHT - 10, heading);
        x_pos += menu_button[heading]->get_width();
        ui_objects.alloc(menu_button[heading], SETTINGS);
    }
    
    // APPEARANCE OBJECTS: Initialize themes first
    fetch_themes();
    theme_toggle = new ThemeToggle(SETTING_PADDING, 0, gameScreenWidth - 2 * SETTING_PADDING, 40, setting_json["appearance"]["theme"].as<string>());
    theme_toggle->set_bounds(boundary);
    init_theme(theme_toggle->get_selected());
    heading_objects["Appearance"].push_back(theme_toggle);
    
    // Titles (heading)
    for (string heading : headings_list)
    {
        heading_text[heading] = new Textbox(SETTING_PADDING, 0, gameScreenWidth - 2 * SETTING_PADDING, 50, "\n" + heading, 45, "main", true);
    }

    // BEHAVIOR objects 
    behavior_panel = new TogglePanel(SETTING_PADDING, 0, gameScreenWidth - 2 * SETTING_PADDING, toggle_pointers, {
        {"Show Live WPM", "Displays the live WPM on the test screen."},
        {"Strict Space", "When enbled, pressing space at the beginning of a word will insert a space character."},
        {"Tape Mode", "Only shows one line which scrolls horizontally."},
        {"Debug Mode", "Allows debugging functions."}
    });
    behavior_panel->set_bounds(boundary);
    heading_objects["Behavior"].push_back(behavior_panel);

    // Set all UIObject positions
    float y_pos = boundary.y;

    for (string heading : headings_list)
    {
        // Heading title
        UIObject* title_obj = heading_text[heading];
        title_obj->set_pos(boundary.x, y_pos);
        y_pos += title_obj->get_height();
        ui_objects.alloc(title_obj, SETTINGS);
        // Objects under heading 
        for (auto obj : heading_objects[heading])
        {
            obj->set_pos(boundary.x, y_pos);
            y_pos += obj->get_height();
            // Add obj to ui object set
            ui_objects.alloc(obj, SETTINGS);
        }
    }

    // Set all menu button functions
    for (string heading : headings_list)
    {
        float new_offset = -(heading_text[heading]->get_rect().y - boundary.y);
        menu_button[heading]->attach_trigger([=] {
            // Set scrollbar offset to title position
            float cur_offset = scrollbar->get_offset();
            scrollbar->shift(0, cur_offset - new_offset);
        });
    }

    // Initialise scrollbar 
    float total_h = y_pos + SETTING_PADDING;  // Extra setting padding at bottom
    constexpr float bar_w = 10;
    scrollbar = new Scrollbar(gameScreenWidth - bar_w / 3, SETTING_PADDING, bar_w, gameScreenHeight - SETTING_PADDING , gameScreenHeight, total_h);
    // Bind all setting objects to scrollbar
    for (string heading : headings_list)
    {
        scrollbar->add_child(heading_text[heading]);
        for (auto obj : heading_objects[heading])
        {
            scrollbar->add_child(obj);
        }
    }
    ui_objects.alloc(scrollbar, SETTINGS);
}

void write_settings()
{
    // update settings json behavior toggles
    for (auto &[label, toggle] : setting_toggle)
    {
        setting_json["behavior"][label]["default"] = toggle->selected_index();
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
    scrollbar->update();
}

void draw_settings()
{
    ClearBackground(theme.background);  // Clear render texture background color
}

// Draw padding and taskbar on top
void draw_borders()
{
    // Draw rectangle above boundary
    DrawRectangle(0, 0, gameScreenWidth, boundary.y, theme.background);
    // Draw rectangle below boundary
    DrawRectangleAlign(0, gameScreenHeight, gameScreenWidth, gameScreenHeight - (boundary.y + boundary.height), theme.background, LEFT, BOTTOM);
    // Drawing left right padding blocks out buttons that expand a bit
    //DrawRectangle(0, 0, SETTING_PADDING, gameScreenHeight, theme.background);
    //DrawRectangleAlign(gameScreenWidth, 0, SETTING_PADDING, gameScreenHeight, theme.background, RIGHT, TOP);
    for (auto& [heading, button] : menu_button)
    {
        button->draw();
    }
    draw_taskbar();
    scrollbar->draw();
}
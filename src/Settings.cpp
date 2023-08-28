#include "Settings.h"
#include "HorizontalGroup.h"

const float MENU_HEIGHT = 50;
const float SETTING_PADDING = 100;
string setting_path = "data/settings.json";
RSJresource setting_json;
Scrollbar* scrollbar;
Rectangle boundary;
HorizontalGroup* menu_group;
FontToggle* font_toggle;
SoundToggle* sound_toggle;

unordered_map<string, string> section_name;
unordered_map<string, Button*> menu_button;
unordered_map<string, Textbox*> heading_text;
unordered_map<string, vector<UIObject*>> heading_objects;
// Toggle descriptions
unordered_map<string, vector<pair<string, string>>> toggle_descriptions({
    { "Behavior",
        {
            {"Quick Restart", "Allows restarting the test by pressing 'tab'."},
            {"Debug Mode", "Allows debugging functions."}
        }
    },
    { "Input",
        {
            {"Strict Space", "When enabled, pressing space at the beginning of a word will insert a space character."},
            {"Replace Typos", "Typos will override the correct character instead of just displaying the correct character with an error color."}
        }
    },
    { "Appearance",
        {
            {"Show WPM",  "Displays the live WPM on the test screen."},
            {"Tape Mode", "Only shows one line which scrolls horizontally."}
        }
    }
});

void init_settings()
{
    RSJresource default_json(
    R"(
    { 
        'Behavior' : {
            'quick restart': { options: ['off', 'on'], default: 1 },
            'debug mode' : { options: ['off', 'on'], default: 0 }
        }, 
        'Input': {
            'strict space' : { options: ['off', 'on'], default: 0 }, 
            'replace typos': {options: ['off', 'on'], default: 0}
        },
        'Appearance' : {
            'show wpm' : { options: ['off', 'on'], default: 1 },
            'tape mode' : { options: ['off', 'on'], default: 0 }
        },
        'Theme' : {
            'name' : 'serika dark'
        },
        'Font' : {
            'path' : 'fonts/RobotoMono.ttf'
        },
        'Sound' : {
            'path' : 'nk_creams'
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
    

    // Initialise UI ---------------------------------------------------------------------------
    boundary = Rectangle(SETTING_PADDING, SETTING_PADDING + MENU_HEIGHT, gameScreenWidth - 2 * SETTING_PADDING, gameScreenHeight - 2 * SETTING_PADDING - MENU_HEIGHT);  // Mouse will be registered 
       
    // FONT/THEME OBJECTS: Initialize themes and fonts first as they are required to initializes other objects
    // Font init
    init_font(setting_json["Font"]["path"].as<string>());  // INTIALIZE GLOBAL FONT 
    font_toggle = new FontToggle(SETTING_PADDING, 0, gameScreenWidth - 2 * SETTING_PADDING, 40, font_path);
    font_toggle->set_bounds(boundary);
    heading_objects["Font"].push_back(font_toggle);
    // Theme init
    fetch_themes();
    theme_toggle = new ThemeToggle(SETTING_PADDING, 0, gameScreenWidth - 2 * SETTING_PADDING, 40, setting_json["Theme"]["name"].as<string>());
    theme_toggle->set_bounds(boundary);
    init_theme(theme_toggle->get_selected());  // INTIALIZE GLOBAL THEME
    heading_objects["Theme"].push_back(theme_toggle);
    
    // Menu buttons -> MOVE DOWN, FONTS FIRST
    list<string> headings_list{ "Behavior", "Input", "Appearance", "Theme", "Font", "Sound" };
    float x_pos = SETTING_PADDING;
    vector<UIObject*> button_list;
    for (string heading : headings_list)
    {
        menu_button[heading] = new Button(x_pos, SETTING_PADDING, MENU_HEIGHT - 6, heading, NULL, "jump to " + heading);
        x_pos += menu_button[heading]->get_width();
        button_list.push_back(menu_button[heading]);
    }
    menu_group = new HorizontalGroup(gameScreenWidth * 0.5f, SETTING_PADDING, MENU_HEIGHT/2, button_list, true);
    ui_objects.alloc(menu_group, SETTINGS);

    // Titles (heading)
    for (string heading : headings_list)
    {
        heading_text[heading] = new Textbox(SETTING_PADDING, 0, gameScreenWidth - 2 * SETTING_PADDING, 50, "\n" + heading, font_measure.title(), "main", true);
    }

    // Sounds
    sound_toggle = new SoundToggle(SETTING_PADDING, 0, gameScreenWidth - 2 * SETTING_PADDING, 40, setting_json["Sound"]["path"].as<string>());
    load_sound_effects(SOUNDS_FOLDER + "/" + sound_toggle->get_selected());
    sound_toggle->set_bounds(boundary);
    heading_objects["Sound"].push_back(sound_toggle);

    // Generic toggle objects 
    for (string SETTING_SECTION : {"Behavior", "Input", "Appearance"})
    {
        vector<ToggleGroup*> toggle_pointers;
        for (auto& [key, value] : setting_json[SETTING_SECTION].as_object())
        {
            // INIT ToggleGroup
            vector<string> options;
            for (auto& str : value["options"].as_array())
                options.push_back(str.as<string>());
            setting_toggle[key] = new ToggleGroup(0, 0, font_measure.medium_height, value["default"].as<int>(), options, true);
            ui_objects.alloc(setting_toggle[key], SETTINGS);
            section_name[key] = SETTING_SECTION;
        }
        // Init display panel
        vector<string> toggles_ordered = default_json[SETTING_SECTION].get_keys();
        for (auto& str : toggles_ordered)
        {
            toggle_pointers.push_back(setting_toggle[str]);
        }
        TogglePanel* display_panel = new TogglePanel(SETTING_PADDING, 0, gameScreenWidth - 2 * SETTING_PADDING, toggle_pointers, toggle_descriptions[SETTING_SECTION]);
        display_panel->set_bounds(boundary);
        heading_objects[SETTING_SECTION].push_back(display_panel);
    }
    
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
    // Update settings json behavior toggles
    for (auto &[label, toggle] : setting_toggle)
    {
        setting_json[section_name[label]][label]["default"] = toggle->selected_index();
    }
    
    setting_json["Theme"]["name"] = RSJresource("'" + theme_toggle->get_selected() + "'");
    setting_json["Font"]["path"] = RSJresource("'" + font_toggle->get_selected() + "'");
    setting_json["Sound"]["path"] = RSJresource("'" + sound_toggle->get_selected() + "'");
    // Update settings file
    // Replace single quote with double quote 
    string setting_str = setting_json.as_str();
    replace(setting_str.begin(), setting_str.end(), '\'', '"');
    writeText(setting_path, setting_str);
}

void update_settings()
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        repeat_test = true;
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
    menu_group->draw();
    draw_taskbar();
    scrollbar->draw();
    menu_group->draw_hint();
}
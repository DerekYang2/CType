#pragma once
#include "globals.h"
#include "UIAlloc.h"
#include "TogglePanel.h"
#include "jsonparser.h"
#include "ThemeToggle.h"
#include "Button.h"
#include "Textbox.h"
#include "Scrollbar.h"
#include "VerticalToggle.h"
// EXTERNALS USED
extern UIAlloc ui_objects;
extern void switch_start();
extern void draw_taskbar();

// settings.json object
extern RSJresource setting_json; 

// variables
extern const float SETTING_PADDING;
extern Scrollbar* scrollbar;
extern TogglePanel* behavior_panel;
extern ToggleGroup* taskbar;
extern ThemeToggle * theme_toggle;
extern unordered_map<string, ToggleGroup*> setting_toggle;
extern string default_settings;
extern string setting_path;
extern Rectangle boundary;
extern unordered_map<string, Button*> menu_button;
extern unordered_map<string, Textbox*> heading_text;
extern unordered_map<string, vector<UIObject*>> heading_objects;

// custom macros
#define is_strict_space (setting_toggle["strict space"]->get_selected() == "on")
#define is_tape_mode (setting_toggle["tape mode"]->get_selected() == "on")
extern void init_settings();
extern void write_settings();  // write settings.json to file path
extern void update_settings();
extern void draw_settings();
extern void draw_borders();
#pragma once
#include "globals.h"
#include "UIAlloc.h"
#include "TogglePanel.h"
#include "jsonparser.h"
// EXTERNALS
extern UIAlloc ui_objects;
extern void switch_start();
extern void draw_taskbar();

// settings.json object
extern RSJresource setting_json;


// variables
extern TogglePanel* behavior_panel;
extern ToggleGroup* taskbar;
extern unordered_map<string, ToggleGroup*> setting_toggle;
extern string default_settings;
extern string setting_path;

// custom macros
#define is_strict_space (setting_toggle["strict space"]->get_selected() == "on")
#define is_tape_mode (setting_toggle["tape mode"]->get_selected() == "on")
extern void init_settings();
extern void write_settings();  // write settings.json to file path
extern void update_settings();
extern void draw_settings();
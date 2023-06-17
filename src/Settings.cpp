#include "Settings.h"

#define SETTING_PADDING 200

// variables 
TogglePanel* behavior_panel;
ToggleGroup
*show_wpm,
*strict_space;

void init_settings()
{
    
    float setting_h = 30;
    
    show_wpm = new ToggleGroup(0, 0, setting_h, 0, { "on", "off" });
    ui_objects.alloc(show_wpm, SETTINGS);
    
    strict_space = new ToggleGroup(0, 0, setting_h, 1, { "on", "off" });
    ui_objects.alloc(strict_space, SETTINGS);
    
    behavior_panel = new TogglePanel(SETTING_PADDING, 200, gameScreenWidth - 2 * SETTING_PADDING, { show_wpm, strict_space}, {
        {"Show Live WPM", "Displays the live WPM on the test screen."},
        {"Strict Space", "When disabled, pressing space will jump to the next word."}
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
    DrawText("SETTINGS", 0, 0, 20, BLACK);
}
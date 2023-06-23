#include "Settings.h"

#define SETTING_PADDING 200

void init_settings()
{
    
    float setting_h = 30;
    
    show_wpm = new ToggleGroup(0, 0, setting_h, 1, { "off", "on" }, true);
    ui_objects.alloc(show_wpm, SETTINGS);
    
    strict_space = new ToggleGroup(0, 0, setting_h, 0, { "off", "on" }, true);
    ui_objects.alloc(strict_space, SETTINGS);

    tape_mode = new ToggleGroup(0, 0, setting_h, 0, { "off", "on" }, true);
    ui_objects.alloc(tape_mode, SETTINGS);

    debug_mode = new ToggleGroup(0, 0, setting_h, 0, { "off", "on" }, true);
    ui_objects.alloc(debug_mode, SETTINGS);

    behavior_panel = new TogglePanel(SETTING_PADDING, 200, gameScreenWidth - 2 * SETTING_PADDING, { show_wpm, strict_space, tape_mode, debug_mode }, {
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
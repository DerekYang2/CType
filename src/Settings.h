#include "globals.h"
#include "UIAlloc.h"
#include "TogglePanel.h"

// EXTERNALS
extern UIAlloc ui_objects;
extern void switch_start();

// variables
extern TogglePanel* behavior_panel;

extern ToggleGroup
*show_wpm,
*strict_space;

// custom macros
#define is_strict_space (strict_space->selected() == "on")

extern void init_settings();
extern void update_settings();
extern void draw_settings();
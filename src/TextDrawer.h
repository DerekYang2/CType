#pragma once
#include "globals.h"
#include "Settings.h"
#include "WpmLogger.h"

class TextDrawer {
    // non-tape mode variables
    deque<int> newlines;  // newline indices
    Vector2 cursor_pos;   // for moving cursor
    Vector2 cursor_target;  // used for smooth cursor
    Font font;
    int font_size;
    float spacing;
    float offset;
    float center;
    float bottom_y;
    float top_y;
    float cursor_h;
    float padding;
    // offset variables
    float offset_x = 0;
    float offset_vel = 0;
    float vel_target = 0;
    public:
    TextDrawer();
    TextDrawer(string font_path, float fontSize, float spacing = 15);
    TextDrawer(Font draw_font, float fontSize, float spacing = 15);
    int next_foldpoint();
    void draw();
    void draw_time(string time_text, string wpm_text, float time_percent, float dash_percent, bool left_align);
    void draw_caret();
    void add_offset(float x);
    float get_top_y();
};

extern TextDrawer drawer;
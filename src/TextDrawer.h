#pragma once
#include "globals.h"
#include "Settings.h"

struct TextDrawer {
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
    TextDrawer();
    TextDrawer(string font_path, float fontSize, float spacing = 15);
    TextDrawer(Font draw_font, float fontSize, float spacing = 15);
    void set_offset(float x);
    int next_foldpoint();
    void draw();
    void draw_caret();
    float get_top_y();
};

extern TextDrawer drawer;
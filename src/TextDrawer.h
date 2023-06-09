#pragma once
#include "globals.h"
struct TextDrawer {
    Font font;
    int font_size;
    float spacing;
    float offset;
    float center;
    float bottom_y;
    float top_y;
    float cursor_h;
    TextDrawer();
    TextDrawer(string font_path, float fontSize, float spacing = 15);
    TextDrawer(Font draw_font, float fontSize, float spacing = 15);
    void set_offset(float x);
    void draw();
    void draw_cursor();
    float get_top_y();
};

extern TextDrawer drawer;
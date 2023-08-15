#pragma once
#include "UIObject.h"
#include "globals.h"

class Textbox : public UIObject {
    Vector2 corner_init;  // Initial corner 
    string text;
    string og_text;
    float font_size;  // desired font size, will be overriden if wrapping off
    Color color;
    string theme_color = "";
    Rectangle rect;
    bool wrapping;
    string hint = "";  
    float hint_alpha = 0;
    
    string fold(string str, float max_width);
    public:
    // If wrapping = true, text will stay as fontSize, otherwise fontSize will be overriden to fit within width and height
    Textbox(float x, float y, float width, float height, string text_str, float fontSize, Color col, bool wrapping = false, string hintStr = "");
    Textbox(float x, float y, float width, float height, string text_str, float fontSize, string themeColor, bool wrapping = false, string hintStr = "");
    Textbox(float x, float y, float width, string text, Color col);   // specific scaling text constructor
    void update() override;
    void draw() override;
    void draw_hint() override;
    void set_pos(float x, float y) override;
    void set_offset(float dx, float dy) override;
    void set_var_str(vector<string> str);
    Rectangle get_rect();
    float get_width();
    float get_height();
};
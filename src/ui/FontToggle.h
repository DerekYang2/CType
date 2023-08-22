#pragma once
#include "ToggleGroup.h"

/**
 * TODO: image preview cache
*/

extern const int FONT_PER_ROW;
extern void init_font(string font_path);

class FontToggle : public ToggleGroup {
    vector<string> display_text;
    vector<Font> fonts;
    public:
    FontToggle(float x, float y, float w, float h, string init_theme);
    void update() override;
    void draw() override;
    Rectangle bounding_box() override;
    float get_width() override;
    float get_height() override;
    string get_selected() override;
};
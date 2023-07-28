#pragma once
#include "globals.h"
#include "ToggleGroup.h"

class TogglePanel : public UIObject {
    Rectangle bounds{ 0, 0, gameScreenWidth, gameScreenHeight };
    Vector2 corner_init;
    float x, y, width, toggle_h, gap;
    float total_h;
    // font_info
    float font_size;
    vector<pair<float, float>> text_height;
    vector<ToggleGroup*> toggles;
    vector<pair<string, string>> toggle_info;  // label, details

    public:
    TogglePanel(float x, float y, float w, vector<ToggleGroup*> toggles, vector<pair<string, string>> toggle_info);
    void update() override;
    void draw() override;
    void set_pos(float x2, float y2) override;
    void set_offset(float dx, float dy) override;
    void set_bounds(Rectangle rect);
    string selected(int idx);
    float get_height() override;
    private:
    string fold(string str, float max_width);
};

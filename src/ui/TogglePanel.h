#pragma once
#include "globals.h"
#include "ToggleGroup.h"

class TogglePanel : public UIObject {
    float x, y, width, offset_y, toggle_h, gap;
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
    string selected(int idx);
    float get_height() override;
    private:
    string fold(string str, float max_width);
};

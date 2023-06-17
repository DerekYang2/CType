#pragma once
#include "globals.h"
#include "ToggleGroup.h"

class TogglePanel : public UIObject {
    float x, y, width, offset_y, toggle_h;
    // font_info
    float font_size;
    vector<pair<float, float>> text_height;
    vector<ToggleGroup*> toggles;
    vector<pair<string, string>> toggle_info;  // label, details
    public:
    TogglePanel(float x, float y, float w, vector<ToggleGroup*> toggles, vector<pair<string, string>> toggle_info);
    void update();
    void draw();
    string selected(int idx);
};

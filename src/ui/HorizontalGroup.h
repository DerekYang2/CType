#pragma once 
#include "Utils.h"

class HorizontalGroup : public UIObject {
    vector<UIObject*> objects;
    float cx, cy, padding, tot_width, max_height;
    bool show_rect;
    
    public:
    HorizontalGroup();
    HorizontalGroup(float cx, float cy, float padding, vector<UIObject*> objects, bool show_rect = true);
    void update() override;
    void draw() override;
    void draw_hint() override;
    void set_pos(float x, float y) override;  // x,y are corners, not center
    float get_width() override;
    float get_height() override;
};
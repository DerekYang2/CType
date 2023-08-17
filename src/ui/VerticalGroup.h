#pragma once
#include "Utils.h"

class VerticalGroup : public UIObject {
    std::vector<UIObject*> objects;
    float cx, top_y, padding, max_width, tot_height;
    bool show_rect;

public:
    VerticalGroup();
    VerticalGroup(float cx, float top_y, float padding, std::vector<UIObject*> objects, bool show_rect = true);
    void update() override;
    void draw() override;
    void draw_hint() override;
    void set_pos(float x2, float y2) override;
    float get_width() override;
    float get_height() override;
};

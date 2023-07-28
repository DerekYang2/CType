#pragma once
#include "globals.h"
#include "UIObject.h"

/**
 * Vertical only for now
*/
class Scrollbar : public UIObject {
    float offset = 0;
    Rectangle bar;
    float thumb_h, screen_h, total_h;
    bool hover = false, drag = false;
    float click_offset;
    list<UIObject*> children;  // UIObjects bound to scrollbar

    public:
    // By default, x is right aligned rather than usual left
    // h = actual height of scrollbar, screen height is the height of the screen, total height is the height of the full scrollable content
    Scrollbar(float x, float y, float w, float h, float screen_h, float total_h, list<UIObject*> children = {});
    void update() override;
    void draw() override;
    void shift(float dx, float dy);
    void set_pos(float x2, float y2) override;
    void add_child(UIObject* child);
    float get_offset();
    float offset_at(float y);
    Rectangle bounding_box();
    Rectangle thumb_rect();

};